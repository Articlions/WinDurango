#include "Windows.Xbox.Media.Capture.TemporaryClip.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    winrt::Windows::Foundation::DateTime TemporaryClip::DateRecorded()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::TimeSpan TemporaryClip::Duration()
    {
        throw hresult_not_implemented();
    }
    hstring TemporaryClip::SystemOSPath()
    {
        throw hresult_not_implemented();
    }
}
