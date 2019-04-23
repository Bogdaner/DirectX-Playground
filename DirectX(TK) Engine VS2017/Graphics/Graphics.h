#pragma once
#include "AdapterReader.h"

class Graphics
{
public:
    bool Initialize( HWND hwnd, const int width, const int height );
protected:
private:
    bool InitializeDirectX( HWND hwnd, const int width, const int height );

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
};