#include <windows.h>

#include "Window.h"

namespace Client
{
    Window::Window(const int width, const int height)
        : _windowClass(HandleMessageSetup)
    {
        const auto size = GetAdjustedWindowSize(width, height);

        _hWnd = CreateWindow(
            _windowClass.GetName(),
            "Client",
            _windowStyle,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            size.Width,
            size.Height,
            nullptr,
            nullptr,
            _windowClass.GetInstance(),
            this
        );

        ShowWindow(_hWnd, SW_SHOWDEFAULT);
    }

    Window::~Window()
    {
        DestroyWindow(_hWnd);
    }

    Size<int> Window::GetAdjustedWindowSize(const int width, const int height) const noexcept
    {
        RECT rect;
        rect.left = 0;
        rect.right = width;
        rect.top = 0;
        rect.bottom = height;

        AdjustWindowRect(&rect, _windowStyle, FALSE);

        return { rect.right - rect.left, rect.bottom - rect.top };
    }

    LRESULT CALLBACK Window::HandleMessageSetup(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        if (msg == WM_NCCREATE)
        {
            const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            const auto pWnd = static_cast<Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageProxy));
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK Window::HandleMessageProxy(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        const auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK Window::HandleMessage(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        if (msg == WM_CLOSE)
        {
            PostQuitMessage(0);
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    std::optional<WPARAM> Window::ProcessMessages()
    {
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                return msg.wParam;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return {};
    }
}
