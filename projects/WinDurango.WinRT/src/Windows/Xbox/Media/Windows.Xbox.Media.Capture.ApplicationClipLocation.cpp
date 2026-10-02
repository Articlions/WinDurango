#include "Windows.Xbox.Media.Capture.ApplicationClipLocation.h"

namespace winrt::Windows::Xbox::Media::Capture::implementation
{
    winrt::Windows::Foundation::Uri ApplicationClipLocation::Uri()
    {
        throw hresult_not_implemented();
    }
    uint64_t ApplicationClipLocation::FileSize()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Media::Capture::ApplicationClipStreamType ApplicationClipLocation::StreamType()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Xbox::Media::Capture::ApplicationClipStorageType ApplicationClipLocation::StorageType()
    {
        throw hresult_not_implemented();
    }
    winrt::Windows::Foundation::DateTime ApplicationClipLocation::Expiration()
    {
        throw hresult_not_implemented();
    }
}
