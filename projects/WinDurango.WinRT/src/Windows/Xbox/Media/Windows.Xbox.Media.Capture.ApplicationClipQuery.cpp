#include "Windows.Xbox.Media.Capture.ApplicationClipQuery.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> ApplicationClipQuery::GetClipsAsync(winrt::Windows::Xbox::System::User owner)
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> ApplicationClipQuery::GetClipsForServiceConfigurationAsync(winrt::Windows::Xbox::System::User owner, hstring serviceConfigId)
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> ApplicationClipQuery::GetNextClipsAsync()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> ApplicationClipQuery::GetClipsForUserAsync(winrt::Windows::Xbox::System::User requester, hstring targetXboxUserId)
    {
        throw hresult_not_implemented();
    }
}
