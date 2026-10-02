#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipLocation.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipLocation : ApplicationClipLocationT<ApplicationClipLocation>
    {
        ApplicationClipLocation() = default;

        winrt::Windows::Foundation::Uri Uri();
        uint64_t FileSize();
        winrt::Windows::Xbox::Media::Capture::ApplicationClipStreamType StreamType();
        winrt::Windows::Xbox::Media::Capture::ApplicationClipStorageType StorageType();
        winrt::Windows::Foundation::DateTime Expiration();
    };
}
