#include "global.h"
#include "event.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"iphlpapi.lib")


TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("WinormsXP");


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;
    GdiplusStartupInput m_input;
    ULONG_PTR m_token;

    GdiplusStartup(&m_token, &m_input, NULL);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Mega Plane"),
                   MB_OK);

        return 1;
    }

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // WNDWIDTH, WNDHEIGHT: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    hWnd = CreateWindow(
               szWindowClass,
               szTitle,
               WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, // 设置窗口样式，不可改变大小，不可最大化
               CW_USEDEFAULT, CW_USEDEFAULT,
               WNDWIDTH, WNDHEIGHT + TITLEBARHEIGHT,
               NULL,
               NULL,
               hInstance,
               NULL
           );

    if (!hWnd) {
        MessageBox(NULL,
                   _T("Call to CreateWindow failed!"),
                   _T("Mega Plane"),
                   MB_OK);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
               nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(m_token);
    WSACleanup();

    return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        // 初始化
        Init(hWnd, wParam, lParam);
        break;
    case WM_PAINT:
        // 绘制
        Render(hWnd);
        break;
    case WM_KEYDOWN:
        // 键盘按下事件
        KeyDown(hWnd, wParam, lParam);
        break;
    case WM_KEYUP:
        // 键盘松开事件
        KeyUp(hWnd, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        // 鼠标左键事件
        LButtonDown(hWnd, wParam, lParam);
        break;
    case WM_TIMER:
        // 定时器事件
        TimerUpdate(hWnd, wParam, lParam);
        break;
    case WM_DESTROY:
        // 退出窗口事件
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
