#pragma once

#include "Timer.h"
#include "WindowContainer.h"

class Engine : private WindowContainer
{
public:
    bool Initialize(HINSTANCE hInstance, const std::string& windowTitle, const std::string& windowClass, unsigned const int width, unsigned const int height);
    bool ProcessMessages();
    void Update();
    void RenderFrame();
protected:
private:
    Timer timer;
};
