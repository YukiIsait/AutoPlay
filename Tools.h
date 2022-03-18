#pragma once

#include <Windows.h>

void Tool_MakeHotkeyInputArray(UINT numOfHotkey, INPUT* inputArray, ...);
BOOL Tool_HideOrShowTaskBar(BOOL isHide);
BOOL Tool_PlayResource(HMODULE hModule, int resId);
void Tool_MaskImm(HWND hWnd, BOOL isMask);
