#include "ID3D11DMAEngineContext.h"

//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11DMAEngineContextX<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> ULONG D3D11DMAEngineContextX<ABI>::AddRef()
{
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11DMAEngineContextX<ABI>::Release()
{
    m_pImmediateContext->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        m_pImmediateContext->RemoveBackgroundContext(this);
        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid,
                                                                     xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11DMAEngineContextX
//
template <abi_t ABI> D3D11_DEVICE_CONTEXT_TYPE D3D11DMAEngineContextX<ABI>::GetType()
{
    return (D3D11_DEVICE_CONTEXT_TYPE)0x2;
}

template <abi_t ABI>
void D3D11DMAEngineContextX<ABI>::CopyResource(gfx::ID3D11Resource<ABI> *pDstResource,
                                               gfx::ID3D11Resource<ABI> *pSrcResource, uint32_t v3)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::CopySubresourceRegion;
    Command.CopyResource.pDstResource = pDstResource;
    Command.CopyResource.pSrcResource = pSrcResource;
    Command.CopyResource.Flags = v3;
    m_DMACommandQueue.push_back(Command);
}

template <abi_t ABI>
void D3D11DMAEngineContextX<ABI>::CopySubresourceRegion(gfx::ID3D11Resource<ABI> *pDstResource, uint32_t DstSubresource,
                                                        uint32_t DstX, uint32_t DstY, uint32_t DstZ,
                                                        gfx::ID3D11Resource<ABI> *pSrcResource, uint32_t SrcSubresource,
                                                        D3D11_BOX const *pSrcBox, uint32_t v9)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::CopySubresourceRegion;
    Command.CopySubresourceRegion.pDstResource = pDstResource;
    Command.CopySubresourceRegion.DstSubresource = DstSubresource;
    Command.CopySubresourceRegion.DstX = DstX;
    Command.CopySubresourceRegion.DstY = DstY;
    Command.CopySubresourceRegion.DstZ = DstZ;
    Command.CopySubresourceRegion.pSrcResource = pSrcResource;
    Command.CopySubresourceRegion.SrcSubresource = SrcSubresource;
    Command.CopySubresourceRegion.pSrcBox = pSrcBox;
    Command.CopySubresourceRegion.Flags = v9;
    m_DMACommandQueue.push_back(Command);
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::LZDecompressBuffer(gfx::ID3D11Buffer<ABI> *v1, uint32_t v2,
                                                        gfx::ID3D11Buffer<ABI> *v3, uint32_t v4, uint32_t v5,
                                                        uint32_t v6)
{
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::LZDecompressTexture(gfx::ID3D11Resource<ABI> *v1, uint32_t v2, uint32_t v3,
                                                         uint32_t v4, uint32_t v5, gfx::ID3D11Buffer<ABI> *v6,
                                                         uint32_t v7)
{
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::LZCompressBuffer(gfx::ID3D11Buffer<ABI> *v1, uint32_t v2,
                                                      gfx::ID3D11Buffer<ABI> *v3, uint32_t v4, uint32_t v5, uint32_t v6)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::LZCompressTexture(gfx::ID3D11Buffer<ABI> *v1, gfx::ID3D11Resource<ABI> *v2,
                                                       uint32_t v3, D3D11_BOX const *v4, uint32_t v5)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::JPEGDecode(gfx::ID3D11Resource<ABI> *v1, uint32_t v2, uint32_t v3, uint32_t v4,
                                                uint32_t v5, gfx::ID3D11Buffer<ABI> *v6, uint32_t v7)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> uint64_t D3D11DMAEngineContextX<ABI>::InsertFence(uint32_t v1)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::DMAInsertFence; // Signal fence command
    Command.DMAInsertFence.Flags = v1;

    if (DMAFenceIndex >= 1024)
    {
        DMAFenceIndex = 0;
    }

    UINT FenceIndex = DMAFenceIndex;
    DMAFences[FenceIndex] = TRUE;
    DMAFenceIndex++;
    Command.DMAInsertFence.Fence = (UINT64)&DMAFences[FenceIndex];

    m_DMACommandQueue.push_back(Command);

    return (UINT64)&DMAFences[FenceIndex];
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::InsertWaitOnFence(uint32_t v1, uint64_t v2)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::DMAInsertWaitOnFence;
    Command.DMAInsertWaitOnFence.Flags = v1;
    Command.DMAInsertWaitOnFence.Fence = v2;
    m_DMACommandQueue.push_back(Command);
}

template <abi_t ABI> HRESULT D3D11DMAEngineContextX<ABI>::Submit()
{
    m_pImmediateContext->AddBackgroundContext(this);
    return S_OK;
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::CopyLastErrorCodeToMemory(void *v1)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::CopyLastErrorCodeToMemory;
    Command.CopyLastErrorCodeToMemory.pDstAddress = v1;
    m_DMACommandQueue.push_back(Command);
}

template <abi_t ABI>
void D3D11DMAEngineContextX<ABI>::CopyLastErrorCodeToBuffer(gfx::ID3D11Buffer<ABI> *v1, uint32_t v2)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::CopyMemoryToMemory(void *v1, void *v2, uint64_t v3)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::FillMemoryWithValue(void *v1, uint64_t v2, uint32_t v3)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::FillMemoryWithValue;
    Command.FillMemoryWithValue.pDstAddress = v1;
    Command.FillMemoryWithValue.SizeBytes = v2;
    Command.FillMemoryWithValue.FillValue = v3;
    m_DMACommandQueue.push_back(Command);
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::FillResourceWithValue(gfx::ID3D11Resource<ABI> *v1, uint32_t v2)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI>
HRESULT D3D11DMAEngineContextX<ABI>::LZDecompressMemory(void *v1, void *v2, uint32_t v3, uint32_t v4)
{
    DMACommands<ABI> Command{};
    Command.m_DMACommandType = DMACommands<ABI>::DMACommandType::LZDecompressMemory;
    Command.LZDecompressMemory.NextIn = v1;
    Command.LZDecompressMemory.NextOut = v2;
    Command.LZDecompressMemory.AvailIn = v3;
    Command.LZDecompressMemory.Flags = v4;
    m_DMACommandQueue.push_back(Command);
    return S_OK;
}

template <abi_t ABI> HRESULT D3D11DMAEngineContextX<ABI>::LZCompressMemory(void *v1, void *v2, uint32_t v3, uint32_t v4)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::WriteTimestampToMemory(void *v1)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::WriteTimestampToBuffer(gfx::ID3D11Buffer<ABI> *v1, uint32_t v2)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> void D3D11DMAEngineContextX<ABI>::WriteValueBottomOfPipe(void *v1, uint32_t v2)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI>
void D3D11DMAEngineContextX<ABI>::InsertWaitOnMemory(void const *v1, uint32_t v2, D3D11_COMPARISON_FUNC v3, uint32_t v4,
                                                     uint32_t v5)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> BOOL D3D11DMAEngineContextX<ABI>::ExecuteContext()
{
    while (m_CommandIndex < m_DMACommandQueue.size())
    {
        auto &Cmd = m_DMACommandQueue[m_CommandIndex];
        BOOL Wait = TRUE;

        switch (Cmd.m_DMACommandType)
        {
        case DMACommands<ABI>::DMACommandType::LZDecompressMemory:
            Wait = Cmd.LZDecompressMemory.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::DMAInsertFence:
            Wait = Cmd.DMAInsertFence.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::DMAInsertWaitOnFence:
            Wait = Cmd.DMAInsertWaitOnFence.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::CopyResource:
            Wait = Cmd.CopyResource.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::CopySubresourceRegion:
            Wait = Cmd.CopySubresourceRegion.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::FillMemoryWithValue:
            Wait = Cmd.FillMemoryWithValue.Execute(this);
            break;
        case DMACommands<ABI>::DMACommandType::CopyLastErrorCodeToMemory:
            Wait = Cmd.CopyLastErrorCodeToMemory.Execute(this);
            break;
        }

        // If the command told us to wait, return 'FALSE' to
        // indicate that we still have more commands to execute.
        if (Wait)
            return FALSE;

        m_CommandIndex++;
    }

    m_DMACommandQueue.clear();
    m_CommandIndex = 0;
    return TRUE;
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11DMAEngineContextX<ABI>
D3D11_DECLARE_ABI_TEMPLATES();