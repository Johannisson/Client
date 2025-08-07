#pragma once

#include <string>
#include <optional>
#include <memory>

#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
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
        Window& operator=(Window&& _) = delete;

    private:
        [[nodiscard]] Size<int> GetAdjustedWindowSize(int width, int height) const noexcept;
        static void RegisterRawMouseInputDevice();

        static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMessageProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

        static bool IsSystemMessage(UINT msg) noexcept;
        static bool IsKeyboardMessage(UINT msg) noexcept;
        static bool IsRawInputMessage(UINT msg) noexcept;
        static bool IsMouseMessage(UINT msg) noexcept;
        static void HandleSystemMessage(UINT msg) noexcept;
        void HandleKeyboardMessage(UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        void HandleRawInputMessage(LPARAM lParam) noexcept;
        void HandleMouseMessage(UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        void OnMouseMove(int x, int y, WPARAM wParam) noexcept;

    public:
        static std::optional<WPARAM> ProcessMessages();
        void SetTitle(const std::string& title) const noexcept;
        [[nodiscard]] Graphics& GetGraphics() const;

        Keyboard Keyboard;
        Mouse Mouse;

    private:
        HWND _hWnd;
        WindowClass _windowClass;
        DWORD _windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
        int _width;
        int _height;
        std::unique_ptr<Graphics> _pGraphics;
        std::vector<BYTE> _rawInputBuffer;
    };
}
