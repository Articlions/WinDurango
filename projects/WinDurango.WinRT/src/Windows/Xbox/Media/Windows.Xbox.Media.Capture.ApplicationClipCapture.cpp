#include "Windows.Xbox.Media.Capture.ApplicationClipCapture.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    winrt::Windows::Foundation::IAsyncAction ApplicationClipCapture::DisableRecordAsync()
    {
        return {};
    }
    winrt::Windows::Foundation::IAsyncAction ApplicationClipCapture::EnableRecordAsync()
    {
        return {};
    }
    winrt::Windows::Xbox::Media::Capture::ClipContentAttributes ApplicationClipCapture::ClipContentAttributes()
    {
        throw hresult_not_implemented();
    }
    void ApplicationClipCapture::ClipContentAttributes(winrt::Windows::Xbox::Media::Capture::ClipContentAttributes const& value)
    {

    }
    winrt::Windows::Foundation::IAsyncAction ApplicationClipCapture::StartRecordAsync(winrt::Windows::Xbox::System::User owner)
    {
        return {};
    }
    winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::Xbox::Media::Capture::ApplicationClip, winrt::Windows::Xbox::Media::Capture::ApplicationClip> ApplicationClipCapture::StopRecordAsync(winrt::Windows::Xbox::Media::Capture::ApplicationClipInfo clipInfo)
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Windows::Xbox::Media::Capture::ApplicationClip, winrt::Windows::Xbox::Media::Capture::ApplicationClip> ApplicationClipCapture::RecordTimespanAsync(winrt::Windows::Xbox::System::User owner, winrt::Windows::Xbox::Media::Capture::ApplicationClipInfo clipInfo, winrt::Windows::Foundation::DateTime start, winrt::Windows::Foundation::DateTime end)
    {
        throw hresult_not_implemented();
    }
    void ApplicationClipCapture::CancelRecord()
    {

    }
    winrt::event_token ApplicationClipCapture::Failed(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Xbox::Media::Capture::ApplicationClipCapture, winrt::Windows::Xbox::Media::Capture::ApplicationClipCaptureFailedEventArgs> const& handler)
    {
        return {};
    }
    void ApplicationClipCapture::Failed(winrt::event_token const& token) noexcept
    {

    }
    winrt::event_token ApplicationClipCapture::RecordLimitationExceeded(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Xbox::Media::Capture::ApplicationClipCapture, winrt::Windows::Foundation::IInspectable> const& handler)
    {
        return {};
    }
    void ApplicationClipCapture::RecordLimitationExceeded(winrt::event_token const& token) noexcept
    {

    }
}
