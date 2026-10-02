//Credits to RSantila for doing this!
#include "XmaDecoder.h"
#include "xma2defs.h"

#include <string.h>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/error.h>
#include <libavutil/mem.h>
#include <libavutil/samplefmt.h>
}

// Defined in Logan.cpp. Declared by hand rather than by including Logan.h, which defines the whole
// Logan emulation state at namespace scope and would give this translation unit its own copy of it.
void AcpTrace(char const *format, ...);

namespace
{

// Big-endian, because the packet header is the one thing in this structure that is not native
// order. Read as a little-endian dword it gives a frame count of zero and a plausible looking
// offset - the kind of wrong that costs a run to notice.
UINT32 ReadBigEndian32(unsigned char const *at)
{
    return (static_cast<UINT32>(at[0]) << 24) | (static_cast<UINT32>(at[1]) << 16) |
           (static_cast<UINT32>(at[2]) << 8) | static_cast<UINT32>(at[3]);
}

} // namespace

XmaVoice::~XmaVoice()
{
    CloseCodec();
}

bool XmaVoice::OpenCodec(UINT32 channels)
{
    if (_codec && _channels == channels)
        return true;

    CloseCodec();

    AVCodec const *codec = avcodec_find_decoder(AV_CODEC_ID_XMA2);
    if (!codec)
    {
        AcpTrace("[XMA] libavcodec has no xma2 decoder in this build");
        return false;
    }

    _codec = avcodec_alloc_context3(codec);
    if (!_codec)
        return false;

    _codec->sample_rate = 48000;
    _codec->pkt_timebase = AVRational{1, 48000};
    av_channel_layout_default(&_codec->ch_layout, channels);

    UINT32 constexpr c_Xma2WaveFormatExSize = sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX);
    _codec->extradata = static_cast<uint8_t*>(av_mallocz(c_Xma2WaveFormatExSize + AV_INPUT_BUFFER_PADDING_SIZE));
    if (!_codec->extradata)
    {
        CloseCodec();
        return false;
    }

    _codec->extradata_size = c_Xma2WaveFormatExSize;
    _codec->extradata[0] = 1;
    _codec->extradata[1] = 0;

    int const opened = avcodec_open2(_codec, codec, nullptr);
    if (opened < 0)
    {
        char reason[AV_ERROR_MAX_STRING_SIZE] = "";
        av_strerror(opened, reason, sizeof(reason));
        AcpTrace("[XMA] avcodec_open2 failed with %d (%s), %u channels", opened, reason, channels);
        CloseCodec();
        return false;
    }

    _avPacket = av_packet_alloc();
    _avFrame = av_frame_alloc();
    if (!_avPacket || !_avFrame)
    {
        CloseCodec();
        return false;
    }

    _channels = channels;
    AcpTrace("[XMA] decoder opened: %u channels at 48 kHz, packets of %u bytes", channels, c_PacketBytes);
    return true;
}

void XmaVoice::CloseCodec()
{
    if (_avFrame)
        av_frame_free(&_avFrame);
    if (_avPacket)
        av_packet_free(&_avPacket);
    if (_codec)
        avcodec_free_context(&_codec);

    _channels = 0;
}

void XmaVoice::ReleaseInput()
{
    _buffer = nullptr;
    _packets = 0;
    _packet = 0;
    _exhausted = true;
}

void XmaVoice::Reset()
{
    ReleaseInput();

    for (UINT32 c = 0; c < c_MaxChannels; ++c)
    {
        _head[c] = 0;
        _count[c] = 0;
        _skipRemaining[c] = 0;
    }

    _packetsFed = 0;
    _primed = false;
}

void XmaVoice::SetInput(unsigned char const *buffer, UINT32 packetCount, UINT32 startBit, UINT32 channels,
                        UINT32 skipAtStart)
{
    if (!OpenCodec(channels < 1 ? 1 : (channels > c_MaxChannels ? c_MaxChannels : channels)))
        return;

    // Only at the head of a stream. Applying it at every buffer handover would punch a hole in the
    // audio every sixty milliseconds.
    if (_packetsFed == 0)
    {
        for (UINT32 ch = 0; ch < c_MaxChannels; ++ch)
            _skipRemaining[ch] = skipAtStart;
    }

    // Still working through the buffer it was given.
    if (_buffer == buffer && !_exhausted)
        return;

    _buffer = buffer;
    _packets = packetCount;
    _packet = startBit / c_PacketBits;
    _exhausted = (buffer == nullptr || packetCount == 0 || _packet >= packetCount);
}

UINT32 XmaVoice::BitOffset() const
{
    if (!_buffer || _packet >= _packets)
        return c_HeaderBits;

    UINT32 const header = ReadBigEndian32(_buffer + static_cast<SIZE_T>(_packet) * c_PacketBytes);
    return _packet * c_PacketBits + c_HeaderBits + ((header >> 11) & 0x7FFF);
}

void XmaVoice::Push(float const *samples, UINT32 count, UINT32 channel)
{
    if (channel >= c_MaxChannels)
        return;

    for (UINT32 i = 0; i < count; ++i)
    {
        // The encoder's own priming samples, thrown away before anything is queued so nothing
        // downstream has to know about them.
        if (_skipRemaining[channel])
        {
            --_skipRemaining[channel];
            continue;
        }

        if (_count[channel] >= c_QueueSamples)
            break;

        UINT32 const slot = (_head[channel] + _count[channel]) % c_QueueSamples;
        _queue[channel][slot] = samples[i];
        ++_count[channel];
    }
}

void XmaVoice::DrainDecoder()
{
    for (;;)
    {
        int const got = avcodec_receive_frame(_codec, _avFrame);
        if (got < 0)
            break;

        ++_framesDecoded;

        UINT32 const produced = static_cast<UINT32>(_avFrame->nb_samples);
        UINT32 const channels = static_cast<UINT32>(_avFrame->ch_layout.nb_channels);

        // The xma2 decoder hands back planar float, one plane per channel. A voice with one channel
        // feeds both queues from the same plane so a stereo flowgraph on a mono sound is centred
        // rather than silent on one side.
        for (UINT32 c = 0; c < c_MaxChannels; ++c)
        {
            UINT32 const plane = (c < channels) ? c : 0;
            if (plane >= channels || !_avFrame->extended_data[plane])
                continue;

            Push(reinterpret_cast<float const *>(_avFrame->extended_data[plane]), produced, c);
        }

        av_frame_unref(_avFrame);
    }
}

bool XmaVoice::FeedNextPacket()
{
    if (!_codec || !_buffer || _exhausted || _packet >= _packets)
    {
        _exhausted = true;
        return false;
    }

    unsigned char const *const packet = _buffer + static_cast<SIZE_T>(_packet) * c_PacketBytes;
    UINT32 const header = ReadBigEndian32(packet);
    UINT32 const skip = header & 0xFF;

    _avPacket->data = const_cast<unsigned char *>(packet);
    _avPacket->size = static_cast<int>(c_PacketBytes);

    int const sent = avcodec_send_packet(_codec, _avPacket);
    if (sent < 0)
    {
        // A decoder that will not take a packet is either full - which the drain below clears - or
        // has been handed something it cannot parse. The second case is worth a line, because it is
        // the difference between a stream this build cannot play and a walk that has gone wrong.
        if (sent != AVERROR(EAGAIN))
        {
            ++_decodeErrors;
            _lastError = static_cast<UINT32>(-sent);

            if (_decodeErrors <= 4 || (_decodeErrors % 500) == 0)
            {
                char reason[AV_ERROR_MAX_STRING_SIZE] = "";
                av_strerror(sent, reason, sizeof(reason));
                AcpTrace("[XMA] packet %u of %u rejected with %d (%s), %u errors so far", _packet, _packets,
                         sent, reason, _decodeErrors);
            }
        }

        DrainDecoder();
        return false;
    }

    // The same packet twice, while the decoder is still priming.
    //
    // A freshly opened decoder returns nothing for its first few packets, so the queue stays empty
    // and the caller keeps asking - long enough to read both input buffers dry and come back to the
    // first before the title has put anything new in it. Those packets then decode a second time
    // and the sound opens with a stutter. Comparing against a reference decode of the same packets
    // put every difference between the two inside the first 0.83 seconds and none after it.
    //
    // Only while priming: a looping sound legitimately comes round to the same packets again, and
    // dropping those would break the loop.
    if (_packetsFed < ARRAYSIZE(_recentPackets))
    {
        UINT64 fingerprint = 1469598103934665603ull;
        for (UINT32 i = 0; i < c_PacketBytes; i += 8)
            fingerprint = (fingerprint ^ *reinterpret_cast<UINT64 const *>(packet + i)) * 1099511628211ull;

        for (UINT32 i = 0; i < _packetsFed; ++i)
        {
            if (_recentPackets[i] != fingerprint)
                continue;

            AcpTrace("[XMA] packet %u repeats one already decoded while priming - skipped", _packet);
            _packet += 1 + skip;
            if (_packet >= _packets)
                _exhausted = true;
            return true;
        }

        _recentPackets[_packetsFed] = fingerprint;
    }

    //DumpPacket(packet);
    ++_packetsFed;
    _packet += 1 + skip;
    if (_packet >= _packets)
        _exhausted = true;

    DrainDecoder();
    return true;
}

void XmaVoice::DumpPacket(unsigned char const *packet)
{
    if (_packetsDumped >= c_PacketDumpLimit)
        return;

    if (!_packetDump)
    {
        wchar_t path[MAX_PATH];
        if (!GetTempPathW(MAX_PATH, path))
            return;
        wcscat_s(path, L"windurango_xma.raw");

        _packetDump = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                                  FILE_ATTRIBUTE_NORMAL, nullptr);
        if (_packetDump == INVALID_HANDLE_VALUE)
        {
            _packetDump = nullptr;
            return;
        }
    }

    DWORD written = 0;
    WriteFile(_packetDump, packet, c_PacketBytes, &written, nullptr);
    ++_packetsDumped;
}

UINT32 XmaVoice::Take(UINT32 channel, float *out, UINT32 count)
{
    if (channel >= c_MaxChannels)
        channel = 0;

    // While filling the lead, ask for the lead rather than the block.
    UINT32 const wanted = _primed ? count : c_PrimeSamples;

    while (_count[channel] < wanted)
    {
        if (!FeedNextPacket())
            break;
    }

    if (!_primed)
    {
        if (_count[channel] < c_PrimeSamples)
            return 0;
        _primed = true;
    }

    UINT32 const taken = _count[channel] < count ? _count[channel] : count;

    for (UINT32 i = 0; i < taken; ++i)
        out[i] = _queue[channel][(_head[channel] + i) % c_QueueSamples];

    _head[channel] = (_head[channel] + taken) % c_QueueSamples;
    _count[channel] -= taken;

    return taken;
}
