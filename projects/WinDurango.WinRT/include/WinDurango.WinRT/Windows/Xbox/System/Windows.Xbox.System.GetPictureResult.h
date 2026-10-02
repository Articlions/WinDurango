#pragma once
#include "Windows.Xbox.System.GetPictureResult.g.h"
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.storage.streams.h>

namespace winrt::Windows::Xbox::System::implementation
{
    struct GetPictureResult : GetPictureResultT<GetPictureResult>
    {
    public:
        GetPictureResult(winrt::Windows::Xbox::System::UserPictureSize UserPictureSize, winrt::Windows::Storage::Streams::IBuffer Buffer) : m_Buffer(Buffer)
        {

        }

        winrt::hresult Result();
        uint32_t RequiredBufferSize();

    private:
        winrt::Windows::Storage::Streams::IBuffer m_Buffer;
    };
}
