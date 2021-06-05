#include "Graphics.h"

Graphics::~Graphics()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

bool Graphics::Initialize( HWND hwnd, const unsigned int width, const unsigned int height )
{
    windowWidth = width;
    windowHeight = height;
    fpsTimer.Start();

    if ( !InitializeDirectX( hwnd ) )
        return false;

    if ( !InitializeShaders() )
        return false;

    if ( !InitializeScene() )
        return false;

    if ( !InitializeImGui( hwnd ) )
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

    const XMMATRIX world = DirectX::XMMatrixIdentity();
    //camera.AdjustPosition( 0.0f, 0.01f, 0.0f );
    //camera.SetLookAtPos( XMFLOAT3{ 0.0f, 0.0f, 0.0f } );
    //camera.AdjustRotation( 0.0f, 0.0f, 0.0f );

    constantBuffer.data.mat = world * camera.GetViewMatrix() * camera.GetProjectionMatrix();

    // DirectX Math library stores matrices in row major format when HLSL uses column major for faster computations
    constantBuffer.data.mat = XMMatrixTranspose( constantBuffer.data.mat );
    constantBuffer.ApplyChanges();

    deviceContext->VSSetConstantBuffers( 0, 1, constantBuffer.GetAddressOf() );

    UINT offset = 0;
    deviceContext->PSSetShaderResources( 0, 1, texture.GetAddressOf() );
    deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.GetStridePtr(), &offset );
    deviceContext->IASetIndexBuffer( indicesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0 );
    deviceContext->DrawIndexed( 6, 0, 0 );

    // Draw Text
    static unsigned int fpsCounter;
    static std::string fpsStr{"FPS: 0"};
    fpsCounter += 1;
    if ( fpsTimer.GetMilisecondsElapsed() >= 1000.0f)
    {
        fpsTimer.Restart();
        fpsStr = "FPS: " + std::to_string( fpsCounter );
        fpsCounter = 0;
    }

    spriteBatch->Begin();
    spriteFont->DrawString( spriteBatch.get(), 
                            fpsStr.c_str(), 
                            DirectX::XMFLOAT2( 0.0f, 0.0f ), 
                            DirectX::Colors::White, 0.0f, 
                            DirectX::XMFLOAT2( 0.0f, 0.0f ), 
                            DirectX::XMFLOAT2( 1.0f, 1.0f ) 
    );

    spriteBatch->End();

    RenderImGuiFrame();

    swapchain->Present( 0, NULL ); // first argument vsync on/off
}

void Graphics::RenderImGuiFrame() const
{
    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    ImGui::ShowDemoWindow( &show_demo_window );

    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
}

bool Graphics::InitializeDirectX( HWND hwnd )
{
    if ( !SetupDeviceAndSwapchain( hwnd ) )
        return false;

    if ( !SetupZBuffer() )
        return false;

    if ( !SetupRasterizer() )
        return false;

    if ( !SetupSamplerState() )
        return false;

    InitializeFonts();
    return true;
}

bool Graphics::InitializeShaders()
{
    std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
    if ( IsDebuggerPresent() == TRUE )
    {
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
        shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
        shaderfolder = L"..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
        shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
        shaderfolder = L"..\\Release\\";
#endif
#endif
    }

    D3D11_INPUT_ELEMENT_DESC layout[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    const UINT numElements = ARRAYSIZE( layout );

    if ( !vertexShader.Initialize( device, shaderfolder + L"vertexshader.cso", layout, numElements ) )
        return false;

    if ( !pixelShader.Initialize( device,  shaderfolder + L"pixelshader.cso") )
        return false;

    return true;
}

bool Graphics::InitializeScene()
{
    std::vector<Vertex> v;
    // Geometry def
    v.push_back( { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f} ); // Bottom left
    v.push_back( { -1.0f, 1.0f, 0.0f, 0.0f, 0.0f } ); // Top left
    v.push_back( { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f} ); // Bottom right
    v.push_back( { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f } ); // Top right

    std::vector<DWORD> indices{ 0, 1, 2, 1, 3, 2 };

    // Vertex Buffer Setup
    HRESULT hr = vertexBuffer.Initialize( device.Get(), v.data(), static_cast<UINT>( v.size() ) );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create VertexBuffer." );
        return false;
    }

    // Index Buffer Setup
    hr = indicesBuffer.Initialize(device.Get(), indices.data(), static_cast<UINT>( indices.size() ) );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create IndexBuffer." );
        return false;
    }

    // Constant Buffer Setup
    hr = constantBuffer.Initialize( device.Get(), deviceContext.Get() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to initialize constant buffer." );
        return false;
    }
    
    // Texture loading
    hr = DirectX::CreateWICTextureFromFile( device.Get(), L"Data\\Textures\\hello.png", nullptr, texture.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to load a texture from file." );
        return false;
    }

    // Camera Init
    camera.SetPosition( 0.0f, 0.0f, -2.0f );
    camera.SetProjectionValues(
        90.0f,
        static_cast<float>( windowWidth ) / static_cast<float>( windowHeight ),
        0.1f,
        1000.0f );

    return true;
}

bool Graphics::InitializeImGui( HWND hwnd )
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    const bool imGuiWin32Init = ImGui_ImplWin32_Init( hwnd );
    const bool imGuiDX11Init = ImGui_ImplDX11_Init( device.Get(), deviceContext.Get() );

    return imGuiWin32Init && imGuiDX11Init;
}

void Graphics::InitializeFonts()
{
    // https://github.com/microsoft/DirectXTK/wiki/Drawing-text
    spriteBatch = std::make_unique<DirectX::SpriteBatch>( deviceContext.Get() );
    spriteFont = std::make_unique<DirectX::SpriteFont>( device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont" );
}

bool Graphics::SetupDeviceAndSwapchain( const HWND hwnd )
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

    scd.BufferDesc.Width = windowWidth;
    scd.BufferDesc.Height = windowHeight;
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

    hr = device->CreateRenderTargetView( backBuffer.Get(), NULL, renderTargetView.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( "Failed to create render target view" );
        return false;
    }

    return true;
}

bool Graphics::SetupZBuffer()
{
    //Describe our Depth/Stencil Buffer
    D3D11_TEXTURE2D_DESC depthStencilDescTex;
    depthStencilDescTex.Width = windowWidth;
    depthStencilDescTex.Height = windowHeight;
    depthStencilDescTex.MipLevels = 1;
    depthStencilDescTex.ArraySize = 1;
    depthStencilDescTex.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDescTex.SampleDesc.Count = 1;
    depthStencilDescTex.SampleDesc.Quality = 0;
    depthStencilDescTex.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDescTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDescTex.CPUAccessFlags = 0;
    depthStencilDescTex.MiscFlags = 0;

    HRESULT hr = device->CreateTexture2D( &depthStencilDescTex, NULL, depthStencilBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create depth stencil buffer." );
        return false;
    }

    hr = device->CreateDepthStencilView( depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf() );
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

    hr = device->CreateDepthStencilState( &depthStencilDesc, depthStencilState.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create depth stencil state." );
        return false;
    }

    return true;
}

bool Graphics::SetupRasterizer()
{
    // RASTERIZER SETUP
    D3D11_VIEWPORT viewport;
    ZeroMemory( &viewport, sizeof( D3D11_VIEWPORT ) );

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast< float >( windowWidth );
    viewport.Height = static_cast< float >( windowHeight );
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
