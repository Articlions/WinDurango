#include "Logan.h"
#include <algorithm>
#include <math.h>

PVOID LoganHeap::GetVirtualAddress(APU_ADDRESS ApuAddress, SIZE_T SizeInBytes)
{
    if (!ApuAddress || (ApuAddress + SizeInBytes) >= _sizeInBytes)
        return nullptr;
    return (PVOID)((ULONG_PTR)_view + ApuAddress);
}

APU_ADDRESS LoganHeap::GetAPUAddress(PVOID CpuAddress)
{
    return (UINT64)CpuAddress - (UINT64)_view;
}

HRESULT LoganHeap::GetDriverMemory(UINT32 index, LOGAN_PHYSICAL_MEMORY *memory)
{
    *memory = _driverMemory[index];
    return S_OK;
}

template <typename T> T *LoganHeap::GetVirtualAddress(APU_ADDRESS ApuAddress, SIZE_T Count)
{
    return (T *)GetVirtualAddress(ApuAddress, sizeof(T) * Count);
}

_Use_decl_annotations_
HRESULT LoganHeap::Map(PVOID cpuAddress, APU_ADDRESS apuAddress,UINT32 sizeInBytes)
{
    if (!cpuAddress)
        return E_INVALIDARG;
    if (apuAddress == 0)
        return E_INVALIDARG;
    if (sizeInBytes == 0)
        return E_INVALIDARG;
    if (!!(sizeInBytes & (PAGE_SIZE_4KB - 1)))
        return E_INVALIDARG;

    sizeInBytes = (sizeInBytes + (PAGE_SIZE_64K - 1)) & ~(PAGE_SIZE_64K - 1);

    // It is assumed that the virtual address is the start of a range of 64K placeholders.
    for (UINT32 i = 0; i < sizeInBytes / PAGE_SIZE_64K; i++)
    {
        LPVOID pageAddress = (LPVOID)((ULONG_PTR)cpuAddress + PAGE_SIZE_64K * i);

        // If the page is already mapped, we need to unmap it first.
        UnmapViewOfFileEx(pageAddress, MEM_PRESERVE_PLACEHOLDER);

        // Any failures here are treated as fatal errors, we expect these to succeed.
        PVOID Mapped = MapViewOfFile3(hLoganMap, nullptr, pageAddress, apuAddress + PAGE_SIZE_64K * i, PAGE_SIZE_64K, MEM_REPLACE_PLACEHOLDER, PAGE_READWRITE, nullptr, 0);
        if (!Mapped)
        {
            printf("LoganHeap::Map: MapViewOfFile3 failed!\n");
        }

        PVOID Allocated = VirtualAlloc2(nullptr, pageAddress, PAGE_SIZE_64K, MEM_COMMIT, PAGE_READWRITE, nullptr, 0);
        if (!Allocated)
        {
            printf("LoganHeap::Map: VirtualAlloc2 failed!\n");
        }
    }

    return S_OK;
}

void LoganHeap::AllocateDriverMemory()
{
    for (SIZE_T i = 0; i < ARRAYSIZE(_driverMemory); i++)
    {
        _driverMemory[i].sizeInBytes = 0x100000;
        UINT Size = (_driverMemory[i].sizeInBytes + ((1ULL << 16) - 1)) & ~((1ULL << 16) - 1);
        UINT NumPages = Size / (1ULL << 16);
        _driverMemory[i].address = XMemAlloc(_driverMemory[i].sizeInBytes, c_XMemAttributes);
        if (_driverMemory[i].address == 0)
        {
            throw GetLastError();
        }
        _driverMemory[i].apuAddress = BumpAlloc(_driverMemory[i].sizeInBytes);
        Map(_driverMemory[i].address, _driverMemory[i].apuAddress, _driverMemory[i].sizeInBytes);
    }
}

_Use_decl_annotations_ DWORD DispatchGetDriverMemory(DWORD IoControlCode, PVOID InBuffer, DWORD InBufferSize,
                                                     PVOID OutBuffer, DWORD OutBufferSize, PDWORD BytesReturned,
                                                     LPOVERLAPPED Overlapped)
{
    auto In = reinterpret_cast<LOGAN_IOCTL_IN_GET_DRIVER_MEMORY const *>(InBuffer);
    auto Out = reinterpret_cast<LOGAN_IOCTL_OUT_GET_DRIVER_MEMORY *>(OutBuffer);

    LOGAN_PHYSICAL_MEMORY memory{};
    g_LoganHeap.GetDriverMemory(In->type, &memory);

    ZeroMemory(Out, sizeof(LOGAN_IOCTL_OUT_GET_DRIVER_MEMORY));
    Out->address = memory.address;
    Out->apuAddress = memory.apuAddress;
    Out->sizeInBytes = memory.sizeInBytes;

    if (BytesReturned)
        *BytesReturned = sizeof(LOGAN_IOCTL_OUT_GET_DRIVER_MEMORY);

    return ERROR_SUCCESS;
}
 
inline void DispatchACPCommand(ACP_COMMAND_TYPE_INTERNAL cmdType, AcpState *acpState, AcpCommand Cmd)
{
    if (cmdType == INTERNAL_ACP_COMMAND_TYPE_CONNECT)
    {
        g_LoganHeap._acpConnectStorage[Cmd.connect.index] = Cmd;
        g_LoganHeap._acpConnectCommand[Cmd.connect.index] = &g_LoganHeap._acpConnectStorage[Cmd.connect.index];
        printf("Registered main ACP command and message queues.\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_DISCONNECT)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_DISCONNECT)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_REGISTER_CONTEXT_ARRAYS)
    {
        g_LoganHeap._acpContextArrays = Cmd.registerContextArrays;
        printf("Registered ACP context arrays.\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_INIT)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_INIT)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_ENABLE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_ENABLE)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_DISABLE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_DISABLE)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_TERMINATE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_TERMINATE)\n");
    }
    else
    {
        printf("Received unknown Internal ACP command of type 0x%x\n", cmdType);
    }
}

inline void DispatchACPCommandOld(ACP_COMMAND_TYPE_INTERNAL cmdType, AcpState_Old *acpState, AcpCommand_Old Cmd)
{
    if (cmdType == INTERNAL_ACP_COMMAND_TYPE_CONNECT)
    {
        g_LoganHeap._acpConnectStorageOld[Cmd.connect.instance - 1] = Cmd;
        g_LoganHeap._acpConnectCommandOld[Cmd.connect.instance - 1] = &g_LoganHeap._acpConnectStorageOld[Cmd.connect.instance - 1];
        printf("Registered main ACP command and message queues.\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_DISCONNECT)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_DISCONNECT)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_REGISTER_CONTEXT_ARRAYS)
    {
        g_LoganHeap._acpContextArrays = Cmd.registerContextArrays;
        printf("Registered ACP context arrays.\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_INIT)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_INIT)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_ENABLE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_ENABLE)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_DISABLE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_EVENT_LOG_DISABLE)\n");
    }
    else if (cmdType == INTERNAL_ACP_COMMAND_TYPE_TERMINATE)
    {
        printf("Received Internal ACP command (INTERNAL_ACP_COMMAND_TYPE_TERMINATE)\n");
    }
    else
    {
        printf("Received unknown Internal ACP command of type 0x%x\n", cmdType);
    }
}

// The console WinDurango allocates also receives the WinRT logger's output, which buries anything
// printed here within seconds. ACP tracing goes to a file instead, appended and flushed per line
// so it survives a hard exit.
void AcpTrace(char const *format, ...)
{
    // Writing the trace opens a file, and CreateFileW is detoured - so tracing a successful open
    // re-enters here and recurses until the stack is gone. The guard is per-thread because several
    // threads trace concurrently and a shared flag would drop their lines.
    thread_local bool tracing = false;
    if (tracing)
        return;

    struct Guard
    {
        bool &flag;
        Guard(bool &f) : flag(f) { flag = true; }
        ~Guard() { flag = false; }
    } guard(tracing);

    char body[512];
    va_list args;
    va_start(args, format);
    vsprintf_s(body, format, args);
    va_end(args);

    // The handle is opened once and kept. This used to open, write, flush and close per line, and
    // since a line is written on every file the title opens, the flush - a physical write to the
    // same disk the title streams from - ran inside its CreateFileW, on its loading thread. Dropping
    // it costs nothing in practice: an append that has reached the file system cache survives the
    // process dying, which is what the tracing has to survive.
    static SRWLOCK openLock = SRWLOCK_INIT;
    static HANDLE file = INVALID_HANDLE_VALUE;

    if (file == INVALID_HANDLE_VALUE)
    {
        AcquireSRWLockExclusive(&openLock);
        if (file == INVALID_HANDLE_VALUE)
        {
            wchar_t path[MAX_PATH];
            if (GetTempPathW(MAX_PATH, path))
            {
                wcscat_s(path, L"windurango_acp.log");
                file = CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
                                   FILE_ATTRIBUTE_NORMAL, nullptr);
            }
        }
        ReleaseSRWLockExclusive(&openLock);
    }

    if (file == INVALID_HANDLE_VALUE)
        return;

    SYSTEMTIME now;
    GetLocalTime(&now);

    char line[640];
    int length = sprintf_s(line, "%02u:%02u:%02u.%03u %s\n", now.wHour, now.wMinute, now.wSecond, now.wMilliseconds,
                           body);

    // A single WriteFile on a FILE_APPEND_DATA handle is atomic, so concurrent tracing threads
    // cannot interleave halves of a line.
    DWORD written = 0;
    WriteFile(file, line, static_cast<DWORD>(length), &written, nullptr);
}

template <typename T>
inline void DispatchClientACPCommand(ACP_COMMAND_TYPE cmdType, AcpState* acpState, T Cmd)
{
    if (cmdType == ACP_COMMAND_TYPE_REGISTER_MESSAGE)
    {
        //Enables one or more message types. Uses the same struct as ACP_COMMAND_TYPE_UNREGISTER_MESSAGE
        g_LoganHeap._enabledMessages |= Cmd.registerMessage.message;

        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_COMMAND_COMPLETED)
        {
            printf("Enabled command completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_ERROR)
        {
            printf("Enabled error message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_AUDIO_FRAME_START)
        {
            printf("Enabled frame start message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DISCONNECTED)
        {
            printf("Enabled disconnected message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
        {
            printf("Enabled flowgraph completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_SRC_BLOCKED)
        {
            printf("Enabled Sample Rate Converter blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DMA_BLOCKED)
        {
            printf("Enabled DMA blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_TERMINATED)
        {
            printf("Enabled flowgraph terminated message type.\n");
        }
    }
    else if (cmdType == ACP_COMMAND_TYPE_UNREGISTER_MESSAGE)
    {
        //Disables one or more message types. Uses the same struct as ACP_COMMAND_TYPE_REGISTER_MESSAGE
        g_LoganHeap._enabledMessages &= ~Cmd.registerMessage.message;

        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_COMMAND_COMPLETED)
        {
            printf("Disabled command completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_ERROR)
        {
            printf("Disabled error message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_AUDIO_FRAME_START)
        {
            printf("Disabled frame start message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DISCONNECTED)
        {
            printf("Disabled disconnected message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
        {
            printf("Disabled flowgraph completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_SRC_BLOCKED)
        {
            printf("Disabled Sample Rate Converter blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DMA_BLOCKED)
        {
            printf("Disabled DMA blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_TERMINATED)
        {
            printf("Disabled flowgraph terminated message type.\n");
        }
    }
    else if (cmdType == ACP_COMMAND_TYPE_DISABLE_XMA_CONTEXT)
    {
        ACP_COMMAND_ENABLE_OR_DISABLE_XMA_CONTEXT Command = Cmd.enableOrDisableXmaContext;
        g_LoganHeap._disabledXmaContexts[Command.contextIndex] = !g_LoganHeap._disabledXmaContexts[Command.contextIndex];
        printf("Disabled Xma Context at index %u.\n", Command.contextIndex);
    }
    else if (cmdType == ACP_COMMAND_TYPE_UPDATE_XMA_CONTEXT)
    {
        g_LoganHeap.UpdateXmaContext(Cmd.updateXmaContext);
    }
    else if (cmdType == ACP_COMMAND_TYPE_DISABLE_XMA_CONTEXTS)
    {
        ACP_COMMAND_ENABLE_OR_DISABLE_XMA_CONTEXTS Command = Cmd.enableOrDisableXmaContexts;
        printf("TODO: Disable Xma Contexts.\n");
    }
    else if (cmdType == ACP_COMMAND_TYPE_UPDATE_XMA_READ_BUFFER)
    {
        g_LoganHeap.UpdateXmaReadBuffer(Cmd.updateXmaReadBuffer);
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_XMA_WRITE_BUFFER_OFFSET_READ)
    {
        // The client saying how much it has taken out of a decoder's output ring. This layer does
        // not use that ring - decoded samples go straight from the codec into the flowgraph - so
        // there is nothing to move, and the command is accepted rather than logged as unknown.
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_DMA_READ_POINTER)
    {
        g_LoganHeap.IncrementDmaPointer(Cmd.incrementDmaPointer.contextIndex,
                                        Cmd.incrementDmaPointer.increment, false);
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_DMA_WRITE_POINTER)
    {
        g_LoganHeap.IncrementDmaPointer(Cmd.incrementDmaPointer.contextIndex,
                                        Cmd.incrementDmaPointer.increment, true);
    }
    else if (cmdType == ACP_COMMAND_TYPE_LOAD_SHAPE_FLOWGRAPH)
    {
        ACP_COMMAND_LOAD_SHAPE_FLOWGRAPH Command = Cmd.loadFlowgraph;
        g_LoganHeap._flowgraph = Command.flowgraph;
        g_LoganHeap._numCommandsInFlowgraph = Command.numCommands;
        g_LoganHeap.RecordFlowgraph(Command);
        //printf("Loaded SHAPE Flowgraph with %u commands.\n", Command.numCommands);
    }
    else
    {
        printf("Received unknown Client ACP command of type 0x%x\n", cmdType);
    }
}

template <typename T> 
inline void DispatchClientACPCommandOld(ACP_COMMAND_TYPE cmdType, AcpState_Old *acpState, T Cmd)
{
    if (cmdType == ACP_COMMAND_TYPE_REGISTER_MESSAGE)
    {
        //Enables one or more message types. Uses the same struct as ACP_COMMAND_TYPE_UNREGISTER_MESSAGE
        g_LoganHeap._enabledMessages |= Cmd.registerMessage.message;

        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_COMMAND_COMPLETED)
        {
            printf("Enabled command completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_ERROR)
        {
            printf("Enabled error message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_AUDIO_FRAME_START)
        {
            printf("Enabled frame start message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DISCONNECTED)
        {
            printf("Enabled disconnected message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
        {
            printf("Enabled flowgraph completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_SRC_BLOCKED)
        {
            printf("Enabled Sample Rate Converter blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DMA_BLOCKED)
        {
            printf("Enabled DMA blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_TERMINATED)
        {
            printf("Enabled flowgraph terminated message type.\n");
        }
    }
    else if (cmdType == ACP_COMMAND_TYPE_UNREGISTER_MESSAGE)
    {
        //Disables one or more message types. Uses the same struct as ACP_COMMAND_TYPE_REGISTER_MESSAGE
        g_LoganHeap._enabledMessages &= ~Cmd.registerMessage.message;

        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_COMMAND_COMPLETED)
        {
            printf("Disabled command completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_ERROR)
        {
            printf("Disabled error message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_AUDIO_FRAME_START)
        {
            printf("Disabled frame start message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DISCONNECTED)
        {
            printf("Disabled disconnected message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_COMPLETED)
        {
            printf("Disabled flowgraph completed message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_SRC_BLOCKED)
        {
            printf("Disabled Sample Rate Converter blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_DMA_BLOCKED)
        {
            printf("Disabled DMA blocked message type.\n");
        }
        if (Cmd.registerMessage.message & ACP_MESSAGE_TYPE_FLOWGRAPH_TERMINATED)
        {
            printf("Disabled flowgraph terminated message type.\n");
        }
    }
    else if (cmdType == ACP_COMMAND_TYPE_DISABLE_XMA_CONTEXT)
    {
        ACP_COMMAND_ENABLE_OR_DISABLE_XMA_CONTEXT Command = Cmd.enableOrDisableXmaContext;
        g_LoganHeap._disabledXmaContexts[Command.contextIndex] = !g_LoganHeap._disabledXmaContexts[Command.contextIndex];
        printf("Disabled Xma Context at index %u.\n", Command.contextIndex);
    }
    else if (cmdType == ACP_COMMAND_TYPE_UPDATE_XMA_CONTEXT)
    {
        g_LoganHeap.UpdateXmaContext(Cmd.updateXmaContext);
    }
    else if (cmdType == ACP_COMMAND_TYPE_DISABLE_XMA_CONTEXTS)
    {
        ACP_COMMAND_ENABLE_OR_DISABLE_XMA_CONTEXTS Command = Cmd.enableOrDisableXmaContexts;
        printf("TODO: Disable Xma Contexts.\n");
    }
    else if (cmdType == ACP_COMMAND_TYPE_UPDATE_XMA_READ_BUFFER)
    {
        g_LoganHeap.UpdateXmaReadBuffer(Cmd.updateXmaReadBuffer);
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_XMA_WRITE_BUFFER_OFFSET_READ)
    {
        // The client saying how much it has taken out of a decoder's output ring. This layer does
        // not use that ring - decoded samples go straight from the codec into the flowgraph - so
        // there is nothing to move, and the command is accepted rather than logged as unknown.
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_DMA_READ_POINTER)
    {
        g_LoganHeap.IncrementDmaPointer(Cmd.incrementDmaPointer.contextIndex,
                                        Cmd.incrementDmaPointer.increment, false);
    }
    else if (cmdType == ACP_COMMAND_TYPE_INCREMENT_DMA_WRITE_POINTER)
    {
        g_LoganHeap.IncrementDmaPointer(Cmd.incrementDmaPointer.contextIndex,
                                        Cmd.incrementDmaPointer.increment, true);
    }
    else if (cmdType == ACP_COMMAND_TYPE_LOAD_SHAPE_FLOWGRAPH)
    {
        ACP_COMMAND_LOAD_SHAPE_FLOWGRAPH Command = Cmd.loadFlowgraph;
        g_LoganHeap._flowgraph = Command.flowgraph;
        g_LoganHeap._numCommandsInFlowgraph = Command.numCommands;
        g_LoganHeap.RecordFlowgraph(Command);
        //printf("Loaded SHAPE Flowgraph with %u commands.\n", Command.numCommands);
    }
    else
    {
        printf("Received unknown Client ACP command of type 0x%x\n", cmdType);
    }
}

template <typename T> 
inline void DispatchLoganCommand(LOGAN_COMMAND_TYPE cmdType, T cmd)
{
    if (cmdType == LOGAN_COMMAND_TYPE_ACP_INIT)
    {
        printf("Received Logan command (LOGAN_COMMAND_TYPE_ACP_INIT)\n");

        if (g_ABI <= abi_t{6,2,9894,0})
            InitialCommandOld = reinterpret_cast<LOGAN_COMMAND_ACP_INIT_OLD*>(cmd);
        else
            InitialCommand = reinterpret_cast<LOGAN_COMMAND_ACP_INIT*>(cmd);
    }
    else
    {
        printf("Received unimplemented Logan command: 0x%u.\n", cmdType);
    }
}

void SendMessageFromACP(ACP_MESSAGE *pMessage, AcpMessageQueueEntry *pMessageQueue, AcpState *pAcpState, UINT ClientIndex)
{
    if (!pMessageQueue || !pMessage || !pAcpState)
        return;

    UINT32 const slot = pAcpState->clientMessageQueueWritePointer[ClientIndex];
    if (pMessageQueue[slot].state)
    {
        ++g_LoganHeap._droppedMessages;
        return;
    }

    pMessageQueue[slot].message = *pMessage;
    pMessageQueue[slot].state = 1;

    pAcpState->clientMessageQueueWritePointer[ClientIndex] =
        (slot + 1 < g_LoganHeap._acpConnectCommand[ClientIndex]->connect.numMessages) ? slot + 1 : 0;
}

void SendMessageFromACPOld(ACP_MESSAGE_OLD *pMessage, AcpMessageQueueEntry_Old *pMessageQueue, AcpState *pAcpState, UINT ClientIndex)
{
    if (!pMessageQueue || !pMessage || !pAcpState)
        return;

    UINT32 const slot = pAcpState->clientMessageQueueWritePointer[ClientIndex];
    if (pMessageQueue[slot].state)
    {
        ++g_LoganHeap._droppedMessages;
        return;
    }

    // Body first, then the flag that publishes it. The other order lets the reader see a marked
    // entry whose contents are still the previous message's.
    pMessageQueue[slot].message = *pMessage;
    pMessageQueue[slot].state = 1;

    pAcpState->clientMessageQueueWritePointer[ClientIndex] =
        (slot + 1 < g_LoganHeap._acpConnectCommand[ClientIndex]->connect.numMessages) ? slot + 1 : 0;
}

void SendMessageFromACPOlder(ACP_MESSAGE_OLD *pMessage, AcpMessageQueueEntry_Old *pMessageQueue, AcpState_Old *pAcpState, UINT ClientIndex)
{
    if (!pMessageQueue || !pMessage || !pAcpState)
        return;

    UINT32 const slot = pAcpState->clientMessageQueueWritePointer[ClientIndex];
    if (pMessageQueue[slot].state)
    {
        ++g_LoganHeap._droppedMessages;
        return;
    }

    pMessageQueue[slot].message = *pMessage;
    pMessageQueue[slot].state = 1;

    pAcpState->clientMessageQueueWritePointer[ClientIndex] =
        (slot + 1 < g_LoganHeap._acpConnectCommandOld[ClientIndex]->connect.numMessages) ? slot + 1 : 0;
}

void LoganHeap::RecordFlowgraph(ACP_COMMAND_LOAD_SHAPE_FLOWGRAPH const &command)
{
    SHAPE_FLOWGRAPH_COMMAND *Flowgraph = GetVirtualAddress<SHAPE_FLOWGRAPH_COMMAND>(command.flowgraph, command.numCommands);

    struct MixSource
    {
        UINT32 context;
        UINT32 channel;
        UINT32 numIn;
        UINT32 numOut;
        UINT32 virtualId;
        bool set;
    };
    MixSource mix[8192]{}; //There can be up to 8192 Mix Buffers

    for (UINT32 i = 0; i < command.numCommands; ++i)
    {
        SHAPE_FLOWGRAPH_COMMAND entry = Flowgraph[i];
        if (entry.header.disabled)
            continue;

        if (entry.header.command == SHAPE_FLOWGRAPH_COMMAND_TYPE_SRC)
        {
            UINT64 buffers[2] = {entry.src.mixBuffer0, entry.src.mixBuffer1};
            for (UINT64 channel = 0; channel < 2; ++channel)
            {
                if (buffers[channel] < 8192)
                {
                    mix[buffers[channel]].context = entry.src.contextId;
                    mix[buffers[channel]].channel = channel;
                    mix[buffers[channel]].set = true;
                }
            }
        }
        else if (entry.header.command == SHAPE_FLOWGRAPH_COMMAND_TYPE_DMA)
        {
            UINT32 target = entry.dma.contextId;
            UINT32 buffer = entry.dma.mixBuffer;
            if (target >= 256 || buffer >= 8192 || !mix[buffer].set)
                continue;

            _dmaSource[target].xmaContext = mix[buffer].context;
            _dmaSource[target].channel = mix[buffer].channel;
            _dmaSource[target].set = true;
        }
        else if (entry.header.command == SHAPE_FLOWGRAPH_COMMAND_TYPE_ALLOCMIXBUFFER)
        {
            UINT32 virtualId = entry.allocMixBuffer.virtualId;
            UINT32 numIn = entry.allocMixBuffer.numIn;
            UINT32 numOut = entry.allocMixBuffer.numOut;

            UINT64 buffers[2] = {entry.src.mixBuffer0, entry.src.mixBuffer1};
            for (UINT32 Channel = 0; Channel < 2; ++Channel)
            {
                mix[buffers[Channel]].numIn = numIn;
                mix[buffers[Channel]].numOut = numOut;
                mix[buffers[Channel]].channel = Channel;
                mix[buffers[Channel]].virtualId = virtualId;
                mix[buffers[Channel]].set = true;
                virtualId++;
            }

            for (UINT64 Buffer = 0; Buffer < 2; ++Buffer)
            {
                _dmaSource[Buffer].xmaContext = mix[Buffer].context;
                _dmaSource[Buffer].channel = mix[Buffer].channel;
                _dmaSource[Buffer].set = true;
            }
        }
    }
}

// Prints every context that carries a pointer, whatever unit it belongs to.
//
// Kept separate from the five second census because the watcher below calls it too, and a context
// that lives for one decode is only ever seen by the watcher.
// The client's half of the DMA ring handshake.
//
// The ACP fills a block and moves the write pointer; the client takes blocks out and sends
// INCREMENT_DMA_READ_POINTER to say how many. Nothing here acted on that command, so the read
// pointer stayed at zero, the four block ring reported itself full after four audio frames and the
// ACP never wrote another block - while the client went on consuming the same four blocks over and
// over. That is what a tone written into the buffer sounded like: about eleven milliseconds of it,
// repeating.
void LoganHeap::IncrementDmaPointer(UINT32 contextIndex, UINT32 increment, bool writePointer)
{
    auto *contexts =
        GetVirtualAddress<SHAPE_DMA_CONTEXT>(_acpContextArrays.dmaContextArray, _acpContextArrays.numDmaContexts);
    if (!contexts || contextIndex >= _acpContextArrays.numDmaContexts)
        return;

    SHAPE_DMA_CONTEXT &context = contexts[contextIndex];
    UINT32 const blocks = context.numFrames + 1;
    if (!blocks || !increment)
        return;

    ++_dmaIncrements;
    if (!writePointer && contextIndex < ARRAYSIZE(_dmaTakenPerContext))
        _dmaTakenPerContext[contextIndex] += increment;

    if (writePointer)
    {
        context.writePointer = (context.writePointer + increment) % blocks;
        if (context.writePointer == context.readPointer)
            context.full = 1;
    }
    else
    {
        // A ring that is not full when the client comes for its period is a ring we did not manage
        // to refill in time, and the client reads a block it has already played. That is the only
        // failure this stage can have, and it is audible, so it is counted on its own rather than
        // being inferred from throughput.
        if (!context.full)
            ++_dmaUnderruns;

        context.readPointer = (context.readPointer + increment) % blocks;
        context.full = 0;
    }

    // The room appeared in this call, so fill it in this call. Leaving it to the next pass of the
    // pump adds that pass's latency to every period, and a period is only four blocks long.
    if (!writePointer)
        RunFlowgraph();
}

// Runs the loaded SHAPE program once, which is what the hardware does every audio frame.
//
// The program Forza loads is five commands long and never changes:
//
//     alloc mix buffer 1        alloc mix buffer 2
//     src context 100 -> mix buffer 1, mix buffer 2
//     dma context 200 <- mix buffer 1
//     dma context 201 <- mix buffer 2
//
// The command code for DMA is 16, not the 5 its position in the enum suggests. Both DMA words
// decode to context 200 and 201 with the bit layout in the header, which is what confirms that
// layout: two independent readings landing on the two contexts the title actually programmed.
//
// Only the DMA step is implemented here, and it writes a tone rather than audio. That is
// deliberate and temporary. Everything upstream of the DMA - the XMA decoder and the sample rate
// converter - is the part that needs a real codec, and there is no point building it until the
// path from a DMA buffer to the speakers is known to work. If the tone is audible where the music
// should be, then the flowgraph, the contexts, the ring handshake, FMOD's mixer and WASAPI are all
// correct and only the codec is missing. If it is not audible, this would have been the wrong
// thing to build the codec on top of.
// Credits every routed target with the frames that have gone by.
//
// The cap is per target and generous enough to cover any scheduling jitter, but not so generous
// that a sound nobody is reading builds a backlog worth minutes: a target whose client has stopped
// taking blocks - a paused or finished sound - would otherwise come back with a burst.
void LoganHeap::OweFrames(UINT32 frames)
{
    if (!frames)
        return;

    UINT32 constexpr c_MaxOwed = 8;

    for (UINT32 index = 0; index < ARRAYSIZE(_dmaOwed); ++index)
    {
        if (!_dmaSource[index].set)
            continue;

        _dmaOwed[index] += frames;
        if (_dmaOwed[index] > c_MaxOwed)
            _dmaOwed[index] = c_MaxOwed;
    }
}

void LoganHeap::RunFlowgraph()
{

    auto *dmaContexts =
        GetVirtualAddress<SHAPE_DMA_CONTEXT>(_acpContextArrays.dmaContextArray, _acpContextArrays.numDmaContexts);
    auto *xmaContexts =
        GetVirtualAddress<SHAPE_XMA_CONTEXT>(_acpContextArrays.xmaContextArray, _acpContextArrays.numXmaContexts);
    if (!dmaContexts || !xmaContexts)
        return;

    // One audio frame is 128 samples and the DMA converts to float, so a block is 512 bytes. The
    // 2 KB the title allocates per channel is exactly four of them, and numFrames reads 3 - a ring
    // of four with five bit pointers.
    UINT32 constexpr c_SamplesPerFrame = 128;

    // Every routed target, once per audio frame that is owed.
    //
    // The clock decides how much is due and the ring decides how much of it fits; both limits are
    // needed. Answering each client message with a full ring wrote four hundred milliseconds of
    // audio into a ten millisecond buffer inside one millisecond, and pacing on the ring alone lost
    // seven per cent of the time to scheduling jitter. Pacing on flowgraph submissions - which is
    // what a queue of programs invites - starved every voice whose program arrived infrequently.
    for (UINT32 index = 0; index < _acpContextArrays.numDmaContexts; ++index)
    {
        if (!_dmaSource[index].set)
            continue;

        SHAPE_DMA_CONTEXT &context = dmaContexts[index];
        if (!context.address)
            continue;

        while (_dmaOwed[index])
        {
            // A ring with no room keeps its debt: the client has simply not read yet, and the block
            // is due the moment it does. Writing the frame off here is what lost six per cent of
            // the audio a block at a time.
            if (context.full)
            {
                ++_dmaBlocksSkipped;
                if (index < ARRAYSIZE(_dmaSkippedPerContext))
                    ++_dmaSkippedPerContext[index];
                break;
            }

            UINT32 const blocks = context.numFrames + 1;
            UINT32 const block = context.writePointer;
            if (block >= blocks)
                break;

            auto *samples = GetVirtualAddress<float>(context.address + block * c_SamplesPerFrame * sizeof(float),
                                                     c_SamplesPerFrame);
            if (!samples)
                break;

            UINT32 produced = 0;
            if (_dmaSource[index].xmaContext < _acpContextArrays.numXmaContexts && !g_LoganHeap._disabledXmaContexts[_dmaSource[index].xmaContext])
                produced = ServiceXmaContext(_dmaSource[index].xmaContext, _dmaSource[index].channel, samples, c_SamplesPerFrame);

            // A decoder that came up short has run out of input the title has not refilled yet.
            // Silence is the right filler: repeating the last block is what a stalled ring already
            // sounded like, and that is worse than a gap because it sounds deliberate.
            for (UINT32 s = produced; s < c_SamplesPerFrame; ++s)
                samples[s] = 0.0f;

            // Bend the overshoot down instead of cutting it off. libavcodec does not clamp, and
            // this material decodes to about 1.07 of full scale on its peaks; cutting those flat
            // put a corner in the waveform on every musical peak, which is nothing as a proportion
            // and crackle in practice. Below the knee nothing is touched.
            float constexpr knee = 0.90f;
            for (UINT32 s = 0; s < produced; ++s)
            {
                float const magnitude = samples[s] < 0.0f ? -samples[s] : samples[s];
                if (magnitude > _dmaPeak)
                    _dmaPeak = magnitude;

                if (magnitude <= knee)
                    continue;

                float const over = (magnitude - knee) / (1.0f - knee);
                float const shaped = knee + (1.0f - knee) * tanhf(over);
                samples[s] = samples[s] < 0.0f ? -shaped : shaped;
                ++_dmaClipped;
            }

            _dmaSamplesDecoded += produced;
            _dmaSamplesPadded += c_SamplesPerFrame - produced;

            context.writePointer = (context.writePointer + 1) % blocks;
            if (context.writePointer == context.readPointer)
                context.full = 1;

            ++_dmaBlocksWritten;
            if (index < ARRAYSIZE(_dmaWrittenPerContext))
                ++_dmaWrittenPerContext[index];

            --_dmaOwed[index];
        }
    }

    // Programs are consumed at the rate they arrive; they carry the wiring, which is learned when
    // they are loaded, so nothing here needs them beyond keeping the queue from filling up.
    _flowgraphsRun += _flowgraphQueued;
    _flowgraphHead = _flowgraphTail;
    _flowgraphQueued = 0;
}

// Marks one of a decoder's two input buffers as refilled and ready.
//
// The handover has two halves and only one of them was implemented. The decoder clears a buffer's
// valid bit when it has read it through, and the title answers with this command once it has put
// fresh packets there. Ignoring it left the valid mask at zero after four handovers - about half a
// second of audio, which is exactly what came out of the speakers - while the title went on sending
// the command 187 times a second to a layer that filed it under "unknown".
//
// The addresses are applied only when they look like addresses. sizeRead is a twelve bit field
// counted in packets, and a byte count written into it would truncate to something arbitrary, so a
// size that cannot be a packet count is logged and left alone rather than trusted.
void LoganHeap::UpdateXmaReadBuffer(ACP_COMMAND_UPDATE_XMA_READ_BUFFER const &command)
{
    auto *contexts = GetVirtualAddress<SHAPE_XMA_CONTEXT>(_acpContextArrays.xmaContextArray, _acpContextArrays.numXmaContexts);
    if (!contexts || command.contextIndex >= _acpContextArrays.numXmaContexts)
        return;

    SHAPE_XMA_CONTEXT &context = contexts[command.contextIndex];
    UINT32 const buffer = command.bufferIndex & 1;
    bool const sizeUsable = command.bufferSize > 0 && command.bufferSize < 0x1000;

    if (command.bufferAddress)
    {
        if (buffer)
        {
            context.ptrRead1 = command.bufferAddress;
            if (sizeUsable)
                context.sizeRead1 = command.bufferSize;
        }
        else
        {
            context.ptrRead0 = command.bufferAddress;
            if (sizeUsable)
                context.sizeRead0 = command.bufferSize;
        }
    }

    context.validBuffer = context.validBuffer | (1u << buffer);
}

void LoganHeap::UpdateXmaContext(ACP_COMMAND_UPDATE_XMA_CONTEXT const &command)
{
    auto *contexts = GetVirtualAddress<SHAPE_XMA_CONTEXT>(_acpContextArrays.xmaContextArray, _acpContextArrays.numXmaContexts);
    if (!contexts || command.contextIndex >= _acpContextArrays.numXmaContexts)
        return;

    SHAPE_XMA_CONTEXT &context = contexts[command.contextIndex];
    SHAPE_XMA_CONTEXT *UpdateData = GetVirtualAddress<SHAPE_XMA_CONTEXT>(command.contextData);

    if (command.updateMask & SHAPE_XMA_UPDATE_SIZE_READ0)
        context.sizeRead0 = UpdateData->sizeRead0;
    if (command.updateMask & SHAPE_XMA_UPDATE_SIZE_READ1)
        context.sizeRead1 = UpdateData->sizeRead1;
    if (command.updateMask & SHAPE_XMA_UPDATE_NUM_LOOPS)
        context.numLoops = UpdateData->numLoops;
    if (command.updateMask & SHAPE_XMA_UPDATE_VALID_BUFFER)
        context.validBuffer = UpdateData->validBuffer;
    if (command.updateMask & SHAPE_XMA_UPDATE_SIZE_WRITE)
        context.sizeWrite = UpdateData->sizeWrite;
    if (command.updateMask & SHAPE_XMA_UPDATE_LOOP_SUBFRAME_END)
        context.loopSubframeEnd = UpdateData->loopSubframeEnd;
    if (command.updateMask & SHAPE_XMA_UPDATE_LOOP_SUBFRAME_SKIP)
        context.loopSubframeSkip = UpdateData->loopSubframeSkip;
    if (command.updateMask & SHAPE_XMA_UPDATE_NUM_SUBFRAME_TO_DECODE)
        context.numSubframesToDecode = UpdateData->numSubframesToDecode;
    if (command.updateMask & SHAPE_XMA_UPDATE_NUM_SUBFRAMES_TO_SKIP)
        context.numSubframesToSkip = UpdateData->numSubframesToSkip;
    if (command.updateMask & SHAPE_XMA_UPDATE_SAMPLERATE)
        context.sampleRate = UpdateData->sampleRate;
    if (command.updateMask & SHAPE_XMA_UPDATE_NUM_CHANNELS)
        context.numChannels = UpdateData->numChannels;
    if (command.updateMask & SHAPE_XMA_UPDATE_VALID_WRITE)
        context.validWrite = UpdateData->validWrite;
    if (command.updateMask & SHAPE_XMA_UPDATE_OFFSET_READ)
        context.offsetRead = UpdateData->offsetRead;
    if (command.updateMask & SHAPE_XMA_UPDATE_LOOP_START_OFFSET)
        context.loopStartOffset = UpdateData->loopStartOffset;
    if (command.updateMask & SHAPE_XMA_UPDATE_LOOP_END_OFFSET)
        context.loopEndOffset = UpdateData->loopEndOffset;
    if (command.updateMask & SHAPE_XMA_UPDATE_PACKET_METADATA)
        context.packetMetaData = UpdateData->packetMetaData;
    if (command.updateMask & SHAPE_XMA_UPDATE_CURRENT_BUFFER)
        context.currentBuffer = UpdateData->currentBuffer;
    if (command.updateMask & SHAPE_XMA_UPDATE_PTR_READ0)
        context.ptrRead0 = UpdateData->ptrRead0;
    if (command.updateMask & SHAPE_XMA_UPDATE_PTR_READ1)
        context.ptrRead1 = UpdateData->ptrRead1;
    if (command.updateMask & SHAPE_XMA_UPDATE_PTR_WRITE)
        context.ptrWrite = UpdateData->ptrWrite;
    if (command.updateMask & SHAPE_XMA_UPDATE_PTR_OVERLAPADD)
        context.ptrOverlapAdd = UpdateData->ptrOverlapAdd;
    if (command.updateMask & SHAPE_XMA_UPDATE_WRITE_BUFFER_OFFSET_READ)
        context.writeBufferOffsetRead = UpdateData->writeBufferOffsetRead;

    printf("Updated Xma Context at index %u.\n", command.contextIndex);
}

// Raw 32 bit float, one file per channel, twenty seconds and then it stops.
//
// No header: what matters is the samples, and a reader that has to be told the format cannot be
// confused by one that lies. Files land beside the trace as windurango_pcm0.raw / _pcm1.raw.
void LoganHeap::DumpPcm(UINT32 channel, float const *samples, UINT32 count)
{
    if (channel >= ARRAYSIZE(_pcmDump) || _pcmDumped[channel] >= c_PcmDumpLimit)
        return;

    if (!_pcmDump[channel])
    {
        wchar_t path[MAX_PATH];
        if (!GetTempPathW(MAX_PATH, path))
            return;

        wchar_t name[32];
        swprintf_s(name, L"windurango_pcm%u.raw", channel);
        wcscat_s(path, name);

        _pcmDump[channel] = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                                        FILE_ATTRIBUTE_NORMAL, nullptr);
        if (_pcmDump[channel] == INVALID_HANDLE_VALUE)
        {
            _pcmDump[channel] = nullptr;
            return;
        }
    }

    DWORD written = 0;
    WriteFile(_pcmDump[channel], samples, count * sizeof(float), &written, nullptr);
    _pcmDumped[channel] += count;

    if (_pcmDumped[channel] >= c_PcmDumpLimit)
        AcpTrace("[PCM] channel %u recorded %u samples, stopping", channel, _pcmDumped[channel]);
}

// Pulls one channel out of one decoder context, and keeps the shared context in step with how far
// the decoder has actually read.
//
// The input side is a two buffer handover: the title fills buffer 0 and buffer 1 and sets a bit in
// validBuffer for each, and the decoder clears the bit of a buffer it has finished so the title
// knows it may refill that one. Getting this wrong does not sound wrong, it just stops.
UINT32 LoganHeap::ServiceXmaContext(UINT32 index, UINT32 channel, float *out, UINT32 count)
{
    auto *contexts =
        GetVirtualAddress<SHAPE_XMA_CONTEXT>(_acpContextArrays.xmaContextArray, _acpContextArrays.numXmaContexts);
    if (!contexts || index >= _acpContextArrays.numXmaContexts || index >= ARRAYSIZE(_xmaVoices))
        return 0;

    SHAPE_XMA_CONTEXT &context = contexts[index];

    if (!_xmaVoices[index])
        _xmaVoices[index] = new XmaVoice();

    XmaVoice &voice = *_xmaVoices[index];

    UINT32 const current = context.currentBuffer;
    UINT32 const pointer = current ? context.ptrRead1 : context.ptrRead0;
    UINT32 const packets = current ? context.sizeRead1 : context.sizeRead0;
    bool const bufferValid = ((context.validBuffer >> current) & 1) != 0;

    if (bufferValid && pointer && packets)
    {
        auto *buffer =
            GetVirtualAddress<unsigned char>(pointer, static_cast<SIZE_T>(packets) * XmaVoice::c_PacketBytes);
        if (buffer)
        {
            // offsetRead is where the title expects the hardware to resume. Anything outside this
            // buffer means a fresh one, and a fresh one starts after the first packet's header.
            UINT32 const limit = packets * XmaVoice::c_PacketBits;
            UINT32 const start = (context.offsetRead >= XmaVoice::c_HeaderBits && context.offsetRead < limit)
                                     ? context.offsetRead
                                     : XmaVoice::c_HeaderBits;

            // numSubframesToSkip is in subframes of 128 samples: the encoder delay at the head of
            // the stream, which the title expects the hardware to swallow. Playing it is what the
            // first moment of a sound came out as.
            voice.SetInput(buffer, packets, start, context.numChannels + 1, context.numSubframesToSkip * 128);
        }
    }

    UINT32 const taken = voice.Take(channel, out, count);
    UINT32 const newOffset = voice.BitOffset();

    if (voice.Exhausted() && bufferValid)
    {
        context.validBuffer &= ~(1u << current);
        context.currentBuffer = current ^ 1;
        context.offsetRead = XmaVoice::c_HeaderBits;

        voice.ReleaseInput();
    }
    else
    {
        context.offsetRead = newOffset;
    }

    return taken;
}

BOOL LoganHeap::CheckForBlockedSampleRateConverterContexts(UINT *BlockedIndex)
{
    if (_acpContextArrays.srcContextArray)
    {
        SHAPE_SRC_CONTEXT *SrcContextArray = GetVirtualAddress<SHAPE_SRC_CONTEXT>(_acpContextArrays.srcContextArray, _acpContextArrays.numSrcContexts);
        for (UINT i = 0; i < _acpContextArrays.numSrcContexts; ++i)
        {
            if (!SrcContextArray[i].samplePointer)
            {
                (*BlockedIndex) = i;
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL LoganHeap::CheckForBlockedDMAContexts(UINT *BlockedIndex)
{
    if (_acpContextArrays.dmaContextArray)
    {
        SHAPE_DMA_CONTEXT *DmaContextArray = GetVirtualAddress<SHAPE_DMA_CONTEXT>(_acpContextArrays.dmaContextArray, _acpContextArrays.numDmaContexts);
        for (UINT i = 0; i < _acpContextArrays.numDmaContexts; ++i)
        {
            if (!DmaContextArray[i].address)
            {
                (*BlockedIndex) = i;
                return TRUE;
            }
        }
    }

    return FALSE;
}
