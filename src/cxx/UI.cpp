#include "UI.h"
#include <optional>
#include "Drawing.h"
#include <dwmapi.h>
#include <random>
ID3D11Device *UI::pd3dDevice = nullptr;
ID3D11DeviceContext *UI::pd3dDeviceContext = nullptr;
IDXGISwapChain *UI::pSwapChain = nullptr;
ID3D11RenderTargetView *UI::pMainRenderTargetView = nullptr;
HMODULE UI::hCurrentModule = nullptr;

bool UI::CreateDeviceD3D(HWND hWnd) {
    DXGI_SWAP_CHAIN_DESC sd{};
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2U;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    //  sd.BufferDesc.RefreshRate.Numerator = 60U;
    //sd.BufferDesc.RefreshRate.Denominator = 1U;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;

    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel = {};
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0,};
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray,
                                      2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel,
                                      &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CreateRenderTarget() {
    ID3D11Texture2D *pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr) {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void UI::CleanupRenderTarget() {
    if (pMainRenderTargetView) {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void UI::CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (pSwapChain) {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }
    if (pd3dDeviceContext) {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }
    if (pd3dDevice) {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return 0L;

    switch (msg) {
        case WM_SIZE:
            if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED) {
                CleanupRenderTarget();
                pSwapChain->ResizeBuffers(0, (UINT) LOWORD(lParam), (UINT) HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
            }
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports) {
                const RECT *suggested_rect = (RECT *) lParam;
                ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top,
                               suggested_rect->right - suggested_rect->left,
                               suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        default:
            break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void UI::ForceTopMost(HWND hWnd) {
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void UI::EnableClickThrough(HWND hWnd) {
    auto ex_style = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);
    SetWindowLongPtrW(hWnd, GWL_EXSTYLE, ex_style | WS_EX_TRANSPARENT | WS_EX_LAYERED);
}

static HWND hwnd;
static int monitor_width;
static int monitor_height;
static WNDCLASSEXW wc;
bool bDone = false;

static std::wstring class_name;


std::wstring to_wstring(const std::string &str) {
    if (str.empty()) return L"";

    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int) str.size(),
        nullptr, 0);

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(
        CP_UTF8, 0, str.c_str(), (int) str.size(),
        &wstr[0], size_needed);

    return wstr;
}


void UI::Initialize() {
    class_name = to_wstring(UI::RandomString(8));

    ImGui_ImplWin32_EnableDpiAwareness();
    wc = {
        sizeof(WNDCLASSEXW), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, 0L, nullptr, nullptr, nullptr, nullptr,
        class_name.c_str(), nullptr
    };
    ::RegisterClassExW(&wc);

    POINT pt = {0, 0};
    const HMONITOR monitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    monitor_width = info.rcMonitor.right - info.rcMonitor.left;
    monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
    hwnd = ::CreateWindowExW(WS_EX_TOPMOST  | WS_EX_LAYERED | WS_EX_TRANSPARENT, wc.lpszClassName,
                             class_name.c_str(), WS_POPUP, info.rcMonitor.left, info.rcMonitor.top,
                             monitor_width, monitor_height, NULL, NULL, wc.hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        RECT windowRect;
        GetWindowRect(hwnd, &windowRect);
        POINT diff{};
        ClientToScreen(hwnd, &diff);
        const MARGINS margins = {
            windowRect.left + (diff.x - windowRect.left),
            windowRect.top + (diff.y - windowRect.top),
            clientRect.right,
            clientRect.bottom,
        };
        DwmExtendFrameIntoClientArea(hwnd, &margins);
    }

    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();

    if (monitor_height > 1080) {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * fScale;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
    }

    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    bDone = false;
    Drawing::Init();
}

static bool g_ClickThrough = true;


void SetClickThrough(HWND hWnd, bool enable)
{
    LONG_PTR exStyle = GetWindowLongPtrW(hWnd, GWL_EXSTYLE);

    if (enable)
    {
        exStyle |= WS_EX_TRANSPARENT;
    }
    else
    {
        exStyle &= ~WS_EX_TRANSPARENT;
    }

    SetWindowLongPtrW(hWnd, GWL_EXSTYLE, exStyle);

    // Force Windows to re-evaluate hit testing
    SetWindowPos(
        hWnd,
        nullptr,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
    );
}


void UI::PreRenderEvent() {
    
    UI::ForceTopMost(hwnd);
    //UI::EnableClickThrough(hwnd);
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            bDone = true;
    }

    if (GetAsyncKeyState(VK_END) & 1)
        bDone = true;


    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void UI::PostRenderEvent() {
    ImGui::Render();
    const float clear_color_with_alpha[4] = {0, 0, 0, 0};
    pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
    pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    pSwapChain->Present(1, 0);
}

void UI::Destroy() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

bool UI::NeedExit() {
    return bDone;
}

std::string UI::RandomString(uint32_t length) {
    const std::string chars =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, chars.size() - 1);

    std::string result;
    result.reserve(length);

    for (std::size_t i = 0; i < length; ++i) {
        result += chars[dist(generator)];
    }

    return result;
}

void UI::EnableClickThrough(bool state)
{
    SetClickThrough(hwnd, state);
}


uint32_t UI::Width() {
    return monitor_width;
}

uint32_t UI::Height() {
    return monitor_height;
}
