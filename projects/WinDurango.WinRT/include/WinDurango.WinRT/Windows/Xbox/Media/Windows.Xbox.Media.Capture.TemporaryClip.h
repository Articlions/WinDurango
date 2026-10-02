#pragma once
#include "Windows.Xbox.Media.Capture.TemporaryClip.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct TemporaryClip : TemporaryClipT<TemporaryClip>
    {
        TemporaryClip() = default;

        winrt::Windows::Foundation::DateTime DateRecorded();
        winrt::Windows::Foundation::TimeSpan Duration();
        hstring SystemOSPath();
    };
}
