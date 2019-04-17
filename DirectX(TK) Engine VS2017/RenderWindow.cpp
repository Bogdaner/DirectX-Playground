#include "WindowContainer.h"

RenderWindow::RenderWindow() : handle{ NULL }, hInstance{ NULL }, window_title{ "" }, window_title_wide{ L"" }, window_class{ "" }, window_class_wide{ L"" },
                               width{ 0u }, height{ 0u }
{

}

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, const std::string& window_title, const std::string& window_class, unsigned const int width, unsigned const int height)
{
    this->hInstance = hInstance;
    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(window_title);
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(window_class);
    this->width = width;
    this->height = height;

    RegisterWindowClass();

    handle = Create(pWindowContainer);
    if (handle == NULL)
    {
        ErrorLogger::Log(GetLastError(), "CreateWindowEx failed for window: " + window_title);
        return false;
    }

    // Bring the window up on the screen and set it as main focus
    ShowWindow(handle, SW_SHOW);
    SetForegroundWindow(handle);
    SetFocus(handle);

    return true;
}

bool RenderWindow::ProcessMessages()
{
    MSG msg = {};
    //ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    switch (msg.message)
    {
        case WM_NULL: // when window is closed we will recive NULL messages
        {
            if (!IsWindow(handle))
            {
                handle = NULL;
                UnregisterClass(window_class_wide.c_str(), hInstance);
                return false;
            }
        }
    }

    return true;
}

RenderWindow::~RenderWindow()
{
    if (handle != NULL)
    {
        UnregisterClass(window_class_wide.c_str(), hInstance);
        DestroyWindow(handle);
    }
}

HWND RenderWindow::Create(WindowContainer* pWindowContainer)
{
    return CreateWindowEx(0,                      //Extended Windows style - we are using the default. For other options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ff700543(v=vs.85).aspx
        this->window_class_wide.c_str(),          //Window class name
        this->window_title_wide.c_str(),          //Window Title
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //Windows style - See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms632600(v=vs.85).aspx
        0,                                        //Window X Position
        0,                                        //Window Y Position
        this->width,                              //Window Width
        this->height,                             //Window Height
        NULL,                                     //Handle to parent of this window. Since this is the first window, it has no parent window.
        NULL,                                     //Handle to menu or child window identifier. Can be set to NULL and use menu in WindowClassEx if a menu is desired to be used.
        this->hInstance,                          //Handle to the instance of module to be used with this window
        pWindowContainer);                        //Param to create window
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }

        // All other messages
        default:
        {
            // retrieve ptr to window class
            WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            // forward message to window class handler
            return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_NCCREATE:
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            WindowContainer * pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
            if (pWindow == nullptr) //Sanity check
            {
                ErrorLogger::Log("Critical Error: Pointer to window container is null during WM_NCCREATE.");
                exit(-1);
            }
            // send ptr to window class (we are retriving it in HandleMsgRedirect func ^^)
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
            SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
            return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;                                    //Our Window Class (This has to be filled before our window can be created) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms633577(v=vs.85).aspx
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
    wc.lpfnWndProc = HandleMessageSetup;            //Pointer to Window Proc function for handling messages from this window
    wc.cbClsExtra = 0;                                //# of extra bytes to allocate following the window-class structure. We are not currently using this.
    wc.cbWndExtra = 0;                                //# of extra bytes to allocate following the window instance. We are not currently using this.
    wc.hInstance = this->hInstance;                    //Handle to the instance that contains the Window Procedure
    wc.hIcon = NULL;                                //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
    wc.hIconSm = NULL;                                //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);        //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
    wc.hbrBackground = NULL;                        //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
    wc.lpszMenuName = NULL;                            //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
    wc.lpszClassName = this->window_class_wide.c_str(); //Pointer to null terminated string of our class name for this window.
    wc.cbSize = sizeof(WNDCLASSEX);                    //Need to fill in the size of our struct for cbSize
    RegisterClassEx(&wc);                            // Register the class so that it is usable.
}

