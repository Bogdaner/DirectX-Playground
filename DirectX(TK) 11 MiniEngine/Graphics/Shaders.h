#pragma once

#pragma comment(lib, "D3DCompiler.lib")
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl\client.h>

#include "..//ErrorLogger.h"

class VertexShader
{
public:
    bool Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements );
    ID3D11VertexShader* GetShader() const;
    ID3D10Blob* GetShaderBuffer() const;
    ID3D11InputLayout* GetInputLayout() const;
protected:
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
    bool Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device, std::wstring shaderPath );
    ID3D11PixelShader* GetShader() const;
    ID3D10Blob* GetShaderBuffer() const;
protected:
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
    Microsoft::WRL::ComPtr<ID3D10Blob> shaderBuffer;
};