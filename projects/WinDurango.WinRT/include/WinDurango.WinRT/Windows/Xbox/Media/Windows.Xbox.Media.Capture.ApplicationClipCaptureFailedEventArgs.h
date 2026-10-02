#pragma once
#include "Windows.Xbox.Media.Capture.ApplicationClipCaptureFailedEventArgs.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct ApplicationClipCaptureFailedEventArgs : ApplicationClipCaptureFailedEventArgsT<ApplicationClipCaptureFailedEventArgs>
    {
        ApplicationClipCaptureFailedEventArgs() = default;

        uint32_t Code();
        hstring Message();
    };
}
