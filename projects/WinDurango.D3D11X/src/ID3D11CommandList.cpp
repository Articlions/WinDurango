#include "ID3D11CommandList.h"

//
// IUnknown
//
template<abi_t ABI>
HRESULT D3D11CommandList<ABI>::QueryInterface(REFIID riid, void** ppvObject)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template<abi_t ABI>
ULONG D3D11CommandList<ABI>::AddRef()
{
    if (m_pFunction) m_pFunction->AddRef();
    return InterlockedIncrement(&this->m_RefCount);
}

template<abi_t ABI>
ULONG D3D11CommandList<ABI>::Release()
{
    if (m_pFunction) m_pFunction->Release();
    ULONG RefCount = InterlockedDecrement(&this->m_RefCount);
    if (RefCount <= 1)
    {
        for (auto& command : m_Commands)
        {
            if (command.m_CommandType == DrawBundlesCommandType::IASetVertexBuffers)
            {
                if (command.IASetVertexBuffers.ppVertexBuffers)
                {
                    if (!(m_Flags & gfx::D3D11_CREATE_DEFERRED_CONTEXT_TITLE_MANAGED_COMMAND_LIST_OBJECT_LIFETIMES))
                    {
                        for (UINT i = 0; i < command.IASetVertexBuffers.NumBuffers; ++i)
                        {
                            if (command.IASetVertexBuffers.ppVertexBuffers[i])
                            {
                                command.IASetVertexBuffers.ppVertexBuffers[i]->Release();
                            }
                        }
                    }

                    delete[] command.IASetVertexBuffers.ppVertexBuffers;
                    command.IASetVertexBuffers.ppVertexBuffers = nullptr;
                }
                if (command.IASetVertexBuffers.pStrides)
                {
                    delete[] command.IASetVertexBuffers.pStrides;
                    command.IASetVertexBuffers.pStrides = nullptr;
                }
                if (command.IASetVertexBuffers.pOffsets)
                {
                    delete[] command.IASetVertexBuffers.pOffsets;
                    command.IASetVertexBuffers.pOffsets = nullptr;
                }
            }
            else if (command.m_CommandType == DrawBundlesCommandType::VSSetShaderResourcesPC)
            {
                if (command.VSSetShaderResourcesPC.ppShaderResourceViews)
                {
                    if (!(m_Flags & gfx::D3D11_CREATE_DEFERRED_CONTEXT_TITLE_MANAGED_COMMAND_LIST_OBJECT_LIFETIMES))
                    {
                        for (UINT i = 0; i < command.VSSetShaderResourcesPC.NumViews; ++i)
                        {
                            if (command.VSSetShaderResourcesPC.ppShaderResourceViews[i])
                            {
                                command.VSSetShaderResourcesPC.ppShaderResourceViews[i]->Release();
                            }
                        }
                    }

                    delete[] command.VSSetShaderResourcesPC.ppShaderResourceViews;
                    command.VSSetShaderResourcesPC.ppShaderResourceViews = nullptr;
                }
            }
            else if (command.m_CommandType == DrawBundlesCommandType::PSSetShaderResourcesPC)
            {
                if (command.PSSetShaderResourcesPC.ppShaderResourceViews)
                {
                    if (!(m_Flags & gfx::D3D11_CREATE_DEFERRED_CONTEXT_TITLE_MANAGED_COMMAND_LIST_OBJECT_LIFETIMES))
                    {
                        for (UINT i = 0; i < command.PSSetShaderResourcesPC.NumViews; ++i)
                        {
                            if (command.PSSetShaderResourcesPC.ppShaderResourceViews[i])
                            {
                                command.PSSetShaderResourcesPC.ppShaderResourceViews[i]->Release();
                            }
                        }
                    }

                    delete[] command.PSSetShaderResourcesPC.ppShaderResourceViews;
                    command.PSSetShaderResourcesPC.ppShaderResourceViews = nullptr;
                }
            }
            else if (command.m_CommandType == DrawBundlesCommandType::PSSetSamplers)
            {
                if (command.PSSetSamplers.ppSamplers)
                {
                    delete[] command.PSSetSamplers.ppSamplers;
                    command.PSSetSamplers.ppSamplers = nullptr;
                }
            }

            if (!(m_Flags & gfx::D3D11_CREATE_DEFERRED_CONTEXT_TITLE_MANAGED_COMMAND_LIST_OBJECT_LIFETIMES))
            {
                if (command.m_CommandType == DrawBundlesCommandType::OMSetBlendState)
                {
                    if (command.OMSetBlendState.pBlendState)
                    {
                        command.OMSetBlendState.pBlendState->Release();
                        command.OMSetBlendState.pBlendState = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::DSSetShader)
                {
                    if (command.DSSetShader.pDomainShader)
                    {
                        command.DSSetShader.pDomainShader->Release();
                        command.DSSetShader.pDomainShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::HSSetShader)
                {
                    if (command.HSSetShader.pHullShader)
                    {
                        command.HSSetShader.pHullShader->Release();
                        command.HSSetShader.pHullShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::GSSetShader)
                {
                    if (command.DSSetShader.pDomainShader)
                    {
                        command.DSSetShader.pDomainShader->Release();
                        command.DSSetShader.pDomainShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::CSSetShader)
                {
                    if (command.HSSetShader.pHullShader)
                    {
                        command.HSSetShader.pHullShader->Release();
                        command.HSSetShader.pHullShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::VSSetShader)
                {
                    if (command.VSSetShader.pVertexShader)
                    {
                        command.VSSetShader.pVertexShader->Release();
                        command.VSSetShader.pVertexShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::PSSetShader)
                {
                    if (command.PSSetShader.pPixelShader)
                    {
                        command.PSSetShader.pPixelShader->Release();
                        command.PSSetShader.pPixelShader = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::PSSetFastShaderResource)
                {
                    if (command.PSSetFastShaderResource.pShaderResourceView)
                    {
                        command.PSSetFastShaderResource.pShaderResourceView->Release();
                        command.PSSetFastShaderResource.pShaderResourceView = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::VSSetFastShaderResource)
                {
                    if (command.VSSetFastShaderResource.pShaderResourceView)
                    {
                        command.VSSetFastShaderResource.pShaderResourceView->Release();
                        command.VSSetFastShaderResource.pShaderResourceView = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::VSSetFastConstantBuffer)
                {
                    if (command.VSSetFastConstantBuffer.pConstantBuffer)
                    {
                        command.VSSetFastConstantBuffer.pConstantBuffer->Release();
                        command.VSSetFastConstantBuffer.pConstantBuffer = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::PSSetFastConstantBuffer)
                {
                    if (command.PSSetFastConstantBuffer.pConstantBuffer)
                    {
                        command.PSSetFastConstantBuffer.pConstantBuffer->Release();
                        command.PSSetFastConstantBuffer.pConstantBuffer = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::PSSetPlacementShaderResource)
                {
                    if (command.PSSetPlacementShaderResource.pShaderResourceView)
                    {
                        command.PSSetPlacementShaderResource.pShaderResourceView->Release();
                        command.PSSetPlacementShaderResource.pShaderResourceView = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::VSSetPlacementShaderResource)
                {
                    if (command.VSSetPlacementShaderResource.pShaderResourceView)
                    {
                        command.VSSetPlacementShaderResource.pShaderResourceView->Release();
                        command.VSSetPlacementShaderResource.pShaderResourceView = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::VSSetPlacementConstantBuffer)
                {
                    if (command.VSSetPlacementConstantBuffer.pConstantBuffer)
                    {
                        command.VSSetPlacementConstantBuffer.pConstantBuffer->Release();
                        command.VSSetPlacementConstantBuffer.pConstantBuffer = nullptr;
                    }
                }
                else if (command.m_CommandType == DrawBundlesCommandType::PSSetPlacementConstantBuffer)
                {
                    if (command.PSSetPlacementConstantBuffer.pConstantBuffer)
                    {
                        command.PSSetPlacementConstantBuffer.pConstantBuffer->Release();
                        command.PSSetPlacementConstantBuffer.pConstantBuffer = nullptr;
                    }
                }
            }
        }

        m_Commands.clear();
    }

    if (!RefCount) delete this;
    return RefCount;
}

//
// ID3D11DeviceChild
//
template<abi_t ABI>
void D3D11CommandList<ABI>::GetDevice(gfx::ID3D11Device<ABI>** ppDevice)
{
    IMPLEMENT_STUB();
}

template<abi_t ABI>
HRESULT D3D11CommandList<ABI>::GetPrivateData(_GUID const& guid, uint32_t* pDataSize, void* pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template<abi_t ABI>
HRESULT D3D11CommandList<ABI>::SetPrivateData(_GUID const& guid, uint32_t DataSize, void const* pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template<abi_t ABI>
HRESULT D3D11CommandList<ABI>::SetPrivateDataInterface(_GUID const& guid, IUnknown const* pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

template<abi_t ABI>
HRESULT D3D11CommandList<ABI>::SetPrivateDataInterfaceGraphics(_GUID const& guid, xbox::IGraphicsUnknown<ABI> const* pData)
{
    IMPLEMENT_STUB();
    return E_NOTIMPL;
}

//
// ID3D11CommandList
//
template<abi_t ABI>
uint32_t D3D11CommandList<ABI>::GetContextFlags()
{
    IMPLEMENT_STUB();
    return 0;
}

#undef ABI_INTERFACE
#define ABI_INTERFACE(ABI) D3D11CommandList<ABI>
D3D11_DECLARE_ABI_TEMPLATES();