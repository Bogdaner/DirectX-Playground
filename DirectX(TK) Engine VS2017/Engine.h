#pragma once

#include "WindowContainer.h"

class Engine : private WindowContainer
{
public:
    bool Initialize(HINSTANCE hInstance, const std::string& window_title, const std::string& window_class, unsigned const int width, unsigned const int height);
    bool ProcessMessages();
    void Update();
protected:
private:
};
