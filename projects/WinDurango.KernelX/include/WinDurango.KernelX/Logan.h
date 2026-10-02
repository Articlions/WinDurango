#pragma once
#include "ACPLoganTypes.h"
#include "XmaDecoder.h"

BOOL(WINAPI *TrueDeviceIoControl)(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize,
                                  LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned,
                                  LPOVERLAPPED lpOverlapped) = DeviceIoControl;
HANDLE hLoganMap{};
DWORD m_pBumpAllocStart = 0x10000;

DWORD BumpAlloc(_In_ SIZE_T size)
{
    DWORD allocatedBlock = m_pBumpAllocStart;
    m_pBumpAllocStart += (size + 0xFFFF) & ~0xFFFF;
    return allocatedBlock;
}

class LoganHeap
{
  public:
    PVOID GetVirtualAddress(_In_ APU_ADDRESS ApuAddress, _In_ SIZE_T SizeInBytes);

    template <typename T> 
    T *GetVirtualAddress(_In_ APU_ADDRESS ApuAddress, _In_ SIZE_T Count = 1);

    APU_ADDRESS GetAPUAddress(_In_ PVOID CpuAddress);

    HRESULT GetDriverMemory(UINT32 index, LOGAN_PHYSICAL_MEMORY *memory);
    void AllocateDriverMemory();
    HRESULT Map(PVOID cpuAddress, APU_ADDRESS apuAddress, UINT32 sizeInBytes);
    BOOL CheckForBlockedSampleRateConverterContexts(UINT *BlockedIndex);
    BOOL CheckForBlockedDMAContexts(UINT *BlockedIndex);

    // Keeps every flowgraph the title loads, and prints the program the first time it arrives.
    // The flowgraph is the wiring diagram - which unit feeds which mix buffer and where the result
    // is written - so it is what any implementation of the pipeline has to be built against, and
    // the title only ever sends it once.
    void RecordFlowgraph(ACP_COMMAND_LOAD_SHAPE_FLOWGRAPH const &command);

    // Executes the loaded SHAPE program once, as the hardware does every audio frame.
    void RunFlowgraph();

    // Credits every routed target with the frames that have elapsed.
    void OweFrames(UINT32 frames);

    // The client's half of the DMA ring handshake: it takes blocks out of the buffer and then tells
    // the ACP how many, and the ACP moves the pointer. Without this the ring fills once and stops.
    void IncrementDmaPointer(UINT32 contextIndex, UINT32 increment, bool writePointer);

    // Pulls one channel from one decoder context and keeps the shared context's read position and
    // buffer handover in step with it.
    UINT32 ServiceXmaContext(UINT32 index, UINT32 channel, float *out, UINT32 count);

    // The title's half of the input handover: it refills one of a decoder's two input buffers and
    // says so with this command. Until it is acted on, a buffer released by the decoder never comes
    // back and the sound stops after the first two packets.
    void UpdateXmaReadBuffer(ACP_COMMAND_UPDATE_XMA_READ_BUFFER const &command);

    void UpdateXmaContext(ACP_COMMAND_UPDATE_XMA_CONTEXT const &command);

    // Created on first use rather than up front: the title registers 512 contexts and uses a
    // handful, and a voice carries queued samples, so an array of them would be megabytes of
    // memory that never gets touched.
    XmaVoice *_xmaVoices[512]{};

    // Blocks the audio clock says are due and that have not been produced yet, capped at the depth
    // of the ring. This is what stops a burst of client messages from being answered with a burst
    // of audio: time decides how much there is to make, and the ring only decides how much of it
    // can be held at once.
    UINT32 _framesOwed = 0;

    UINT32 _dmaBlocksWritten = 0;
    UINT32 _dmaBlocksSkipped = 0;

    // Indexed by DMA context. Only a couple of hundred are ever used and the array is small enough
    // not to matter, so no mapping is needed to keep it honest.
    // Where each DMA target gets its samples from, kept across programs rather than read out of
    // whichever one happens to be running.
    //
    // A flowgraph says which decoder feeds which target - it is a wiring diagram, not a schedule.
    // Tying block production to how often a program was submitted starved every voice whose program
    // came in rarely: measured over one run, the music target was written 22748 blocks against
    // 23488 taken, while a second voice got 60 written against 304 taken and so played four fifths
    // silence. Routing is remembered; the audio clock decides when a block is due.
    struct DmaSource
    {
        UINT32 xmaContext;
        UINT32 channel;
        bool set;
    };

    static UINT32 constexpr c_TrackedDmaContexts = 256;
    DmaSource _dmaSource[c_TrackedDmaContexts]{};
    // Blocks each target is owed. Per target rather than one figure for all of them, because a ring
    // that is momentarily full must defer its block, not lose it: a shared debt is spent by whoever
    // gets there first, so a target that had to skip never gets that frame back. Measured at 352
    // blocks a second delivered against 375 taken, with the missing 23 skipped against a full ring -
    // six per cent of the audio, dropped a block at a time, which is what the remaining squeak was.
    UINT32 _dmaOwed[c_TrackedDmaContexts]{};
    UINT32 _dmaWrittenPerContext[c_TrackedDmaContexts]{};
    UINT32 _dmaTakenPerContext[c_TrackedDmaContexts]{};
    UINT32 _dmaSkippedPerContext[c_TrackedDmaContexts]{};
    UINT32 _dmaIncrements = 0;
    UINT32 _dmaUnderruns = 0;
    UINT32 _dmaSamplesDecoded = 0;
    UINT32 _dmaSamplesPadded = 0;
    UINT32 _dmaClipped = 0;
    float _dmaPeak = 0.0f;

    // Writes what leaves the pipeline to a pair of raw 32 bit float files, one per channel, next to
    // the trace. Every counter here can be perfect while the audio is still wrong - wrong order,
    // repeated blocks, a channel doubled - and none of them would show it. The recording turns
    // "it sounds rough" into something that can be measured.
    void DumpPcm(UINT32 channel, float const *samples, UINT32 count);
    HANDLE _pcmDump[2]{};
    UINT32 _pcmDumped[2]{};
    static UINT32 constexpr c_PcmDumpLimit = 48000 * 20;

    UINT32 _flowgraphHead = 0;
    UINT32 _flowgraphTail = 0;
    UINT32 _flowgraphQueued = 0;
    UINT32 _flowgraphsRun = 0;

    PVOID _view;
    SIZE_T _sizeInBytes;
    LOGAN_PHYSICAL_MEMORY _driverMemory[3];
    static ULONGLONG constexpr c_XMemAttributes = 0xEC810000; 
    ACP_COMMAND_REGISTER_CONTEXT_ARRAYS _acpContextArrays;
    // The connect command carries the addresses of the title's command and message queues, and it
    // has to outlive the dispatch call that delivered it. It used to be stored as a pointer to
    // the by-value parameter, so every queue address was later read back off a dead stack frame -
    // which is why no ACP message could ever reach the title, not even COMMAND_COMPLETED.
    AcpCommand _acpConnectStorage[4]{};
    AcpCommand_Old _acpConnectStorageOld[4]{};
    AcpCommand *_acpConnectCommand[4]{};
    AcpCommand_Old *_acpConnectCommandOld[4]{};
    APU_ADDRESS _flowgraph;
    UINT32 _numCommandsInFlowgraph = 0;
    UINT32 _enabledMessages = 0;
    // One counter per ACP command type. The per-type log lines are rationed, so a command
    // that arrives twice in five minutes and one that arrives a thousand times a second
    // both print about three lines; the totals are what tell them apart.
    UINT32 _clientCommandCounts[64]{};
    // Completions are numbered by the ACP, from one, and not by echoing the caller's own tag back
    // at it. See where this is used for the arithmetic that settles which of the two it is.
    UINT64 _completedCommands = 0;
    UINT32 _droppedMessages = 0;
    // Counts the audio frames the emulated ACP has announced. Hardware advances this every 128
    // samples and titles use it to pace their audio pipeline.
    UINT32 _audioFrame = 0;
    // Off, because the census settled what these messages are worth: not one SRC context and not
    // one DMA context is ever configured - all 512 and all 1024 of them stay zero for a whole run.
    // "Sample rate converter 0 is starved" is therefore a statement about a unit the title never
    // set up, invented by us and delivered ninety-four times a second, and it is the only traffic
    // we send. A wrong message is worse than no message: hardware that has nothing to report says
    // nothing, and the title did not even register for the one beat it would get anyway
    // (mask 0xFE has AUDIO_FRAME_START clear).
    //
    // Set it back to TRUE to A/B this: everything around it still works, the queue just goes quiet.
    BOOL _emitStarvationMessages = FALSE;
    BOOL _disabledXmaContexts[512]{};
};

// Defined in Logan.cpp. Writes to a dedicated file because the console is swamped by the WinRT
// logger within seconds of startup.
void AcpTrace(char const *format, ...);

template <typename T> void _declspec(dllexport) DispatchLoganCommand(LOGAN_COMMAND_TYPE cmdType, T cmd);

LoganHeap g_LoganHeap;

_Use_decl_annotations_ DWORD DispatchGetDriverMemory(DWORD IoControlCode, PVOID InBuffer, DWORD InBufferSize,
                                                     PVOID OutBuffer, DWORD OutBufferSize, PDWORD BytesReturned,
                                                     LPOVERLAPPED Overlapped);

template <typename T>
static BOOL ReadFromRingBuffer(T *OutBuffer, T *InBuffer, LOGAN_RING_BUFFER_DESCRIPTOR *InBufferDesc)
{
    if (InBufferDesc->offsetRead == InBufferDesc->offsetWrite)
        return FALSE;

    *OutBuffer = InBuffer[InBufferDesc->offsetRead];
    InBufferDesc->offsetRead =
        InBufferDesc->offsetRead + 1 < InBufferDesc->sizeInBlocks ? InBufferDesc->offsetRead + 1 : 0;
    return TRUE;
}

static BOOL ReadFromInternalACPRingBuffer(AcpCommand *OutBuffer, AcpInternalCommandQueueEntry* InBuffer, AcpState *InBufferDesc)
{
    if (InBufferDesc->internalCommandQueueReadCounter == InBufferDesc->internalCommandQueueSendCounter)
        return FALSE;

    *OutBuffer = InBuffer[InBufferDesc->internalCommandQueueReadPointer].command;
    InBuffer[InBufferDesc->internalCommandQueueReadPointer].state = 0;

    InBufferDesc->internalCommandQueueReadPointer++;
    InBufferDesc->internalCommandQueueReadCounter++;

    if (InBufferDesc->internalCommandQueueReadPointer >= 16)
        InBufferDesc->internalCommandQueueReadPointer = 0;

    return TRUE;
}

static BOOL ReadFromInternalACPRingBufferOld(AcpCommand_Old *OutBuffer, AcpInternalCommandQueueEntry_Old* InBuffer, AcpState_Old *InBufferDesc)
{
    if (!InBuffer[InBufferDesc->internalCommandQueueReadPointer].command.commandType)
        return FALSE;

    *OutBuffer = InBuffer[InBufferDesc->internalCommandQueueReadPointer].command;
    InBuffer[InBufferDesc->internalCommandQueueReadPointer].state = 0;

    InBufferDesc->internalCommandQueueReadPointer++;

    if (InBufferDesc->internalCommandQueueReadPointer >= 16)
        InBufferDesc->internalCommandQueueReadPointer = 0;

    return TRUE;
}

static BOOL ReadFromClientACPRingBuffer(AcpCommand *OutBuffer, AcpCommandQueueEntry *InBuffer, AcpState *InBufferDesc, UINT ClientIndex)
{
    if (InBufferDesc->clientCommandQueueReadCounter[ClientIndex] == InBufferDesc->clientCommandQueueSendCounter[ClientIndex])
        return FALSE;

    // Copy first, then release the slot. AcpHal publishes a command by writing state = 1 and
    // refuses to fill an entry whose state is not zero - it returns 0x8AC80003 and the command is
    // never sent. Clearing the field is how the ACP says "taken". This used to write 1, which is
    // what AcpHal had already put there, so the ring filled up permanently the first time it
    // wrapped: the title got exactly numCommands commands through - 1024 of them, counted in the
    // run - and then its audio engine stopped with no error reported anywhere.
    *OutBuffer = InBuffer[InBufferDesc->clientCommandQueueReadPointer[ClientIndex]].command;
    InBuffer[InBufferDesc->clientCommandQueueReadPointer[ClientIndex]].state = 0;

    InBufferDesc->clientCommandQueueReadPointer[ClientIndex]++;
    InBufferDesc->clientCommandQueueReadCounter[ClientIndex]++;

    if (InBufferDesc->clientCommandQueueReadPointer[ClientIndex] >= g_LoganHeap._acpConnectCommand[ClientIndex]->connect.numCommands)
        InBufferDesc->clientCommandQueueReadPointer[ClientIndex] = 0;

    return TRUE;
}

static BOOL ReadFromClientACPRingBufferOld(AcpCommand_Old *OutBuffer, AcpCommandQueueEntry_Old *InBuffer, AcpState_Old *InBufferDesc, UINT ClientIndex)
{
    if (!InBuffer[InBufferDesc->clientCommandQueueReadPointer[ClientIndex]].command.commandType)
        return FALSE;

    *OutBuffer = InBuffer[InBufferDesc->clientCommandQueueReadPointer[ClientIndex]].command;
    InBuffer[InBufferDesc->clientCommandQueueReadPointer[ClientIndex]].state = 0;

    InBufferDesc->clientCommandQueueReadPointer[ClientIndex]++;

    if (InBufferDesc->clientCommandQueueReadPointer[ClientIndex] >= g_LoganHeap._acpConnectCommandOld[ClientIndex]->connect.numCommands)
        InBufferDesc->clientCommandQueueReadPointer[ClientIndex] = 0;

    return TRUE;
}

void SendMessageFromACP(ACP_MESSAGE *pMessage, AcpMessageQueueEntry *pMessageQueue, AcpState *pAcpState, UINT ClientIndex);

LOGAN_COMMAND_ACP_INIT *InitialCommand = nullptr;
LOGAN_COMMAND_ACP_INIT_OLD *InitialCommandOld = nullptr;

inline void DispatchACPCommand(ACP_COMMAND_TYPE_INTERNAL cmdType, AcpState *acpState, AcpCommand Cmd);
inline void DispatchACPCommandOld(ACP_COMMAND_TYPE_INTERNAL cmdType, AcpState_Old *acpState, AcpCommand_Old Cmd);

template <typename T> 
inline void DispatchClientACPCommand(ACP_COMMAND_TYPE cmdType, AcpState *acpState, T Cmd);

template <typename T> 
inline void DispatchClientACPCommandOld(ACP_COMMAND_TYPE cmdType, AcpState_Old *acpState, T Cmd);

template <typename T> 
inline void DispatchLoganCommand(LOGAN_COMMAND_TYPE cmdType, T cmd);

void SendMessageFromACP(ACP_MESSAGE *pMessage, AcpMessageQueueEntry *pMessageQueue, AcpState *pAcpState, UINT ClientIndex);

void SendMessageFromACPOld(ACP_MESSAGE_OLD *pMessage, AcpMessageQueueEntry_Old *pMessageQueue, AcpState *pAcpState, UINT ClientIndex);

void SendMessageFromACPOlder(ACP_MESSAGE_OLD *pMessage, AcpMessageQueueEntry_Old *pMessageQueue, AcpState_Old *pAcpState, UINT ClientIndex);

static DWORD WINAPI LoganChannelProc(LPVOID lpThreadParameter)
{
    SetThreadDescription(GetCurrentThread(), L"Logan Thread");

    auto channel = (LOGAN_CHANNEL *)lpThreadParameter;
    auto commands = g_LoganHeap.GetVirtualAddress<LOGAN_COMMAND_INTERNAL>(channel->commands.apuAddress,
                                                                          channel->commands.sizeInBlocks);
    auto messages = g_LoganHeap.GetVirtualAddress<LOGAN_MESSAGE_INTERNAL>(channel->messages.apuAddress,
                                                                          channel->messages.sizeInBlocks);

    AcpState *pAcpState = nullptr;
    AcpState_Old *pAcpStateOld = nullptr;
    CpuState *pCpuState = nullptr;

    while (true)
    {
        LOGAN_COMMAND_INTERNAL command{};
        while (ReadFromRingBuffer(&command, commands, &channel->commands))
        {
            DispatchLoganCommand((LOGAN_COMMAND_TYPE)command.commandType, g_LoganHeap.GetVirtualAddress(command.apuAddress, sizeof((LOGAN_COMMAND_TYPE)command.commandType)));
        }

        if (InitialCommand != nullptr)
        {
            AcpInternalCommandQueueEntry *AcpInternalCommandQueue = g_LoganHeap.GetVirtualAddress<AcpInternalCommandQueueEntry>(InitialCommand->acpCommandQueue);
            if (!pAcpState) pAcpState = g_LoganHeap.GetVirtualAddress<AcpState>(InitialCommand->acpState);

            if (pAcpState && AcpInternalCommandQueue)
            {
                AcpCommand Command{};
                while (ReadFromInternalACPRingBuffer(&Command, AcpInternalCommandQueue, pAcpState))
                {
                    if (Command.commandType)
                    {
                        DispatchACPCommand((ACP_COMMAND_TYPE_INTERNAL)Command.commandType, pAcpState, Command);
                    }
                }
            }
        }
        else if (InitialCommandOld != nullptr)
        {
            AcpInternalCommandQueueEntry_Old *AcpInternalCommandQueue = g_LoganHeap.GetVirtualAddress<AcpInternalCommandQueueEntry_Old>(InitialCommandOld->acpCommandQueue);
            if (!pAcpStateOld) pAcpStateOld = g_LoganHeap.GetVirtualAddress<AcpState_Old>(InitialCommandOld->acpState);
            if (!pCpuState) pCpuState = g_LoganHeap.GetVirtualAddress<CpuState>(InitialCommandOld->cpuState);

            if (pAcpStateOld && AcpInternalCommandQueue)
            {
                AcpCommand_Old Command{};
                while (ReadFromInternalACPRingBufferOld(&Command, AcpInternalCommandQueue, pAcpStateOld))
                {
                    if (Command.commandType)
                    {
                        DispatchACPCommandOld((ACP_COMMAND_TYPE_INTERNAL)Command.commandType, pAcpStateOld, Command);
                    }
                }
            }
        }

        // On hardware the ACP is a DSP that runs continuously and beats out an audio frame
        // message the title clocks its audio pipeline against. Nothing here ever produced that
        // beat: the only message ever sent was COMMAND_COMPLETED in reply to a command, so a
        // title that waits for the pipeline to advance waits forever. Forza does exactly that
        // once its intro finishes, which is why it softlocks on a black screen with audio
        // registered but never processed.
        //
        // One Xbox audio frame is 128 samples at 48 kHz, so a beat is due about every 2.67 ms.
        UINT FramesDue = 0;
        static LARGE_INTEGER Frequency{};
        static LARGE_INTEGER Last{};

        if (!Frequency.QuadPart)
        {
            QueryPerformanceFrequency(&Frequency);
            QueryPerformanceCounter(&Last);
        }

        LARGE_INTEGER Now{};
        QueryPerformanceCounter(&Now);

        LONGLONG const TicksPerFrame = Frequency.QuadPart * 128 / 48000;
        if (TicksPerFrame > 0)
        {
            FramesDue = static_cast<UINT>((Now.QuadPart - Last.QuadPart) / TicksPerFrame);

            // Never flush a long backlog in one go - a stall would otherwise swamp the queue.
            if (FramesDue > 8)
                FramesDue = 8;

            if (FramesDue)
                Last.QuadPart += static_cast<LONGLONG>(FramesDue) * TicksPerFrame;
        }

        // The frame counter belongs to the ACP, not to a client: it used to be advanced inside the
        // per-client loop, so a second connected client would have made time run twice as fast.
        UINT32 const frameBase = g_LoganHeap._audioFrame;
        g_LoganHeap._audioFrame += FramesDue;

        // The clock says how much is owed; the ring says how much of it fits at once. Those are two
        // different limits and the debt has to outlive the ring being full, or time is lost for
        // good every time a pass runs late.
        //
        // Capping the debt at the depth of the ring looked reasonable and was not: the client's
        // period is exactly four frames, so the debt sits at the cap permanently and every scrap of
        // scheduling jitter is written off. That measured as 3466 blocks delivered against 3752
        // wanted - seven per cent short, nine hundred underruns in a run where there had been one -
        // and it is what a track played slightly slow sounds like.
        //
        // Forty milliseconds is far more than any jitter and far less than a backlog worth keeping;
        // past that the machine is simply not keeping up and there is nothing to catch up to.
        g_LoganHeap.OweFrames(FramesDue);
        g_LoganHeap.RunFlowgraph();

        for (UINT i = 0; i < 4; i++)
        {
            if (g_LoganHeap._acpConnectCommand[i] && pAcpState)
            {
                AcpCommandQueueEntry *AcpClientCommandQueue = g_LoganHeap.GetVirtualAddress<AcpCommandQueueEntry>(g_LoganHeap._acpConnectCommand[i]->connect.commandQueue, g_LoganHeap._acpConnectCommand[i]->connect.numCommands);
                AcpPendingCommand *AcpClientPendingCommand = g_LoganHeap.GetVirtualAddress<AcpPendingCommand>(g_LoganHeap._acpConnectCommand[i]->connect.pendingCommandList);
                AcpMessageQueueEntry *AcpClientMessageQueue = nullptr;
                AcpMessageQueueEntry_Old *AcpClientMessageQueueOld = nullptr;
                if (g_ABI >= abi_t{6,2,11785,0})
                    AcpClientMessageQueue = g_LoganHeap.GetVirtualAddress<AcpMessageQueueEntry>(g_LoganHeap._acpConnectCommand[i]->connect.messageQueue, g_LoganHeap._acpConnectCommand[i]->connect.numMessages);
                else
                    AcpClientMessageQueueOld = g_LoganHeap.GetVirtualAddress<AcpMessageQueueEntry_Old>(g_LoganHeap._acpConnectCommand[i]->connect.messageQueue, g_LoganHeap._acpConnectCommand[i]->connect.numMessages);

                if (pAcpState && AcpClientCommandQueue)
                {
                    AcpCommand Command{};
                    while (ReadFromClientACPRingBuffer(&Command, AcpClientCommandQueue, pAcpState, i))
                    {
                        if (Command.commandType)
                        {
                            DispatchClientACPCommand((ACP_COMMAND_TYPE)Command.commandType, pAcpState, Command);

                            // The frame a command completed in is the frame it completed in, not the
                            // frame the caller asked it to be scheduled for. Those were the same
                            // field here, so every completion carried the caller's own 0xFFFFFFFF -
                            // "no particular frame" - straight back to it. A client that waits until
                            // the reported frame has passed can never be satisfied by 0xFFFFFFFF.
                            UINT32 const completedIn = g_LoganHeap._audioFrame;

                            // The completion carries the ACP's own sequence number, counted from
                            // one, not the tag the caller put in the command.
                            //
                            // The title's handler at Forza+0x19D50CB only accepts an id equal to
                            // its last-in-order plus one, and that counter starts at zero, so the
                            // first completion it will accept is 1. Forza's own first command is
                            // tagged 0, and echoing that back is what put the manager's outstanding
                            // command count at -1 and kept it there: with a negative count it never
                            // publishes a completed id to anything waiting on one. The counters
                            // read out of the live title say exactly this - last in order 0 while
                            // outstanding is already -1, before the second command was ever sent.
                            UINT64 const sequence = ++g_LoganHeap._completedCommands;

                            if ((g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_COMMAND_COMPLETED) && AcpClientMessageQueueOld)
                            {
                                ACP_MESSAGE_OLD Message{};
                                Message.type = ACP_MESSAGE_TYPE_COMMAND_COMPLETED;
                                Message.commandCompleted.commandId = Command.commandId != 0 ? Command.commandId : sequence;
                                Message.commandCompleted.commandType = Command.commandType;
                                Message.commandCompleted.audioFrame = completedIn;
                                Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                                SendMessageFromACPOld(&Message, AcpClientMessageQueueOld, pAcpState, i);
                            }
                            else if ((g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_COMMAND_COMPLETED) && AcpClientMessageQueue)
                            {
                                ACP_MESSAGE Message{};
                                Message.type = ACP_MESSAGE_TYPE_COMMAND_COMPLETED;
                                Message.commandCompleted.commandId = Command.commandId != 0 ? Command.commandId : sequence;
                                Message.commandCompleted.commandType = Command.commandType;
                                Message.commandCompleted.audioFrame = completedIn;
                                Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                                SendMessageFromACP(&Message, AcpClientMessageQueue, pAcpState, i);
                            }
                        }
                    }
                }

                // FLOWGRAPH_COMPLETED is the beat the title's audio engine runs on, and it was the
                // one thing we never sent.
                //
                // Read out of the title at Forza+0x19D4ED8. Every thread that needs the audio
                // engine to advance calls a helper that spins on one counter:
                //
                //     wanted  = request[+0x18]
                //     while (manager[+0x254] < wanted) { SetEvent(kick); WaitForSingleObject(done); }
                //
                // and the only thing that ever raises manager[+0x254] is the ACP message handler at
                // Forza+0x19D5128: on message type 2 it bumps a small counter, and every fourth one
                // raises [+0x254]. So one audio period is four flowgraph completions, and until
                // those arrive the very first sound never finishes being created - which is why the
                // XMA context array stays zero and the ACP thread burns a core polling.
                //
                // The mask the title registers says the same thing: 0xFE has AUDIO_FRAME_START
                // clear and FLOWGRAPH_COMPLETED set. It never wanted the frame-start beat; it wanted
                // this one. SRC_BLOCKED and DMA_BLOCKED, which we used to send instead, reach a
                // branch in the same handler that does nothing but format a warning string.
                //
                // Hardware runs the flowgraph once per audio frame, so this is emitted once per
                // frame: 375 a second, giving the title 93.75 audio periods a second - a 512 sample
                // period at 48 kHz, which is what its buffers are sized for.
                for (UINT Beat = 0; Beat < FramesDue; ++Beat)
                {
                    UINT32 const frame = frameBase + Beat + 1;

                    if (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
                    {
                        if (AcpClientMessageQueueOld)
                        {
                            ACP_MESSAGE_OLD Message{};
                            Message.type = ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            Message.flowgraphCompleted.flowgraph = g_LoganHeap._flowgraph;
                            SendMessageFromACPOld(&Message, AcpClientMessageQueueOld, pAcpState, i);
                        }
                        else if (AcpClientMessageQueue)
                        {
                            ACP_MESSAGE Message{};
                            Message.type = ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            Message.flowgraphCompleted.flowgraph = g_LoganHeap._flowgraph;
                            SendMessageFromACP(&Message, AcpClientMessageQueue, pAcpState, i);
                        }
                    }

                    // Every eighth frame is often enough to prompt a starved pipeline without
                    // filling the queue faster than the title drains it.
                    if ((frame % 8) != 0)
                        continue;

                    UINT BlockedIndex = 0;

                    if (g_LoganHeap._emitStarvationMessages &&
                        (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_SRC_BLOCKED) &&
                        g_LoganHeap.CheckForBlockedSampleRateConverterContexts(&BlockedIndex))
                    {
                        if (AcpClientMessageQueueOld)
                        {
                            ACP_MESSAGE_OLD Message{};
                            Message.type = ACP_MESSAGE_TYPE_SRC_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACPOld(&Message, AcpClientMessageQueueOld, pAcpState, i);
                        }
                        else if (AcpClientMessageQueue)
                        {
                            ACP_MESSAGE Message{};
                            Message.type = ACP_MESSAGE_TYPE_SRC_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACP(&Message, AcpClientMessageQueue, pAcpState, i);
                        }
                    }

                    if (g_LoganHeap._emitStarvationMessages &&
                        (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_DMA_BLOCKED) &&
                        g_LoganHeap.CheckForBlockedDMAContexts(&BlockedIndex))
                    {
                        if (AcpClientMessageQueueOld)
                        {
                            ACP_MESSAGE_OLD Message{};
                            Message.type = ACP_MESSAGE_TYPE_DMA_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACPOld(&Message, AcpClientMessageQueueOld, pAcpState, i);
                        }
                        else if (AcpClientMessageQueue)
                        {
                            ACP_MESSAGE Message{};
                            Message.type = ACP_MESSAGE_TYPE_DMA_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACP(&Message, AcpClientMessageQueue, pAcpState, i);
                        }
                    }
                }
            }
            else if (g_LoganHeap._acpConnectCommandOld[i] && pAcpStateOld)
            {
                AcpCommandQueueEntry_Old *AcpClientCommandQueue = g_LoganHeap.GetVirtualAddress<AcpCommandQueueEntry_Old>(g_LoganHeap._acpConnectCommandOld[i]->connect.commandQueue, g_LoganHeap._acpConnectCommandOld[i]->connect.numCommands);
                AcpMessageQueueEntry_Old *AcpClientMessageQueue = g_LoganHeap.GetVirtualAddress<AcpMessageQueueEntry_Old>(g_LoganHeap._acpConnectCommandOld[i]->connect.messageQueue, g_LoganHeap._acpConnectCommandOld[i]->connect.numMessages);
                AcpPendingCommand *AcpClientPendingCommand = g_LoganHeap.GetVirtualAddress<AcpPendingCommand>(g_LoganHeap._acpConnectCommandOld[i]->connect.pendingCommandList);

                if (pAcpStateOld && AcpClientCommandQueue)
                {
                    AcpCommand_Old Command{};
                    while (ReadFromClientACPRingBufferOld(&Command, AcpClientCommandQueue, pAcpStateOld, i))
                    {
                        if (Command.commandType)
                        {
                            DispatchClientACPCommandOld((ACP_COMMAND_TYPE)Command.commandType, pAcpStateOld, Command);

                            // The frame a command completed in is the frame it completed in, not the
                            // frame the caller asked it to be scheduled for. Those were the same
                            // field here, so every completion carried the caller's own 0xFFFFFFFF -
                            // "no particular frame" - straight back to it. A client that waits until
                            // the reported frame has passed can never be satisfied by 0xFFFFFFFF.
                            UINT32 const completedIn = g_LoganHeap._audioFrame;

                            // The completion carries the ACP's own sequence number, counted from
                            // one, not the tag the caller put in the command.
                            //
                            // The title's handler at Forza+0x19D50CB only accepts an id equal to
                            // its last-in-order plus one, and that counter starts at zero, so the
                            // first completion it will accept is 1. Forza's own first command is
                            // tagged 0, and echoing that back is what put the manager's outstanding
                            // command count at -1 and kept it there: with a negative count it never
                            // publishes a completed id to anything waiting on one. The counters
                            // read out of the live title say exactly this - last in order 0 while
                            // outstanding is already -1, before the second command was ever sent.
                            UINT64 const sequence = ++g_LoganHeap._completedCommands;

                            if (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_COMMAND_COMPLETED)
                            {
                                ACP_MESSAGE_OLD Message{};
                                Message.type = ACP_MESSAGE_TYPE_COMMAND_COMPLETED;
                                Message.commandCompleted.commandId = Command.commandId != 0 ? Command.commandId : sequence;
                                Message.commandCompleted.commandType = Command.commandType;
                                Message.commandCompleted.audioFrame = completedIn;
                                Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                                SendMessageFromACPOlder(&Message, AcpClientMessageQueue, pAcpStateOld, i);
                            }
                        }
                    }
                }

                for (UINT Beat = 0; Beat < FramesDue; ++Beat)
                {
                    UINT32 const frame = frameBase + Beat + 1;

                    if (!AcpClientMessageQueue)
                        continue;

                    if (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
                    {
                        ACP_MESSAGE_OLD Message{};
                        Message.type = ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED;
                        Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                        Message.flowgraphCompleted.flowgraph = g_LoganHeap._flowgraph;
                        SendMessageFromACPOlder(&Message, AcpClientMessageQueue, pAcpStateOld, i);
                    }

                    if ((frame % 8) != 0)
                        continue;

                    UINT BlockedIndex = 0;

                    if (g_LoganHeap._emitStarvationMessages &&
                        (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_SRC_BLOCKED) &&
                        g_LoganHeap.CheckForBlockedSampleRateConverterContexts(&BlockedIndex))
                    {
                        if (AcpClientMessageQueue)
                        {
                            ACP_MESSAGE_OLD Message{};
                            Message.type = ACP_MESSAGE_TYPE_SRC_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACPOlder(&Message, AcpClientMessageQueue, pAcpStateOld, i);
                        }
                    }

                    if (g_LoganHeap._emitStarvationMessages &&
                        (g_LoganHeap._enabledMessages & ACP_MESSAGE_TYPE_DMA_BLOCKED) &&
                        g_LoganHeap.CheckForBlockedDMAContexts(&BlockedIndex))
                    {
                        if (AcpClientMessageQueue)
                        {
                            ACP_MESSAGE_OLD Message{};
                            Message.type = ACP_MESSAGE_TYPE_DMA_BLOCKED;
                            Message.shapeCommandBlocked.contextIndex = BlockedIndex;
                            Message.shapeCommandBlocked.flowgraph = g_LoganHeap._flowgraph;
                            Message.droppedMessageCount = g_LoganHeap._droppedMessages;
                            SendMessageFromACPOlder(&Message, AcpClientMessageQueue, pAcpStateOld, i);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

HANDLE LoganHandle = nullptr;

EXTERN_C BOOL __stdcall EraDeviceIoControl(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer,
                                           DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize,
                                           LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped)
{
    if (hDevice != LoganHandle)
    {
        return TrueDeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize,
                               lpBytesReturned, lpOverlapped);
    }
    else
    {
        if (dwIoControlCode == IOCTL_LOGAN_ALLOC_MAP) // 0x800E007
        {
            LOGAN_IOCTL_IN_ALLOC_MAP *InMap = reinterpret_cast<LOGAN_IOCTL_IN_ALLOC_MAP *>(lpInBuffer);

            UINT Size = (InMap->sizeInBytes + ((1ULL << 16) - 1)) & ~((1ULL << 16) - 1);
            UINT NumPages = Size / (1ULL << 16);
            DWORD ApuAddress = BumpAlloc(Size);
            g_LoganHeap.Map(InMap->address, ApuAddress, InMap->sizeInBytes);

            LOGAN_IOCTL_OUT_ALLOC_MAP outMap{};
            outMap.allocated = InMap->sizeInBytes;
            outMap.address = InMap->address;
            outMap.apuAddress = ApuAddress;
            outMap.reserved0 = 0;

            *reinterpret_cast<LOGAN_IOCTL_OUT_ALLOC_MAP *>(lpOutBuffer) = outMap;
            *lpBytesReturned = 32;
            SetLastError(ERROR_SUCCESS);
        }
        else if (dwIoControlCode == IOCTL_LOGAN_ATTACH_CHANNEL) // 0x800E00B
        {
            LOGAN_IOCTL_IN_ATTACH_CHANNEL *In = reinterpret_cast<LOGAN_IOCTL_IN_ATTACH_CHANNEL *>(lpInBuffer);
            LOGAN_CHANNEL *Channel = g_LoganHeap.GetVirtualAddress<LOGAN_CHANNEL>(In->apuAddress);
            std::thread LoganThread(LoganChannelProc, Channel);

            if (In->core == LOGAN_CORE_SHAPE)
            {
                printf("Logan Core is LOGAN_CORE_SHAPE.\n");
            }
            else if (In->core == LOGAN_CORE_ACP)
            {
                printf("Logan Core is LOGAN_CORE_ACP.\n");
            }
            else if (In->core == LOGAN_CORE_NONE)
            {
                printf("Logan Core is LOGAN_CORE_NONE.\n");
            }
            else if (In->core == LOGAN_CORE_AVP)
            {
                printf("Logan Core is LOGAN_CORE_AVP.\n");
            }
            else if (In->core == LOGAN_CORE_ASP)
            {
                printf("Logan Core is LOGAN_CORE_ASP.\n");
            }

            LoganThread.detach();
            SetLastError(ERROR_SUCCESS);
        }
        else if (dwIoControlCode == IOCTL_LOGAN_GET_MEMORY_STATISTICS)
        {
            AcpTrace("[IOCTL] IOCTL_LOGAN_GET_MEMORY_STATISTICS unhandled");
        }
        else if (dwIoControlCode == IOCTL_LOGAN_GET_NUI_BUFFER_STATE)
        {
            AcpTrace("[IOCTL] IOCTL_LOGAN_GET_NUI_BUFFER_STATE unhandled");
        }
        else if (dwIoControlCode == IOCTL_LOGAN_SET_NUI_MEC_BUFFER_WRITE_OFFSET)
        {
            AcpTrace("[IOCTL] IOCTL_LOGAN_SET_NUI_MEC_BUFFER_WRITE_OFFSET unhandled");
        }
        else if (dwIoControlCode == IOCTL_LOGAN_LOAD_ACP_FIRMWARE)
        {
            SetLastError(ERROR_SUCCESS);
            return TRUE;
        }
        else if (dwIoControlCode == IOCTL_LOGAN_SET_USB_CAPTURE_STATE)
        {
            AcpTrace("[IOCTL] IOCTL_LOGAN_SET_USB_CAPTURE_STATE unhandled");
        }
        else if (dwIoControlCode == IOCTL_LOGAN_GET_DRIVER_MEMORY)
        {
            DispatchGetDriverMemory(dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize,
                                    lpBytesReturned, lpOverlapped);
            SetLastError(ERROR_SUCCESS);
        }
        else
        {
            //This shouldn't happen since all known codes are listed above.
            AcpTrace("[IOCTL] unknown Logan code 0x%08X", dwIoControlCode);
        }

        return TRUE;
    }
}

void LoganInit()
{
    GetCombaseVersion();
    hLoganMap = CreateFileMapping2(INVALID_HANDLE_VALUE, nullptr, FILE_MAP_READ | FILE_MAP_WRITE, PAGE_READWRITE, SEC_RESERVE, 0x20000000, nullptr, nullptr, 0);
    g_LoganHeap.AllocateDriverMemory();
    g_LoganHeap._view = MapViewOfFile(hLoganMap, FILE_MAP_WRITE, 0, 0, 0);
    g_LoganHeap._sizeInBytes = 0x20000000;
    printf("Logan was initialized.\n");
}
