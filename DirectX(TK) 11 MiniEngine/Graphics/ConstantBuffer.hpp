#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <memory>

#include "ConstantBufferTypes.h"
#include "..\\ErrorLogger.h"

template<class T>
class ConstantBuffer
{
public:
    T data;

    ID3D11Buffer* Get() const
    {
        return constantBuffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return constantBuffer.GetAddressOf();
    }

    HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    {
        this->deviceContext = deviceContext;

        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = static_cast<UINT>( sizeof( T ) + ( 16 - ( sizeof( T ) % 16 ) ) );;
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        HRESULT hr = device->CreateBuffer( &cbDesc, NULL, constantBuffer.GetAddressOf() );

        return hr;
    }

    bool ApplyChanges()
    {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = this->deviceContext->Map( constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
        if ( FAILED( hr ) )
        {
            ErrorLogger::Log( hr, "Failed to map constant buffer." );
            return false;
        }
        CopyMemory( mappedResource.pData, &data, sizeof( T ) );
        this->deviceContext->Unmap( constantBuffer.Get(), 0 );

        return true;
    }
protected:
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
    ID3D11DeviceContext* deviceContext = nullptr;
};