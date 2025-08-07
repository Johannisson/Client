#include <sstream>
#include <windows.h>

#include "Window.h"

namespace Client
{
    /**
     * Constructs a Window object by initializing the base Mouse class, setting up the
     * window class, calculating the adjusted window size, creating the window, and
     * displaying it.
     */
    Window::Window(const int width, const int height)
        : Mouse(width, height), _windowClass(HandleMessageSetup), _width(width), _height(height)
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

    /**
     * Destroys the window and releases associated resources.
     */
    Window::~Window()
    {
        DestroyWindow(_hWnd);
    }

    /**
     * Calculates and returns the adjusted window size to account for window decorations
     * like borders and title bar.
     */
    Size<int> Window::GetAdjustedWindowSize(const int width, const int height) const noexcept
    {
        RECT rect { 0, 0, width, height };

        AdjustWindowRect(&rect, _windowStyle, FALSE);

        return {rect.right - rect.left, rect.bottom - rect.top};
    }

    LRESULT CALLBACK Window::HandleMessageSetup(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        if (msg == WM_NCCREATE)
        {
            const auto* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
            const auto pWnd = static_cast<Window*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageProxy));
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    /*
     * Message handler that retrieves the Window instance from the window's user data
     * and forwards the message to the instance-specific HandleMessage method.
     */
    LRESULT CALLBACK Window::HandleMessageProxy(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        const auto pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK Window::HandleMessage(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        if (IsSystemMessage(msg))
        {
            HandleSystemMessage(msg);
        }
        else if (IsKeyboardMessage(msg))
        {
            HandleKeyboardMessage(msg, wParam, lParam);
        }
        else if (IsRawInputMessage(msg))
        {
            HandleRawInputMessage(lParam);
        }
        else if (IsMouseMessage(msg))
        {
            HandleMouseMessage(msg, wParam, lParam);
        }
        else if (msg == WM_CLOSE)
        {
            return 0;
        }

        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    bool Window::IsSystemMessage(const UINT msg) noexcept
    {
        return msg == WM_CLOSE;
    }

    bool Window::IsKeyboardMessage(const UINT msg) noexcept
    {
        return msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP || msg == WM_KILLFOCUS;
    }

    bool Window::IsRawInputMessage(const UINT msg) noexcept
    {
        return msg == WM_INPUT;
    }

    bool Window::IsMouseMessage(const UINT msg) noexcept
    {
        return msg == WM_MOUSEMOVE || msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP || msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP || msg == WM_MOUSEWHEEL;
    }

    void Window::HandleSystemMessage(const UINT msg) noexcept
    {
        if (msg == WM_CLOSE)
        {
            PostQuitMessage(0);
        }
    }

    void Window::HandleKeyboardMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        const auto isAutoRepeat = lParam & 0x40000000;
        const auto keyCode = static_cast<unsigned char>(wParam);

        if (msg == WM_KILLFOCUS)
        {
            Keyboard.ClearKeyState();
        }
        else if (isAutoRepeat == false && (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN))
        {
            Keyboard.OnKeyPressed(keyCode);
        }
        else if (msg == WM_KEYUP || msg == WM_SYSKEYUP)
        {
            Keyboard.OnKeyReleased(keyCode);
        }
    }

    void Window::HandleRawInputMessage(const LPARAM lParam) noexcept
    {
        UINT size = 0;
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
        {
            return;
        }

        _rawInputBuffer.resize(size);
        if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, _rawInputBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
        {
            return;
        }

        const auto& [header, data] = reinterpret_cast<const RAWINPUT&>(*_rawInputBuffer.data());
        const auto x = data.mouse.lLastX;
        const auto y = data.mouse.lLastY;
        if (header.dwType == RIM_TYPEMOUSE && (x != 0 || y != 0))
        {
            Mouse.OnMouseMoveRaw(x, y);
        }
    }

    void Window::HandleMouseMessage(const UINT msg, const WPARAM wParam, const LPARAM lParam) noexcept
    {
        const auto [x, y] = MAKEPOINTS(lParam);

        switch (msg)
        {
        case WM_MOUSEMOVE:
            OnMouseMove(x, y, wParam);
            break;
        case WM_MOUSEWHEEL:
            Mouse.OnWheelDelta(x, y, GET_WHEEL_DELTA_WPARAM(wParam));
            break;
        case WM_LBUTTONDOWN:
            Mouse.OnLeftPressed(x, y);
            break;
        case WM_LBUTTONUP:
            Mouse.OnLeftReleased(x, y);
            break;
        case WM_RBUTTONDOWN:
            Mouse.OnRightPressed(x, y);
            break;
        case WM_RBUTTONUP:
            Mouse.OnRightReleased(x, y);
            break;
        default:
            break;
        }
    }

    void Window::OnMouseMove(const int x, const int y, const WPARAM wParam) noexcept
    {
        if (x >= 0 && x < _width && y >= 0 && y < _height)
        {
            Mouse.OnMouseMove(x, y);
            if (!Mouse.IsInWindow())
            {
                SetCapture(_hWnd);
                Mouse.OnMouseEnter();
            }
        }
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                Mouse.OnMouseMove(x, y);
            }
            else
            {
                ReleaseCapture();
                Mouse.OnMouseLeave();
            }
        }
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

    void Window::SetTitle(const std::string& title) const noexcept
    {
        SetWindowText(_hWnd, title.c_str());
    }
}
