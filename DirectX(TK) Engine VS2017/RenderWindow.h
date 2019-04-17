#pragma once
#include "ErrorLogger.h"

#include <Windows.h>

class WindowContainer; // forward definition

class RenderWindow
{
public:
    RenderWindow();
    bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& window_title, 
        const std::string& window_class, const unsigned int width, const unsigned int height);

    bool ProcessMessages();
    ~RenderWindow();
protected:
private:
    HWND Create(WindowContainer* pWindowContainer);
    void RegisterWindowClass();
    HWND handle = NULL;
    HINSTANCE hInstance = NULL;
    std::string window_title;
    std::wstring window_title_wide;
    std::string window_class;
    std::wstring window_class_wide;
    unsigned int width;
    unsigned int height;
};