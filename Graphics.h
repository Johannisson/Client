// ReSharper disable CppInconsistentNaming

#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>

#include "GraphicsException.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Client
{
    class Graphics
    {
    public:
        DirectX::XMMATRIX ProjectionMatrix;
        DirectX::XMMATRIX CameraMatrix;

        Graphics(HWND hWnd, int width, int height);
        ~Graphics() = default;

        Graphics(const Graphics&) = delete;
        Graphics(Graphics&&) = delete;

        Graphics& operator=(const Graphics&) = delete;
        Graphics& operator=(Graphics&& _) = delete;

        void ClearBuffer(float r, float g, float b) const noexcept;
        void EndFrame();

        [[nodiscard]] ID3D11Device* GetDevice() const noexcept;
        [[nodiscard]] ID3D11DeviceContext* GetDeviceContext() const noexcept;
        [[nodiscard]] const DXGIInformationManager& GetInformationManager() const noexcept;

    private:
        void CreateDeviceAndSwapChain(HWND hWnd);
        void CreateRenderTargetView();
        void CreateAndSetDepthStencilState();
        void CreateDepthStencilView(int width, int height);
        [[nodiscard]] WRL::ComPtr<ID3D11Texture2D> CreateDepthStencilTexture(int width, int height);
        void ConfigureViewport(int width, int height) const noexcept;

        WRL::ComPtr<ID3D11Device> _pDevice;
        WRL::ComPtr<IDXGISwapChain> _pSwapChain;
        WRL::ComPtr<ID3D11DeviceContext> _pDeviceContext;
        WRL::ComPtr<ID3D11RenderTargetView> _pRenderTargetView;
        WRL::ComPtr<ID3D11DepthStencilView> _pDepthStencilView;
        DXGIInformationManager _informationManager;
    };
}
