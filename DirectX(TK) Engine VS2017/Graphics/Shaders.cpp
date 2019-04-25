#include "Shaders.h"
#include "..//Shaders/VertexShader_Header.h"
#include "..//Shaders//PixelShader_Header.h"

// VertexShader Setup
bool VertexShader::Initialize( Microsoft::WRL::ComPtr<ID3D11Device>& device, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements )
{
    HRESULT hr = device->CreateVertexShader( 
        vertexshader_array, 
        sizeof( vertexshader_array ),
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
        GetShaderBuffer(),
        GetShaderBufferSize(),
        inputLayout.GetAddressOf() );

    if ( FAILED( hr ) )
    {
        ErrorLogger::Log( hr, "Failed to create Input Layout" );
        return false;
    }

    return true;
}

ID3D11VertexShader* VertexShader::GetShader() const
{
    return shader.Get();
}


const BYTE* VertexShader::GetShaderBuffer() const
{
    return vertexshader_array;
}

SIZE_T VertexShader::GetShaderBufferSize() const
{
    return sizeof(vertexshader_array);
}

ID3D11InputLayout * VertexShader::GetInputLayout() const
{
    return inputLayout.Get();
}
// --------------------------------------------------------------------------------------------------------------------------------

//PixelShader Setup

bool PixelShader::Initialize( Microsoft::WRL::ComPtr<ID3D11Device>& device )
{
    HRESULT hr = device->CreatePixelShader(
        pixelshader_array,
        sizeof( pixelshader_array ),
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

const BYTE * PixelShader::GetShaderBuffer() const
{
    return pixelshader_array;
}

SIZE_T PixelShader::GetShaderBufferSize() const
{
    return sizeof( pixelshader_array );
}
