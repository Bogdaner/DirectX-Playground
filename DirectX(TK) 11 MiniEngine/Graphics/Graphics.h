#pragma once

#include <algorithm>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

#include "AdapterReader.h"
#include "Camera.h"
#include "ConstantBuffer.hpp"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "..\\Timer.h"
#include "Vertex.h"
#include "VertexBuffer.hpp"

#include "ImGui\\imgui.h"
#include "ImGui\\imgui_impl_win32.h"
#include "ImGui\\imgui_impl_dx11.h"


class Graphics
{
public:
    ~Graphics();
    bool Initialize( HWND hwnd, const unsigned int width, const unsigned int height );
    void RenderFrame();

    Camera camera;
protected:
private:
    bool InitializeDirectX( HWND hwnd );
    bool InitializeShaders();
    bool InitializeScene();
    bool InitializeImGui( HWND hwnd );

    void RenderImGuiFrame() const;

    void InitializeFonts();
    bool SetupDeviceAndSwapchain( const HWND hwnd );
    bool SetupZBuffer();
    bool SetupRasterizer();
    bool SetupSamplerState();


    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indicesBuffer;

    ConstantBuffer<CB_VS_vertexshader> constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> spriteFont;

    VertexShader vertexShader;
    PixelShader pixelShader;

    Timer fpsTimer;

    unsigned int windowHeight;
    unsigned int windowWidth;
};