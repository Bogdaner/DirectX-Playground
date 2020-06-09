#include "Engine.h"
//#define TEST

bool Engine::Initialize( HINSTANCE hInstance, const std::string& window_title, const std::string& window_class, unsigned const int width, unsigned const int height )
{
    //keyboard.EnableAutoRepeatChars();
    if ( !renderWindow.Initialize( this, hInstance, window_title, window_class, width, height ) )
        return false;

    if ( !gfx.Initialize( renderWindow.GetHWND(), width, height ) )
        return false; 

    return true;
}

bool Engine::ProcessMessages()
{
    return renderWindow.ProcessMessages();
}

void Engine::Update()
{
    while ( !keyboard.CharBufferIsEmpty() )
    {
        const unsigned char ch = keyboard.ReadChar();
#ifdef TEST
        // Testing Code:
        std::string s = "Char: ";
        s += ch;
        s += '\n';
        OutputDebugStringA( s.c_str() );
#endif // TEST
    }

    while ( !keyboard.KeyBufferIsEmpty() )
    {
        const unsigned char ch = keyboard.ReadKey().GetKeyCode();
#ifdef TEST
        std::string s = "Keycode: ";
        s += ch;
        s += '\n';
        OutputDebugStringA( s.c_str() );
#endif // TEST
    }

    while ( !mouse.EventBufferIsEmpty() )
    {
        MouseEvent me = mouse.ReadEvent();
#ifdef TEST
        if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
        {
            std::string s = "X: " + std::to_string( me.GetPosX() );
            s += " ";
            s += "Y: " + std::to_string( me.GetPosY() );
            s += '\n';
            OutputDebugStringA( s.c_str() );
        }
#endif // TEST
    }
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
