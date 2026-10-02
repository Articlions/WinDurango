#pragma once
#include "Windows.Xbox.Chat.ChatCaptureSource.g.h"
#include "Windows.Xbox.Chat.ChatRenderTarget.g.h"
#include "Windows.Xbox.Chat.ChatSessionState.g.h"
#include "Windows.Xbox.Chat.ChatSession.g.h"
#include <winrt/Windows.Foundation.h>

namespace winrt::Windows::Xbox::Chat::implementation
{
    struct ChatCaptureSource : ChatCaptureSourceT<ChatCaptureSource>
    {
        ChatCaptureSource() = default;

        hstring Id();
        winrt::Windows::Xbox::Chat::Format Format();
        void GetNextBuffer(winrt::Windows::Storage::Streams::IBuffer& buffer, winrt::Windows::Xbox::Chat::CaptureBufferStatus& status);
        void GetNextBufferWithCorrelationInfo(winrt::Windows::Storage::Streams::IBuffer& buffer, winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Chat::ChatUserCorrelation>& info, winrt::Windows::Xbox::Chat::CaptureBufferStatus& status);
        winrt::Windows::Xbox::Chat::ChatRestriction GetRelationship(hstring const& targetId);
    };

    struct ChatRenderTarget : ChatRenderTargetT<ChatRenderTarget>
    {
        ChatRenderTarget() = default;

        hstring Id();
        winrt::Windows::Xbox::Chat::ChatRestriction GetRelationship(hstring const& source);
        void BeginMix();
        winrt::Windows::Xbox::Chat::ChatRestriction AddMixBuffer(hstring const& source, winrt::Windows::Xbox::Chat::Format const& format, winrt::Windows::Storage::Streams::IBuffer const& buffer);
        void ResetMix();
        void SubmitMix();
    };

    struct ChatSessionState : ChatSessionStateT<ChatSessionState>
    {
        ChatSessionState() = default;

        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Chat::ChatCaptureSource> CaptureSources();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Chat::ChatRenderTarget> RenderTargets();
    };

    struct ChatSession : ChatSessionT<ChatSession>
    {
        ChatSession() = default;

        ChatSession(winrt::Windows::Foundation::TimeSpan const& time, winrt::Windows::Xbox::Chat::ChatFeatures const& enabledFeatures);
        ChatSession(winrt::Windows::Foundation::TimeSpan const& time, uint32_t processorAffinity, winrt::Windows::Xbox::Chat::ChatFeatures const& enabledFeatures);
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Xbox::Chat::ChatChannel> Channels();
        winrt::event_token StateChangedEvent(winrt::Windows::Xbox::Chat::ChatSessionStateChangedHandler const& handler);
        void StateChangedEvent(winrt::event_token const& token) noexcept;
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Xbox::Chat::ChatSessionState> GetStateAsync();
        int32_t TranscriptionEnabled();
        void TranscriptionEnabled(int32_t value);
        winrt::Windows::Xbox::Chat::ChatEncoder CreateTranscribingEncoder(winrt::Windows::Xbox::Chat::ChatParticipant const& participant, winrt::Windows::Xbox::Chat::Format const& format, winrt::Windows::Xbox::Chat::EncodingQuality const& quality);
        winrt::event_token TranscribedTextReadyEvent(winrt::Windows::Xbox::Chat::ChatParticipantTranscribedTextReadyHandler const& handler);
        void TranscribedTextReadyEvent(winrt::event_token const& token) noexcept;
        winrt::Windows::Xbox::Chat::ChatRestriction GetParticipantRelationship(winrt::Windows::Xbox::Chat::ChatParticipant const& source, winrt::Windows::Xbox::Chat::ChatParticipant const& target);
        winrt::Windows::Xbox::System::AudioDeviceInfo GetDedicatedSynthesisDevice(winrt::Windows::Xbox::Chat::ChatParticipant const& participant);
    private:
        winrt::event<winrt::Windows::Xbox::Chat::ChatSessionStateChangedHandler> e_StateChangedEvent;
        winrt::event<winrt::Windows::Xbox::Chat::ChatParticipantTranscribedTextReadyHandler> e_TranscribedTextReadyEvent;
        int32_t transcriptionEnabled;
    };
}
namespace winrt::Windows::Xbox::Chat::factory_implementation
{
    struct ChatSession : ChatSessionT<ChatSession, implementation::ChatSession>
    {
    };
}
