#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <memory>

template<class T>
class VertexBuffer
{
public:
    VertexBuffer() {}

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

    UINT GetStride() const
    {
        return *stride;
    }

    const UINT* GetStridePtr() const
    {
        return stride.get();
    }

    HRESULT Initialize( ID3D11Device* device, T* data, UINT numVertices )
    {
        bufferSize = numVertices;
        stride = std::make_unique<UINT>( static_cast<UINT>( sizeof( T ) ) );

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory( &vertexBufferDesc, sizeof( D3D11_BUFFER_DESC ) );
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof( T ) * numVertices;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferInitData;
        ZeroMemory( &vertexBufferInitData, sizeof( D3D11_SUBRESOURCE_DATA ) );
        vertexBufferInitData.pSysMem = data;

        HRESULT hr = device->CreateBuffer( &vertexBufferDesc, &vertexBufferInitData, buffer.GetAddressOf() );
        return hr;
    }
protected:
private:
    VertexBuffer( const VertexBuffer<T>& rhs );
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    std::unique_ptr<UINT> stride = nullptr;
    UINT bufferSize = 0;
};