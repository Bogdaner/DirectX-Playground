#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, const std::string & window_title, const std::string & window_class, unsigned const int width, unsigned const int height)
{
    //keyboard.EnableAutoRepeatChars();
    return render_window.Initialize(this, hInstance, window_title, window_class, width, height);
}

bool Engine::ProcessMessages()
{
    return render_window.ProcessMessages();
}

void Engine::Update()
{
    while (!keyboard.CharBufferIsEmpty())
    {
        const unsigned char ch = keyboard.ReadChar();
        // Testing Code:
        std::string s = "Char: ";
        s += ch;
        s += '\n';
        OutputDebugStringA(s.c_str());
    }

    while (!keyboard.KeyBufferIsEmpty())
    {
        const unsigned char ch = keyboard.ReadKey().GetKeyCode();
        // Testing Code:
        std::string s = "Keycode: ";
        s += ch;
        s += '\n';
        OutputDebugStringA(s.c_str());
    }

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent me = mouse.ReadEvent();
        // Testing Code:
        if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
        {
            std::string s = "X: " + std::to_string(me.GetPosX());
            s += " ";
            s += "Y: " + std::to_string(me.GetPosY());
            s += '\n';
            OutputDebugStringA(s.c_str());
        }
    }
}
