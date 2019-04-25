#pragma once

#include <d3d11.h>
#include <wrl\client.h>

#include "..//ErrorLogger.h"

class VertexShader
{
public:
    bool Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements );
    ID3D11VertexShader* GetShader() const;
    const BYTE* GetShaderBuffer() const;
    SIZE_T GetShaderBufferSize() const;
    ID3D11InputLayout* GetInputLayout() const;
protected:
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};

class PixelShader
{
public:
    bool Initialize( Microsoft::WRL::ComPtr<ID3D11Device> &device);
    ID3D11PixelShader* GetShader() const;
    const BYTE* GetShaderBuffer() const;
    SIZE_T GetShaderBufferSize() const;
protected:
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
};