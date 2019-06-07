#include "Graphics.h"

bool Graphics::Initialize( HWND hwnd, const unsigned int width, const unsigned int height )
{
    if ( !InitializeDirectX( hwnd, width, height ) )
    {
        return false;
    }

    if ( !InitializeShaders() )
    {
        return false;
    }

    if ( !InitializeScene() )
    {
        return false;
    }

    return true;
}

void Graphics::RenderFrame()
{
    const float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView( renderTargetView.Get(), bgcolor );

    deviceContext->IASetInputLayout( vertexShader.GetInputLayout() );
    deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    UINT stride = sizeof( Vertex );
    UINT offset = 0;
    deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), &stride, &offset );

    deviceContext->VSSetShader( vertexShader.GetShader(), NULL, 0 );
    deviceContext->PSSetShader( pixelShader.GetShader(), NULL, 0 );

    deviceContext->Draw( 3, 0 );
    swapchain->Present( 1, NULL ); // first argument vsync on/off
}

bool Graphics::InitializeDirectX( HWND hwnd, const unsigned int width, const unsigned int height )
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
    });

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
    hr = swapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( backBuffer.GetAddressOf() ) );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "GetBuffer failed" );
        return false;
    }

    hr = device->CreateRenderTargetView( backBuffer.Get() , NULL, renderTargetView.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "Failed to create render target view" );
        return false;
    }

    deviceContext->OMSetRenderTargets( 1, renderTargetView.GetAddressOf(), NULL );

    // RASTERIZER SETUP
    D3D11_VIEWPORT viewport;
    ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>( width );
    viewport.Height = static_cast<float>( height );

    // Set the viewport
    deviceContext->RSSetViewports( 1, &viewport );

    return true;
}

bool Graphics::InitializeShaders()
{
    D3D11_INPUT_ELEMENT_DESC layout[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    const UINT numElements = ARRAYSIZE( layout );

    if ( !vertexShader.Initialize( device, layout, numElements ) )
    {
        return false;
    }

    if ( !pixelShader.Initialize( device ) )
    {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene()
{
    Vertex v[] =
    {
        { -0.1f, 0.0f },
        { 0.0f, 0.1f },
        { 0.1f, 0.0f },
    };

    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof( Vertex ) * ARRAYSIZE( v );
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferInitData;
    ZeroMemory( &vertexBufferInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );
    vertexBufferInitData.pSysMem = v;

    HRESULT hr = device->CreateBuffer( &vertexBufferDesc, &vertexBufferInitData, vertexBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create VertexBuffer." );
        return false;
    }
    return true;
}
