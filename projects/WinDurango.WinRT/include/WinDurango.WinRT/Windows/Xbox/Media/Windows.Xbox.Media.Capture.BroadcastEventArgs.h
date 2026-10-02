#pragma once
#include "Windows.Xbox.Media.Capture.BroadcastEventArgs.g.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    struct BroadcastEventArgs : BroadcastEventArgsT<BroadcastEventArgs>
    {
        BroadcastEventArgs() = default;

        uint64_t State();
        uint64_t IntParam();
        hstring StringParam();
        uint64_t Event();
    };
}
