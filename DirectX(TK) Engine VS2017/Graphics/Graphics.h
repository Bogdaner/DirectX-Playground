#pragma once

#include <algorithm>

#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"

class Graphics
{
public:
    bool Initialize( HWND hwnd, const unsigned int width, const unsigned int height );
    void RenderFrame();
protected:
private:
    bool InitializeDirectX( HWND hwnd, const unsigned int width, const unsigned int height );
    bool InitializeShaders();
    bool InitializeScene();

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    
    VertexShader vertexShader;
    PixelShader pixelShader;
};