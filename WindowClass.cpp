#include "WindowClass.h"

namespace Client
{
    WindowClass::WindowClass(const WNDPROC handleMessageSetup) noexcept
        : _windowClassName("Game"), _hInstance(GetModuleHandle(nullptr))
    {
        WNDCLASSEX wc = {};

        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = handleMessageSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = _hInstance;
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = GetName();

        RegisterClassEx(&wc);
    }

    WindowClass::~WindowClass()
    {
        UnregisterClass(GetName(), _hInstance);
    }

    LPCSTR WindowClass::GetName() const noexcept
    {
        return _windowClassName;
    }

    HINSTANCE WindowClass::GetInstance() const noexcept
    {
        return _hInstance;
    }
}