#pragma once

#include <windows.h>

namespace Client
{
    class WindowClass
    {
    public:
        explicit WindowClass(WNDPROC handleMessageSetup) noexcept;
        WindowClass(const WindowClass&) = delete;
        WindowClass(WindowClass&&) = delete;

        ~WindowClass();

        WindowClass& operator=(const WindowClass&) = delete;
        WindowClass& operator=(WindowClass&& vector) = delete;


        [[nodiscard]] LPCSTR GetName() const noexcept;
        [[nodiscard]] HINSTANCE GetInstance() const noexcept;

    private:
        LPCSTR _windowClassName;
        HINSTANCE _hInstance;
    };
}