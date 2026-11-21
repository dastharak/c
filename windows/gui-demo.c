/*
gcc -std=c17 -Wall -Wextra test_gui.c -o test_gui.exe -mwindows
*/

#include <windows.h>

#define IDC_BTN_CLICK 101
#define IDC_LBL_TEXT  102

static int click_count = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateWindowExA(
                0, "STATIC", "Click the button to test MinGW-w64 GUI.",
                WS_CHILD | WS_VISIBLE,
                20, 20, 320, 24,
                hwnd, (HMENU)IDC_LBL_TEXT, GetModuleHandleA(NULL), NULL
            );

            CreateWindowExA(
                0, "BUTTON", "Click Me",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                20, 60, 120, 32,
                hwnd, (HMENU)IDC_BTN_CLICK, GetModuleHandleA(NULL), NULL
            );
            return 0;
        }

        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_BTN_CLICK && HIWORD(wParam) == BN_CLICKED) {
                char buf[64];
                click_count++;
                wsprintfA(buf, "Button clicked %d time%s.",
                          click_count, click_count == 1 ? "" : "s");
                SetDlgItemTextA(hwnd, IDC_LBL_TEXT, buf);
            }
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdLine, int showCmd) {
    (void)hPrev; (void)cmdLine;

    const char *CLASS_NAME = "MinGWTestWindow";

    WNDCLASSA wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "MinGW-w64 GUI Test",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 380, 160,
        NULL, NULL, hInst, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, showCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return (int)msg.wParam;
}
