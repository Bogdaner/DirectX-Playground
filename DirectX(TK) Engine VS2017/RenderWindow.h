#pragma once

#include <Windows.h>

#include "ErrorLogger.h"

class WindowContainer; // forward definition

class RenderWindow
{
public:
    RenderWindow();
    bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& windowTitle, 
        const std::string& windowClass, const unsigned int width, const unsigned int height);

    bool ProcessMessages();
    HWND GetHWND() const;
    ~RenderWindow();
protected:
private:
    HWND Create(WindowContainer* pWindowContainer);
    void RegisterWindowClass();
    HWND handle = NULL;
    HINSTANCE hInstance = NULL;
    std::string windowTitle;
    std::wstring windowTitleWide;
    std::string windowClass;
    std::wstring windowClassWide;
    unsigned int width;
    unsigned int height;
};