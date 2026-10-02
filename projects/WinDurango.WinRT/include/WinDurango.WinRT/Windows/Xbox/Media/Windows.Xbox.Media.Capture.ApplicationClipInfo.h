#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipInfo.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipInfo : ApplicationClipInfoT<ApplicationClipInfo>
    {
        ApplicationClipInfo() = default;

        ApplicationClipInfo(hstring const& greatestMomentId);
        hstring GreatestMomentId();
        void GreatestMomentId(hstring const& value);
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::User> AdditionalUsers();
        void AdditionalUsers(winrt::Windows::Foundation::Collections::IVectorView<winrt::Windows::Xbox::System::User> const& value);
        hstring MultiplayerCorrelationId();
        void MultiplayerCorrelationId(hstring const& value);
        winrt::Windows::Foundation::DateTime ThumbnailTimestamp();
        void ThumbnailTimestamp(winrt::Windows::Foundation::DateTime const& value);
        hstring TitleData();
        void TitleData(hstring const& value);
        winrt::Windows::Xbox::Media::Capture::IApplicationClipBaseEventData EventData();
        void EventData(winrt::Windows::Xbox::Media::Capture::IApplicationClipBaseEventData const& value);
        hstring LocalizedClipName();
        void LocalizedClipName(hstring const& value);
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClipInfo : ApplicationClipInfoT<ApplicationClipInfo, implementation::ApplicationClipInfo>
    {
    };
}
