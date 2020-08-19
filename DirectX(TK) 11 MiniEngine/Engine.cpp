#include "Engine.h"
//#define TEST

bool Engine::Initialize( HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, unsigned const int width, unsigned const int height )
{
    timer.Start();
    //keyboard.EnableAutoRepeatChars();
    if ( !renderWindow.Initialize( this, hInstance, windowTitle, windowClass, width, height ) )
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
    const float dt = static_cast<float>( timer.GetMilisecondsElapsed() );
    timer.Restart();
    const float cameraSpeed = 0.005f;

    while ( !keyboard.CharBufferIsEmpty() )
    {
        const unsigned char ch = keyboard.ReadChar();
    }

    while ( !keyboard.KeyBufferIsEmpty() )
    {
        const unsigned char ch = keyboard.ReadKey().GetKeyCode();
    }

    while ( !mouse.EventBufferIsEmpty() )
    {
        MouseEvent me = mouse.ReadEvent();

        if ( mouse.IsRightDown() && me.GetType() == MouseEvent::EventType::RAW_MOVE )
        {
            gfx.camera.AdjustRotation( static_cast<float>( me.GetPosY() ) * 0.005f, static_cast<float>( me.GetPosX() ) * 0.005f, 0.0f );
        }
    }

    if ( keyboard.KeyIsPressed( 'W' ) )
    {
        gfx.camera.AdjustPosition( gfx.camera.GetForwardVector() * cameraSpeed * dt );
    }
    if ( keyboard.KeyIsPressed( 'S' ) )
    {
        gfx.camera.AdjustPosition( gfx.camera.GetBackwardVector() * cameraSpeed * dt );
    }
    if ( keyboard.KeyIsPressed( 'A' ) )
    {
        gfx.camera.AdjustPosition( gfx.camera.GetLeftVector() * cameraSpeed * dt );
    }
    if ( keyboard.KeyIsPressed( 'D' ) )
    {
        gfx.camera.AdjustPosition( gfx.camera.GetRightVector() * cameraSpeed * dt );
    }
    if ( keyboard.KeyIsPressed( VK_SPACE ) )
    {
        gfx.camera.AdjustPosition( 0.0f, cameraSpeed * dt, 0.0f );
    }
    if ( keyboard.KeyIsPressed( 'Z' ) )
    {
        gfx.camera.AdjustPosition( 0.0f, -cameraSpeed * dt, 0.0f );
    }
}

void Engine::RenderFrame()
{
    gfx.RenderFrame();
}
