#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipCapture.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipCapture : ApplicationClipCaptureT<ApplicationClipCapture>
    {
        ApplicationClipCapture() = default;

        static winrt::Windows::Foundation::IAsyncAction DisableRecordAsync();
        static winrt::Windows::Foundation::IAsyncAction EnableRecordAsync();
        static winrt::Windows::Xbox::Media::Capture::ClipContentAttributes ClipContentAttributes();
        static void ClipContentAttributes(winrt::Windows::Xbox::Media::Capture::ClipContentAttributes const& value);
        winrt::Windows::Foundation::IAsyncAction StartRecordAsync(winrt::Windows::Xbox::System::User owner);
        winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::Xbox::Media::Capture::ApplicationClip, winrt::Windows::Xbox::Media::Capture::ApplicationClip> StopRecordAsync(winrt::Windows::Xbox::Media::Capture::ApplicationClipInfo clipInfo);
        winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::Xbox::Media::Capture::ApplicationClip, winrt::Windows::Xbox::Media::Capture::ApplicationClip> RecordTimespanAsync(winrt::Windows::Xbox::System::User owner, winrt::Windows::Xbox::Media::Capture::ApplicationClipInfo clipInfo, winrt::Windows::Foundation::DateTime start, winrt::Windows::Foundation::DateTime end);
        void CancelRecord();
        winrt::event_token Failed(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Xbox::Media::Capture::ApplicationClipCapture, winrt::Windows::Xbox::Media::Capture::ApplicationClipCaptureFailedEventArgs> const& handler);
        void Failed(winrt::event_token const& token) noexcept;
        winrt::event_token RecordLimitationExceeded(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Xbox::Media::Capture::ApplicationClipCapture, winrt::Windows::Foundation::IInspectable> const& handler);
        void RecordLimitationExceeded(winrt::event_token const& token) noexcept;
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClipCapture : ApplicationClipCaptureT<ApplicationClipCapture, implementation::ApplicationClipCapture>
    {
    };
}
