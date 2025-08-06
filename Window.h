#pragma once

#include <optional>

#include "Size.h"
#include "WindowClass.h"

namespace Client
{
    class Window
    {
    public:
        Window(int width, int height);
        Window(const Window&) = delete;
        Window(Window&&) = delete;

        ~Window();

        Window& operator=(const Window&) = delete;
        Window& operator=(Window&& window) = delete;

    private:
        [[nodiscard]] Size<int> GetAdjustedWindowSize(int width, int height) const noexcept;

        static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMessageProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    public:
        static std::optional<WPARAM> ProcessMessages();

    private:
        HWND _hWnd;
        WindowClass _windowClass;
        DWORD _windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    };
}
