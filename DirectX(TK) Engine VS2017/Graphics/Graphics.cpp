#include "Graphics.h"

bool Graphics::Initialize( HWND hwnd, const unsigned int width, const unsigned int height )
{
    if (!InitializeDirectX( hwnd, width, height ))
    {
        return false;
    }

    return true;
}

void Graphics::RenderFrame()
{
    const float bgcolor[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    deviceContext->ClearRenderTargetView( renderTargetView.Get(), bgcolor );
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

    return true;
}