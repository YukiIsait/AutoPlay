#include <Windows.h>

#include "Defs.h"
#include "Tools.h"

HMODULE glhInstance;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            Tool_MaskImm(hWnd, TRUE);
            Tool_HideOrShowTaskBar(TRUE);
            Tool_PlayResource(glhInstance, DEFS_RESID_WAVE1);
            SetTimer(hWnd, DEFS_RESID_TIMER1, 1, NULL);
            break;
        }
        case WM_TIMER: {
            POINT point;
            GetCursorPos(&point);
            MoveWindow(hWnd, point.x - DEFS_WINDOW_WIDTH / 2, point.y - DEFS_WINDOW_HEIGHT / 2,
                       DEFS_WINDOW_WIDTH, DEFS_WINDOW_HEIGHT, FALSE);
            INPUT inputs[2] = {0};
            Tool_MakeHotkeyInputArray(1, inputs, VK_VOLUME_UP);
            SendInput(2, inputs, sizeof(INPUT));
            break;
        }
        case WM_KEYDOWN: {
            static int qClickNum = 0;
            switch (wParam) {
                case 'Q': {
                    qClickNum++;
                    break;
                }
                case VK_RETURN: {
                    if (qClickNum >= 5) { PostMessageW(hWnd, WM_CLOSE, 0, 0); }
                    break;
                }
            }
            break;
        }
        case WM_DESTROY: {
            Tool_HideOrShowTaskBar(FALSE);
            Tool_MaskImm(hWnd, FALSE);
            KillTimer(hWnd, DEFS_RESID_TIMER1);
            PostQuitMessage(0);
            break;
        }
        case WM_SYSCOMMAND: {
            if (wParam == SC_CLOSE) break;
        }
        case WM_QUERYENDSESSION: {
            return FALSE;
        }
        default: {
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    glhInstance = hInstance;

    RECT screenRect;
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &screenRect, 0);
    LONG screenWidth = screenRect.right - screenRect.left;
    LONG screenHeight = screenRect.bottom - screenRect.top;

    WNDCLASSW wndClass = {.hInstance = hInstance,
                          .hbrBackground = CreateSolidBrush(RGB(0x00, 0x00, 0x00)),
                          .lpszClassName = DEFS_WINDOW_CLASS_NAME,
                          .lpfnWndProc = WindowProc};
    RegisterClassW(&wndClass);

    HWND hWnd = CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED, DEFS_WINDOW_CLASS_NAME,
                                DEFS_APPLICATION_NAME, WS_POPUP, screenWidth / 2 - DEFS_WINDOW_WIDTH / 2,
                                screenHeight / 2 - DEFS_WINDOW_HEIGHT / 2, DEFS_WINDOW_WIDTH,
                                DEFS_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    // SetLayeredWindowAttributes(hWnd, 0, 50, LWA_ALPHA);  // Test

    ShowWindow(hWnd, SW_SHOW);

    MSG msg;
    INT ret;
    while ((ret = GetMessageW(&msg, NULL, 0, 0)) != 0 && ret != -1) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return msg.wParam;
}
