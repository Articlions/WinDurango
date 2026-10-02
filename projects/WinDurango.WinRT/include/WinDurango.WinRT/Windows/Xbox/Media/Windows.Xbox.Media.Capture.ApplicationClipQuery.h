#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipQuery.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipQuery : ApplicationClipQueryT<ApplicationClipQuery>
    {
        ApplicationClipQuery() = default;

        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> GetClipsAsync(winrt::Windows::Xbox::System::User owner);
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> GetClipsForServiceConfigurationAsync(winrt::Windows::Xbox::System::User owner, hstring serviceConfigId);
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> GetNextClipsAsync();
        winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::Media::Capture::ApplicationClip>> GetClipsForUserAsync(winrt::Windows::Xbox::System::User requester, hstring targetXboxUserId);
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClipQuery : ApplicationClipQueryT<ApplicationClipQuery, implementation::ApplicationClipQuery>
    {
    };
}
