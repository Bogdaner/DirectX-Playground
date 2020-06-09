#include "Shaders.h"

// VertexShader Setup
bool VertexShader::Initialize( 
    Microsoft::WRL::ComPtr<ID3D11Device>& device,
    std::wstring shaderPath,
    D3D11_INPUT_ELEMENT_DESC* layoutDesc,
    UINT numElements )
{
    HRESULT hr = D3DReadFileToBlob( shaderPath.c_str(), shaderBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        std::wstring errorMsg = L"Failed to load shader: ";
        errorMsg += shaderPath;
        ErrorLogger::Log( hr, errorMsg );
        return false;
    }

    hr = device->CreateVertexShader( 
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        nullptr, 
        shader.GetAddressOf() 
    );

    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create vertex shader." );
        return false;
    }

    hr = device->CreateInputLayout(
        layoutDesc,
        numElements,
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        inputLayout.GetAddressOf() );

    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create Input Layout." );
        return false;
    }

    return true;
}

ID3D11VertexShader* VertexShader::GetShader() const
{
    return shader.Get();
}

ID3D10Blob* VertexShader::GetShaderBuffer() const
{
    return shaderBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() const
{
    return inputLayout.Get();
}


//PixelShader Setup
bool PixelShader::Initialize( Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderPath )
{
    HRESULT hr = D3DReadFileToBlob( shaderPath.c_str(), shaderBuffer.GetAddressOf() );
    if ( FAILED( hr ) )
    {
        std::wstring errorMsg = L"Failed to load shader: ";
        errorMsg += shaderPath;
        ErrorLogger::Log( hr, errorMsg );
        return false;
    }

    hr = device->CreatePixelShader(
        shaderBuffer->GetBufferPointer(),
        shaderBuffer->GetBufferSize(),
        nullptr,
        shader.GetAddressOf()
    );

    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create vertex shader." );
        return false;
    }

    return true;
}

ID3D11PixelShader * PixelShader::GetShader() const
{
    return shader.Get();
}

ID3D10Blob* PixelShader::GetShaderBuffer() const
{
    return shaderBuffer.Get();
}
