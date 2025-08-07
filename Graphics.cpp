#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Graphics.h"
#include "GraphicsDeviceRemovedException.h"
#include "GraphicsException.h"

namespace Client
{
    Graphics::Graphics(const HWND hWnd, const int width, const int height)
    {
        CreateDeviceAndSwapChain(hWnd);

        CreateRenderTargetView();

        CreateAndSetDepthStencilState();

        CreateDepthStencilView(width, height);

        ConfigureViewport(width, height);

        _pDeviceContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(), _pDepthStencilView.Get());
    }

    void Graphics::ClearBuffer(const float r, const float g, const float b) const noexcept
    {
        const float color[] = { r, g, b, 1.0f };
        _pDeviceContext->ClearRenderTargetView(_pRenderTargetView.Get(), color);
        _pDeviceContext->ClearDepthStencilView(_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void Graphics::EndFrame()
    {
        _informationManager.Set();

        const auto hResult = _pSwapChain->Present(1, 0);

        if (FAILED(hResult))
        {
            if (hResult == DXGI_ERROR_DEVICE_REMOVED)
            {
                throw GraphicsDeviceRemovedException("Graphics device removed.", _pDevice->GetDeviceRemovedReason(), _informationManager);
            }
            throw GraphicsException("Graphics failure.", hResult, _informationManager);
        }
    }

    ID3D11Device* Graphics::GetDevice() const noexcept
    {
        return _pDevice.Get();
    }

    ID3D11DeviceContext* Graphics::GetDeviceContext() const noexcept
    {
        return _pDeviceContext.Get();
    }

    const DXGIInformationManager& Graphics::GetInformationManager() const noexcept
    {
        return _informationManager;
    }

    void Graphics::CreateDeviceAndSwapChain(const HWND hWnd)
    {
        DXGI_SWAP_CHAIN_DESC desc;
        desc.BufferDesc.Width = 0;
        desc.BufferDesc.Height = 0;
        desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BufferDesc.RefreshRate.Numerator = 0;
        desc.BufferDesc.RefreshRate.Denominator = 0;
        desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 1;
        desc.OutputWindow = hWnd;
        desc.Windowed = TRUE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        desc.Flags = 0;

        UINT swapCreateFlags = 0;
#ifndef NDEBUG
        swapCreateFlags = swapCreateFlags | D3D11_CREATE_DEVICE_DEBUG;
#endif
        _informationManager.Set();
        const auto hResult = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            swapCreateFlags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &desc,
            &_pSwapChain,
            &_pDevice,
            nullptr,
            &_pDeviceContext
        );

        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to create graphics device.", hResult, _informationManager);
        }
    }

    void Graphics::CreateRenderTargetView()
    {
        WRL::ComPtr<ID3D11Resource> pBackBuffer;
        _informationManager.Set();
        auto hResult = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to get back buffer.", hResult, _informationManager);
        }

        _informationManager.Set();
        hResult = _pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &_pRenderTargetView);
        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to create render target view.", hResult, _informationManager);
        }
    }

    void Graphics::CreateAndSetDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable = true;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;

        WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;

        _informationManager.Set();
        const auto hResult = _pDevice->CreateDepthStencilState(&desc, &pDepthStencilState);
        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to create depth stencil state.", hResult, _informationManager);
        }

        _pDeviceContext->OMSetDepthStencilState(pDepthStencilState.Get(), 1);
    }

    void Graphics::CreateDepthStencilView(const int width, const int height)
    {
        auto pDepthStencil = CreateDepthStencilTexture(width, height);

        D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
        desc.Format = DXGI_FORMAT_D32_FLOAT;
        desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipSlice = 0;

        _informationManager.Set();
        const auto hResult = _pDevice->CreateDepthStencilView(pDepthStencil.Get(), &desc, &_pDepthStencilView);
        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to create depth stencil state.", hResult, _informationManager);
        }
    }

    WRL::ComPtr<ID3D11Texture2D> Graphics::CreateDepthStencilTexture(const int width, const int height)
    {
        WRL::ComPtr<ID3D11Texture2D> pDepthStencilTexture;

        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        _informationManager.Set();
        const auto hResult = _pDevice->CreateTexture2D(&desc, nullptr, &pDepthStencilTexture);
        if (FAILED(hResult))
        {
            throw GraphicsException("Failed to create texture.", hResult, _informationManager);
        }

        return pDepthStencilTexture;
    }

    void Graphics::ConfigureViewport(const int width, const int height) const noexcept
    {
        D3D11_VIEWPORT viewport;
        viewport.Width = static_cast<FLOAT>(width);
        viewport.Height = static_cast<FLOAT>(height);
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;

        _pDeviceContext->RSSetViewports(1, &viewport);
    }
}
