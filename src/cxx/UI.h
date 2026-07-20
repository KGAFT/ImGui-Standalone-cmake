#ifndef UI_H
#define UI_H

#include <string>

#include "pch.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class UI
{
private:
	static ID3D11Device* pd3dDevice;
	static ID3D11DeviceContext* pd3dDeviceContext;
	static IDXGISwapChain* pSwapChain;
	static ID3D11RenderTargetView* pMainRenderTargetView;

	static bool CreateDeviceD3D(HWND hWnd);
	static void CleanupDeviceD3D();
	static void CreateRenderTarget();
	static void CleanupRenderTarget();
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void ForceTopMost(HWND hWnd);
	static void EnableClickThrough(HWND hWnd);
public:
	static HMODULE hCurrentModule;

	static void Initialize();
	static void PreRenderEvent();
	static void PostRenderEvent();
	static void Destroy();
	static bool NeedExit();
	static std::string RandomString(uint32_t length);
	static void EnableClickThrough(bool state);
	static uint32_t Width();
	static uint32_t Height();
};

#endif
