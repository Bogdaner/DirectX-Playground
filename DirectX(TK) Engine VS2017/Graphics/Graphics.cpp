#include "Graphics.h"

bool Graphics::Initialize( HWND hwnd, const unsigned int width, const unsigned int height )
{
    if ( !InitializeDirectX( hwnd, width, height ) )
        return false;

    if ( !InitializeShaders() )
        return false;

    if ( !InitializeScene() )
        return false;

    return true;
}

void Graphics::RenderFrame()
{
    const float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView( renderTargetView.Get(), bgcolor );
    deviceContext->ClearDepthStencilView( depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

    deviceContext->IASetInputLayout( vertexShader.GetInputLayout() );
    deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    deviceContext->RSSetState( rasterizer.Get() );
    deviceContext->OMSetDepthStencilState( depthStencilState.Get(), 0 );

    deviceContext->PSSetSamplers( 0, 1, samplerState.GetAddressOf() );

    deviceContext->VSSetShader( vertexShader.GetShader(), NULL, 0 );
    deviceContext->PSSetShader( pixelShader.GetShader(), NULL, 0 );

    UINT stride = sizeof( Vertex );
    UINT offset = 0;
    // Red Triangle
    deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), &stride, &offset );
    deviceContext->Draw( 3, 0 );

    // Draw Text
    spriteBatch->Begin();
    spriteFont->DrawString( spriteBatch.get(), 
                            "Hello World", 
                            DirectX::XMFLOAT2( 0.0f, 0.0f ), 
                            DirectX::Colors::White, 0.0f, 
                            DirectX::XMFLOAT2( 0.0f, 0.0f ), 
                            DirectX::XMFLOAT2( 1.0f, 1.0f ) 
    );

    spriteBatch->End();

    swapchain->Present( 1, NULL ); // first argument vsync on/off
}

bool Graphics::InitializeDirectX( HWND hwnd, const unsigned int width, const unsigned int height )
{
    if ( !SetupDeviceAndSwapchain( hwnd, width, height ) )
        return false;

    if ( !SetupZBuffer( width, height ) )
        return false;

    if ( !SetupRasterizer( width, height ) )
        return false;

    if ( !SetupSamplerState() )
        return false;

    InitializeFonts();
    return true;
}

bool Graphics::InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    const UINT numElements = ARRAYSIZE( layout );

    if ( !vertexShader.Initialize( device, layout, numElements ) )
        return false;

    if ( !pixelShader.Initialize( device ) )
        return false;

    return true;
}

bool Graphics::InitializeScene()
{
    std::vector<Vertex> v;
    // Red triangle
    v.push_back( { -0.5f, -0.5f, 1.0f, 0.0f, 1.0f} );
    v.push_back( { 0.0f, 0.5f, 1.0f, 0.5f, 0.0f } );
    v.push_back( { 0.5f, -0.5f, 1.0f, 1.0f, 1.0f} );

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof( Vertex ) * static_cast<UINT>(v.size()); //ARRAYSIZE( v.data() );
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferInitData;
    ZeroMemory( &vertexBufferInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );
    vertexBufferInitData.pSysMem = v.data();

    HRESULT hr = device->CreateBuffer( &vertexBufferDesc, &vertexBufferInitData, vertexBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create VertexBuffer." );
        return false;
    }

    return true;
}

void Graphics::InitializeFonts()
{
    spriteBatch = std::make_unique<DirectX::SpriteBatch>( deviceContext.Get() );
    spriteFont = std::make_unique<DirectX::SpriteFont>( device.Get(), L"Data\\Fonts\\comic_sans_ms_16_spritefont" );
}

bool Graphics::SetupDeviceAndSwapchain( const HWND hwnd, const unsigned int width, const unsigned int height )
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    if ( adapters.empty() )
    {
        ErrorLogger::Log( "No IDXGI Adapters found." );
        return false;
    }

    // Get graphic adapter with the most memory
    auto max_itr = std::max_element( adapters.begin(), adapters.end(), []( const AdapterData& a, const AdapterData& b ) -> bool
    {
        return a.description.DedicatedVideoMemory < b.description.DedicatedVideoMemory;
    } );

    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory( &scd, sizeof( DXGI_SWAP_CHAIN_DESC ) );

    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;

    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1;
    scd.OutputWindow = hwnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        max_itr->pAdapter,              // IDXGI Adapter
        D3D_DRIVER_TYPE_UNKNOWN,        // FOR SOFTWARE DRIVER TYPE
        NULL,                           // FLAGS FOR RUNTIME LAYERS
        NULL,                           // FEATURE LEVELS ARRAYS
        NULL,
        0,
        D3D11_SDK_VERSION,
        &scd,                           // Swapchain description
        swapchain.GetAddressOf(),
        device.GetAddressOf(),
        NULL,                           // SUPPORTED FEATURE LEVEL
        deviceContext.GetAddressOf() );

    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "Failed to create device and swpachain" );
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    hr = swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( backBuffer.GetAddressOf() ) );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "GetBuffer failed" );
        return false;
    }

    hr = device->CreateRenderTargetView( backBuffer.Get(), NULL, renderTargetView.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "Failed to create render target view" );
        return false;
    }

    return true;
}

bool Graphics::SetupZBuffer( const unsigned int width, const unsigned int height )
{
    //Describe our Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDescTex;
    depthStencilDescTex.Width = width;
    depthStencilDescTex.Height = height;
    depthStencilDescTex.MipLevels = 1;
    depthStencilDescTex.ArraySize = 1;
    depthStencilDescTex.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDescTex.SampleDesc.Count = 1;
    depthStencilDescTex.SampleDesc.Quality = 0;
    depthStencilDescTex.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDescTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDescTex.CPUAccessFlags = 0;
    depthStencilDescTex.MiscFlags = 0;

    HRESULT hr = this->device->CreateTexture2D( &depthStencilDescTex, NULL, this->depthStencilBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create depth stencil buffer." );
        return false;
    }

    hr = this->device->CreateDepthStencilView( this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create depth stencil view." );
        return false;
    }

    deviceContext->OMSetRenderTargets( 1, renderTargetView.GetAddressOf(), depthStencilView.Get() );

    // Create Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory( &depthStencilDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->device->CreateDepthStencilState( &depthStencilDesc, this->depthStencilState.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create depth stencil state." );
        return false;
    }

    return true;
}

bool Graphics::SetupRasterizer( const unsigned int width, const unsigned int height )
{
    // RASTERIZER SETUP
    D3D11_VIEWPORT viewport;
    ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast< float >( width );
    viewport.Height = static_cast< float >( height );
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    // Set the viewport
    deviceContext->RSSetViewports( 1, &viewport );

    // Create Rasterizer State
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory( &rasterizerDesc, sizeof( D3D11_RASTERIZER_DESC ) );
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

    HRESULT hr = device->CreateRasterizerState( &rasterizerDesc, rasterizer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create rasterizer state" );
        return false;
    }

    return true;
}

bool Graphics::SetupSamplerState()
{
    // Create sampler description for sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory( &sampDesc, sizeof( D3D11_SAMPLER_DESC ) );
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MaxLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState( &sampDesc, samplerState.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create Sampler State" );
        return false;
    }

    return true;
}
