#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipUpdateMetadataInfo.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipUpdateMetadataInfo : ApplicationClipUpdateMetadataInfoT<ApplicationClipUpdateMetadataInfo>
    {
        ApplicationClipUpdateMetadataInfo() = default;

        hstring UserCaption();
        void UserCaption(hstring const& value);
        hstring TitleData();
        void TitleData(hstring const& value);
        uint32_t UpdatedFields();
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ApplicationClipUpdateMetadataInfo : ApplicationClipUpdateMetadataInfoT<ApplicationClipUpdateMetadataInfo, implementation::ApplicationClipUpdateMetadataInfo>
    {
    };
}
