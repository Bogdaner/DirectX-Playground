#pragma once

#include <algorithm>
#include <SpriteBatch.h>
#include <SpriteFont.h>

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

    void InitializeFonts();
    bool SetupDeviceAndSwapchain( const HWND hwnd, const unsigned int width, const unsigned int height );
    bool SetupZBuffer( const unsigned int width, const unsigned int height );
    bool SetupRasterizer( const unsigned int width, const unsigned int height );
    bool SetupSamplerState();


    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> spriteFont;

    VertexShader vertexShader;
    PixelShader pixelShader;
};