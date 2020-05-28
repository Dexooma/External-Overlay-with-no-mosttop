#include "overlay.h"
#include <random>

MSG Msg;
uint32_t process_id;
std::string ClassName, WindowName;
int
overlayPosX = -3,
overlayPosY = -1,
overlayWidth = GetSystemMetrics(SM_CXSCREEN) + 54,
overlayHeight = GetSystemMetrics(SM_CYSCREEN) + 45;
LPDIRECT3DDEVICE9 d3dev;
LPDIRECT3D9 d3;
LPD3DXFONT d3Font;
ID3DXLine* d3Line;
D3DPRESENT_PARAMETERS direct3DPresetParams;
static HWND traget_HWND;
static HWND overlay_HWND;

static const char alpha[] =
"0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
const int stringLength = sizeof(alpha) - 1;
char genRandom_char()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 999);
    int rand = dist6(rng);
    return alpha[rand % stringLength];
}

bool Overlay::rendering()
{
    MSG current_message;
    if (PeekMessage(&current_message, nullptr, 0, 0, PM_REMOVE)) {
        if (current_message.message == WM_QUIT) {
            return false;
        }

        TranslateMessage(&current_message);
        DispatchMessage(&current_message);
    }
    SetWindowPos(traget_HWND, overlay_HWND, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return true;
}

RECT Overlay::get_text_dimensions(std::string_view text, LPD3DXFONT font)
{
    RECT r;
    font->DrawTextA(NULL, text.data(), -1, &r, DT_CALCRECT, 0xFFFFFFFF);
    return r;
}

LRESULT CALLBACK MainWndProc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{

    switch (uMsg)
    {
    case WM_CREATE:
        // Initialize the window. 
        return 0;

    case WM_PAINT:
        // Paint the window's client area. 
        return 0;

    case WM_SIZE:
        // Set the size and position of the window. 
        return 0;

    case WM_DESTROY:
        // Clean up window-specific data objects. 
        return 0;

        // 
        // Process other messages. 
        // 

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

auto Overlay::window_d3x_setup() -> void {
    printf("Setup overlay...");
    for (unsigned int i = 0; i < 26; ++i) {
        ClassName += genRandom_char();
        WindowName += genRandom_char();
    }
    traget_HWND = FindWindowA("R6Game", "Rainbow Six"); // Game WindowClass, WindowName
    WNDCLASSEX overlayWindowClass;
    ZeroMemory(&overlayWindowClass, sizeof(WNDCLASSEX)); // Initialises, sets all bits to 0 to remove garbage data
    overlayWindowClass.cbClsExtra = NULL;
    overlayWindowClass.cbWndExtra = NULL;
    overlayWindowClass.cbSize = sizeof(WNDCLASSEX);
    overlayWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    overlayWindowClass.lpfnWndProc = MainWndProc; // Function that will be executed when the window receives a "message" (input). Required! (crashes if set to NULL)
    overlayWindowClass.hInstance = NULL;
    overlayWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    overlayWindowClass.hIcon = LoadIcon(0, IDI_APPLICATION);
    overlayWindowClass.hIconSm = LoadIcon(0, IDI_APPLICATION);
    overlayWindowClass.hbrBackground = (HBRUSH)RGB(0, 0, 0);
    overlayWindowClass.lpszClassName = ClassName.c_str(); // Class name to use with the Windows API function CreateWindow(Ex) to create the window
    overlayWindowClass.lpszMenuName = WindowName.c_str();
    RegisterClassEx(&overlayWindowClass);

    overlay_HWND = CreateWindowEx(NULL, ClassName.c_str(), WindowName.c_str(), WS_POPUP | WS_VISIBLE, overlayPosX, overlayPosY, overlayWidth, overlayHeight, NULL, NULL, NULL, NULL);
    MARGINS margin = { overlayPosX,overlayPosY,overlayWidth,overlayHeight };
    DwmExtendFrameIntoClientArea(overlay_HWND, &margin);
    SetWindowLong(overlay_HWND, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
    ShowWindow(overlay_HWND, SW_SHOW);
    UpdateWindow(overlay_HWND);
   
    d3 = Direct3DCreate9(D3D_SDK_VERSION);
    ZeroMemory(&direct3DPresetParams, sizeof(direct3DPresetParams));
    direct3DPresetParams.Windowed = true;
    direct3DPresetParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    direct3DPresetParams.hDeviceWindow = overlay_HWND;
    direct3DPresetParams.BackBufferFormat = D3DFMT_A8R8G8B8;
    direct3DPresetParams.BackBufferWidth = overlayWidth;
    direct3DPresetParams.BackBufferHeight = overlayHeight;
    direct3DPresetParams.EnableAutoDepthStencil = TRUE;
    direct3DPresetParams.AutoDepthStencilFormat = D3DFMT_D16;
    direct3DPresetParams.MultiSampleType = D3DMULTISAMPLE_NONE;
    direct3DPresetParams.Windowed = TRUE;
    direct3DPresetParams.PresentationInterval = 0x80000000L;

    d3->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, overlay_HWND, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &direct3DPresetParams, &d3dev);
    D3DXCreateFontA(d3dev, 22, NULL, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &d3Font);
    D3DXCreateLine(d3dev, &d3Line);
    //ScreenSize[0] = overlayWidth;
    //ScreenSize[1] = overlayHeight;
    printf("Good\n");
}

auto Overlay::Begin() -> void
{
    d3dev->BeginScene();
    d3dev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
}

auto Overlay::End() -> void
{
    d3dev->EndScene();
    d3dev->Present(NULL, NULL, NULL, NULL);
}

auto Overlay::draw_text(std::string_view text, float x, float y, unsigned long color, bool center, bool outline) ->void {
    if (center) {
        RECT dimensions = get_text_dimensions(text, d3Font);
        x -= (dimensions.right - dimensions.left) / 2;
    }

    auto _text = [&](std::string_view _text, int _x, int _y, unsigned long _color) {
        RECT r{ _x, _y, _x, _y };
        d3Font->DrawTextA(NULL, _text.data(), -5, &r, DT_NOCLIP, _color);
    };
    if (outline) {
        _text(text, x - 1, y, D3DCOLOR_RGBA(1, 1, 1, 255));
        _text(text, x + 1, y, D3DCOLOR_RGBA(1, 1, 1, 255));
        _text(text, x, y - 2, D3DCOLOR_RGBA(1, 1, 1, 255));
        _text(text, x, y + 2, D3DCOLOR_RGBA(1, 1, 1, 255));
    }
    _text(text, x, y, color);
}

auto Overlay::DrawBox(float x, float y, float width, float height, D3DCOLOR color) -> void
{
    D3DXVECTOR2 points[5];
    points[0] = D3DXVECTOR2(x, y);
    points[1] = D3DXVECTOR2(x + width, y);
    points[2] = D3DXVECTOR2(x + width, y + height);
    points[3] = D3DXVECTOR2(x, y + height);
    points[4] = D3DXVECTOR2(x, y);
    d3Line->Draw(points, 5, color);

}