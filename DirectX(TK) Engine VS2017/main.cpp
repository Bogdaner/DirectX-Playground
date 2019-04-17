#include<Windows.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

#include "Engine.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR     lpCmdLine,
    _In_ int       nShowCmd
)
{
    Engine engine;
    engine.Initialize(hInstance, "Window", "Class", 800u, 600u);

    while (engine.ProcessMessages() == true)
    {
        engine.Update();
    }
    return 0;
}