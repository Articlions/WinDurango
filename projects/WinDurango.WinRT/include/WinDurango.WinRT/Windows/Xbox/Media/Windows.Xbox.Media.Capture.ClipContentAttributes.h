#pragma once
#include "Windows.Xbox.Media.Capture.ClipContentAttributes.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ClipContentAttributes : ClipContentAttributesT<ClipContentAttributes>
    {
        ClipContentAttributes() = default;

        winrt::Windows::Xbox::Media::Capture::ClipContents ClipContents();
        void ClipContents(winrt::Windows::Xbox::Media::Capture::ClipContents const& value);
    };
}
namespace winrt::Windows::Xbox::Media::Capture::factory_implementation
{
    struct ClipContentAttributes : ClipContentAttributesT<ClipContentAttributes, implementation::ClipContentAttributes>
    {
    };
}
