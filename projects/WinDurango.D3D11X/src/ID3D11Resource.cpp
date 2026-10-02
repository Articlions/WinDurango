#include "ID3D11Resource.h"
#include "d3d11.x.h"

//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11Resource<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    if (riid == xcom::guid_of<gfx::ID3D11Resource>())
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else if (riid == xcom::guid_of<gfx::ID3D11Texture2D>())
    {
        ::ID3D11Texture2D *pTex2D = nullptr;
        HRESULT hr = m_pFunction->QueryInterface(IID_PPV_ARGS(&pTex2D));
        if (SUCCEEDED(hr))
        {
            *ppvObject = new D3D11Texture2D<ABI>(pTex2D);
            AddRef();
            return S_OK;
        }
        else
        {
            *ppvObject = nullptr;
            return E_NOINTERFACE;
        }
    }

    IMPLEMENT_STUB();
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

template <abi_t ABI> ULONG D3D11Resource<ABI>::AddRef()
{
    m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11Resource<ABI>::Release()
{
    m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        auto it = g_AllocationBaseMap.find(this);
        if (it != g_AllocationBaseMap.end())
        {
            std::lock_guard PlacementUpdateLock(g_ResourceMapMutex);
            auto range = g_ResourceMap.equal_range(it->second);
            for (auto it2 = range.first; it2 != range.second;)
            {
                if (it2->second == this)
                {
                    it2 = g_ResourceMap.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }

            g_AllocationBaseMap.erase(this);
        }

        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11Resource<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> HRESULT D3D11Resource<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Resource<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Resource<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Resource<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid, xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11Resource
//
template <abi_t ABI> void D3D11Resource<ABI>::GetType(D3D11_RESOURCE_DIMENSION *pDimension)
{
    m_pFunction->GetType(pDimension);
}

template <abi_t ABI> void D3D11Resource<ABI>::SetEvictionPriority(uint32_t EvictionPriority)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> uint32_t D3D11Resource<ABI>::GetEvictionPriority()
{
    IMPLEMENT_STUB();
    return {};
}

template <abi_t ABI> void D3D11Resource<ABI>::GetDescriptor(gfx::D3D11X_DESCRIPTOR_RESOURCE *pDesc)
{
    IMPLEMENT_STUB();
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11Resource<ABI>
D3D11_DECLARE_ABI_TEMPLATES();


//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11Texture1D<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    if (riid == xcom::guid_of<gfx::ID3D11Texture1D>() || riid == xcom::guid_of<gfx::ID3D11Resource>())
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    IMPLEMENT_STUB();
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

template <abi_t ABI> ULONG D3D11Texture1D<ABI>::AddRef()
{
    m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11Texture1D<ABI>::Release()
{
    m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        auto it = g_AllocationBaseMap.find(this);
        if (it != g_AllocationBaseMap.end())
        {
            std::lock_guard PlacementUpdateLock(g_ResourceMapMutex);
            auto range = g_ResourceMap.equal_range(it->second);
            for (auto it2 = range.first; it2 != range.second;)
            {
                if (it2->second == this)
                {
                    it2 = g_ResourceMap.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }

            g_AllocationBaseMap.erase(this);
        }

        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11Texture1D<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> HRESULT D3D11Texture1D<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture1D<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Texture1D<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture1D<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid,
                                                             xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11Resource
//
template <abi_t ABI> void D3D11Texture1D<ABI>::GetType(D3D11_RESOURCE_DIMENSION *pDimension)
{
    *pDimension = D3D11_RESOURCE_DIMENSION_TEXTURE1D;
}

template <abi_t ABI> void D3D11Texture1D<ABI>::SetEvictionPriority(uint32_t EvictionPriority)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> uint32_t D3D11Texture1D<ABI>::GetEvictionPriority()
{
    IMPLEMENT_STUB();
    return {};
}

template <abi_t ABI> void D3D11Texture1D<ABI>::GetDescriptor(gfx::D3D11X_DESCRIPTOR_RESOURCE *pDesc)
{
    IMPLEMENT_STUB();
}

//
// ID3D11Texture1D
//
template <abi_t ABI> void D3D11Texture1D<ABI>::GetDesc(D3D11_TEXTURE1D_DESC *pDesc)
{
    m_pFunction->GetDesc(pDesc);
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11Texture1D<ABI>
D3D11_DECLARE_ABI_TEMPLATES();


//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11Texture2D<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    if (riid == xcom::guid_of<gfx::ID3D11Texture2D>() || riid == xcom::guid_of<gfx::ID3D11Resource>())
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    IMPLEMENT_STUB();
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

template <abi_t ABI> ULONG D3D11Texture2D<ABI>::AddRef()
{
    m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11Texture2D<ABI>::Release()
{
    m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        auto it = g_AllocationBaseMap.find(this);
        if (it != g_AllocationBaseMap.end())
        {
            std::lock_guard PlacementUpdateLock(g_ResourceMapMutex);
            auto range = g_ResourceMap.equal_range(it->second);
            for (auto it2 = range.first; it2 != range.second;)
            {
                if (it2->second == this)
                {
                    it2 = g_ResourceMap.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }

            g_AllocationBaseMap.erase(this);
        }

        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11Texture2D<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> HRESULT D3D11Texture2D<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture2D<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Texture2D<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture2D<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid,
                                                             xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11Resource
//
template <abi_t ABI> void D3D11Texture2D<ABI>::GetType(D3D11_RESOURCE_DIMENSION *pDimension)
{
    *pDimension = D3D11_RESOURCE_DIMENSION_TEXTURE2D;
}

template <abi_t ABI> void D3D11Texture2D<ABI>::SetEvictionPriority(uint32_t EvictionPriority)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> uint32_t D3D11Texture2D<ABI>::GetEvictionPriority()
{
    IMPLEMENT_STUB();
    return {};
}

template <abi_t ABI> void D3D11Texture2D<ABI>::GetDescriptor(gfx::D3D11X_DESCRIPTOR_RESOURCE *pDesc)
{
    IMPLEMENT_STUB();
}

//
// ID3D11Texture2D
//
template <abi_t ABI> void D3D11Texture2D<ABI>::GetDesc(D3D11_TEXTURE2D_DESC *pDesc)
{
    m_pFunction->GetDesc(pDesc);
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11Texture2D<ABI>
D3D11_DECLARE_ABI_TEMPLATES();


//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11Texture3D<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    if (riid == xcom::guid_of<gfx::ID3D11Texture3D>() || riid == xcom::guid_of<gfx::ID3D11Resource>())
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    IMPLEMENT_STUB();
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

template <abi_t ABI> ULONG D3D11Texture3D<ABI>::AddRef()
{
    m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11Texture3D<ABI>::Release()
{
    m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        auto it = g_AllocationBaseMap.find(this);
        if (it != g_AllocationBaseMap.end())
        {
            std::lock_guard PlacementUpdateLock(g_ResourceMapMutex);
            auto range = g_ResourceMap.equal_range(it->second);
            for (auto it2 = range.first; it2 != range.second;)
            {
                if (it2->second == this)
                {
                    it2 = g_ResourceMap.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }

            g_AllocationBaseMap.erase(this);
        }

        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11Texture3D<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> HRESULT D3D11Texture3D<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture3D<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Texture3D<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Texture3D<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid,
                                                             xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11Resource
//
template <abi_t ABI> void D3D11Texture3D<ABI>::GetType(D3D11_RESOURCE_DIMENSION *pDimension)
{
    *pDimension = D3D11_RESOURCE_DIMENSION_TEXTURE3D;
}

template <abi_t ABI> void D3D11Texture3D<ABI>::SetEvictionPriority(uint32_t EvictionPriority)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> uint32_t D3D11Texture3D<ABI>::GetEvictionPriority()
{
    IMPLEMENT_STUB();
    return {};
}

template <abi_t ABI> void D3D11Texture3D<ABI>::GetDescriptor(gfx::D3D11X_DESCRIPTOR_RESOURCE *pDesc)
{
    IMPLEMENT_STUB();
}

//
// ID3D11Texture3D
//
template <abi_t ABI> void D3D11Texture3D<ABI>::GetDesc(D3D11_TEXTURE3D_DESC *pDesc)
{
    m_pFunction->GetDesc(pDesc);
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11Texture3D<ABI>
D3D11_DECLARE_ABI_TEMPLATES();


//
// IUnknown
//
template <abi_t ABI> HRESULT D3D11Buffer<ABI>::QueryInterface(REFIID riid, void **ppvObject)
{
    if (riid == xcom::guid_of<gfx::ID3D11Buffer>() || riid == xcom::guid_of<gfx::ID3D11Resource>())
    {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }

    IMPLEMENT_STUB();
    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

template <abi_t ABI> ULONG D3D11Buffer<ABI>::AddRef()
{
    m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template <abi_t ABI> ULONG D3D11Buffer<ABI>::Release()
{
    m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (!RefCount)
    {
        auto it = g_AllocationBaseMap.find(this);
        if (it != g_AllocationBaseMap.end())
        {
            std::lock_guard PlacementUpdateLock(g_ResourceMapMutex);
            auto range = g_ResourceMap.equal_range(it->second);
            for (auto it2 = range.first; it2 != range.second;)
            {
                if (it2->second == this)
                {
                    it2 = g_ResourceMap.erase(it2);
                }
                else
                {
                    ++it2;
                }
            }

            g_AllocationBaseMap.erase(this);
        
            if (g_IAFastIndexBuffer<ABI> == this)
            {
                g_IAFastIndexBuffer<ABI> = nullptr;
            }

            for (UINT i = 0; i < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; ++i)
            {
                if (g_PSFastConstantBuffers<ABI>[i] == this)
                {
                    g_PSFastConstantBuffers<ABI>[i] = nullptr;
                }
                if (g_VSFastConstantBuffers<ABI>[i] == this)
                {
                    g_VSFastConstantBuffers<ABI>[i] = nullptr;
                }
                if (g_CSFastConstantBuffers<ABI>[i] == this)
                {
                    g_CSFastConstantBuffers<ABI>[i] = nullptr;
                }
                if (g_GSFastConstantBuffers<ABI>[i] == this)
                {
                    g_GSFastConstantBuffers<ABI>[i] = nullptr;
                }
                if (g_HSFastConstantBuffers<ABI>[i] == this)
                {
                    g_HSFastConstantBuffers<ABI>[i] = nullptr;
                }
                if (g_DSFastConstantBuffers<ABI>[i] == this)
                {
                    g_DSFastConstantBuffers<ABI>[i] = nullptr;
                }
            }

            for (UINT i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; ++i)
            {
                if (g_IAFastVertexBuffers<ABI>[i] == this)
                {
                    g_IAFastVertexBuffers<ABI>[i] = nullptr;
                }
            }
        }

        delete this;
    }
    return RefCount;
}

//
// ID3D11DeviceChild
//
template <abi_t ABI> void D3D11Buffer<ABI>::GetDevice(gfx::ID3D11Device<ABI> **ppDevice)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> HRESULT D3D11Buffer<ABI>::GetPrivateData(_GUID const &guid, uint32_t *pDataSize, void *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Buffer<ABI>::SetPrivateData(_GUID const &guid, uint32_t DataSize, void const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI> HRESULT D3D11Buffer<ABI>::SetPrivateDataInterface(_GUID const &guid, IUnknown const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template <abi_t ABI>
HRESULT D3D11Buffer<ABI>::SetPrivateDataInterfaceGraphics(_GUID const &guid, xbox::IGraphicsUnknown<ABI> const *pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11Resource
//
template <abi_t ABI> void D3D11Buffer<ABI>::GetType(D3D11_RESOURCE_DIMENSION *pDimension)
{
    *pDimension = D3D11_RESOURCE_DIMENSION_BUFFER;
}

template <abi_t ABI> void D3D11Buffer<ABI>::SetEvictionPriority(uint32_t EvictionPriority)
{
    IMPLEMENT_STUB();
}

template <abi_t ABI> uint32_t D3D11Buffer<ABI>::GetEvictionPriority()
{
    IMPLEMENT_STUB();
    return {};
}

template <abi_t ABI> void D3D11Buffer<ABI>::GetDescriptor(gfx::D3D11X_DESCRIPTOR_RESOURCE *pDesc)
{
    IMPLEMENT_STUB();
}

//
// ID3D11Buffer
//
template <abi_t ABI> void D3D11Buffer<ABI>::GetDesc(D3D11_BUFFER_DESC *pDesc)
{
    m_pFunction->GetDesc(pDesc);
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11Buffer<ABI>
D3D11_DECLARE_ABI_TEMPLATES();