#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <memory>

class IndexBuffer
{
public:
    IndexBuffer() {}

    ID3D11Buffer* Get() const
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    UINT GetBufferSize() const
    {
        return bufferSize;
    }

    HRESULT Initialize( ID3D11Device* device, DWORD* data, UINT numIndices )
    {
        bufferSize = numIndices;

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory( &indexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof( DWORD ) * numIndices;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA indexBufferInitData;
        ZeroMemory( &indexBufferInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );
        indexBufferInitData.pSysMem = data;

        HRESULT hr = device->CreateBuffer( &indexBufferDesc, &indexBufferInitData, buffer.GetAddressOf() );
        return hr;
    }
protected:
private:
    IndexBuffer( const IndexBuffer& rhs );
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    UINT bufferSize = 0;
};