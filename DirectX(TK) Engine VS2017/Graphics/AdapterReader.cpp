#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

std::vector<AdapterData> AdapterReader::GetAdapters()
{
    if (adapters.size() > 0) // if already initialized
        return adapters;

    Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

    // Create DXGIFactory 
    HRESULT hr = CreateDXGIFactory( __uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()) );
    if (FAILED( hr ))
    {
        ErrorLogger::Log( hr, "Failed to create DXGI factory for enumerating adpaters" );
        exit( -1 );
    }

    UINT i = 0;
    IDXGIAdapter* pAdapter;
    while (SUCCEEDED(pFactory->EnumAdapters( i, &pAdapter )))
    {
        adapters.push_back( pAdapter );
        ++i;
    }

    return adapters;
}

AdapterData::AdapterData( IDXGIAdapter * pAdapter ) : pAdapter{ pAdapter }
{
    HRESULT hr = pAdapter->GetDesc( &description );
    if (FAILED( hr ))
    {
        ErrorLogger::Log( hr, "Failed to get description for IDXGIAdapter." );
    }
}
