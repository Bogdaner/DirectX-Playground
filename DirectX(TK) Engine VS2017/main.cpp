#include<Windows.h>
#include "Engine.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR     lpCmdLine,
    _In_ int       nShowCmd
)
{
    Engine engine;
    if ( engine.Initialize( hInstance, "Window", "Class", 800u, 600u ) )
    {
        while ( engine.ProcessMessages() == true )
        {
            engine.Update();
            engine.RenderFrame();
        }
    }
    return 0;
}