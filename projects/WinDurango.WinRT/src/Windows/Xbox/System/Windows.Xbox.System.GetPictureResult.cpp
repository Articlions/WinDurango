#include "Windows.Xbox.System.GetPictureResult.h"
#include <Windows.h>

namespace winrt::Windows::Xbox::System::implementation
{
    winrt::hresult GetPictureResult::Result()
    {
        return S_OK;
    }
    uint32_t GetPictureResult::RequiredBufferSize()
    {
        return m_Buffer.Capacity();
    }
}
