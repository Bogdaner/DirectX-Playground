#include "Graphics.h"

bool Graphics::Initialize( HWND hwnd, const int width, const int height )
{
    if (!InitializeDirectX( hwnd, width, height ))
    {
        return false;
    }

    return true;
}

bool Graphics::InitializeDirectX( HWND hwnd, const int width, const int height )
{
    std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

    return true;
}