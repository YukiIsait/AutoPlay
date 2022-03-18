#include "Tools.h"
#include <Windows.h>
#include <imm.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")

void Tool_MakeHotkeyInputArray(UINT numOfHotkey, INPUT* inputArray, ...) {
    UINT arrayLength = numOfHotkey * 2;
    va_list vaPtr;
    va_start(vaPtr, inputArray);
    for (UINT i = 0; i < arrayLength; i++) {
        inputArray[i].type = INPUT_KEYBOARD;
        if (i < numOfHotkey) {
            inputArray[i].ki.wVk = inputArray[arrayLength - 1 - i].ki.wVk = va_arg(vaPtr, WORD);
        } else {
            inputArray[i].ki.dwFlags = KEYEVENTF_KEYUP;
        }
    }
    va_end(vaPtr);
}

BOOL Tool_HideOrShowTaskBar(BOOL isHide) {
    BOOL bRet = FALSE;

    HWND hTray = FindWindowW(L"Shell_TrayWnd", NULL);
    if (!hTray) return FALSE;

    if (isHide) {
        bRet = ShowWindow(hTray, SW_HIDE);
    } else {
        bRet = ShowWindow(hTray, SW_SHOW);
    }
    return bRet;
}

BOOL Tool_PlayResource(HMODULE hModule, int resId) {
    return PlaySoundW((LPCWSTR) resId, hModule, SND_RESOURCE | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
}

void Tool_MaskImm(HWND hWnd, BOOL isMask) {
    HIMC hImc = ImmGetContext(hWnd);
    if (isMask) {
        ImmAssociateContext(hWnd, NULL);
    } else {
        ImmAssociateContext(hWnd, hImc);
    }
    ImmReleaseContext(hWnd, hImc);
}
