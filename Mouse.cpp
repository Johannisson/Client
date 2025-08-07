#include <windows.h>

#include "Mouse.h"

namespace Client
{
    Mouse::Mouse(const int width, const int height)
        : _width(width), _height(height)
    {
    }

    int Mouse::GetX() const noexcept
    {
        return _x;
    }

    int Mouse::GetY() const noexcept
    {
        return _y;
    }

    float Mouse::GetNormalizedX() const noexcept
    {
        return 2.0f * static_cast<float>(_x) / static_cast<float>(_width) - 1.0f;
    }

    float Mouse::GetNormalizedY() const noexcept
    {
        return -2.0f * static_cast<float>(_y) / static_cast<float>(_height) + 1.0f;
    }

    bool Mouse::IsInWindow() const noexcept
    {
        return _inWindow;
    }

    bool Mouse::IsLeftPressed() const noexcept
    {
        return _leftPressed;
    }

    bool Mouse::IsRightPressed() const noexcept
    {
        return _rightPressed;
    }

    std::optional<MouseEvent> Mouse::ReadEvent() noexcept
    {
        if (_eventBuffer.empty())
        {
            return {};
        }

        const auto& result = _eventBuffer.front();
        _eventBuffer.pop();

        return result;
    }

    std::optional<MouseRawEvent> Mouse::ReadRawEvent() noexcept
    {
        if (_rawEventBuffer.empty())
        {
            return {};
        }

        const auto& result = _rawEventBuffer.front();
        _rawEventBuffer.pop();

        return result;
    }

    bool Mouse::IsEmpty() const noexcept
    {
        return _eventBuffer.empty();
    }

    void Mouse::Flush() noexcept
    {
        _eventBuffer = std::queue<MouseEvent>();
    }

    void Mouse::OnMouseMove(const int x, const int y) noexcept
    {
        this->_x = x;
        this->_y = y;

        _eventBuffer.emplace(Move, false, false, x, y);

        TrimEventBuffer();
    }

    void Mouse::OnMouseMoveRaw(const int x, const int y) noexcept
    {
        _rawEventBuffer.push({ x, y });

        TrimRawEventBuffer();
    }

    void Mouse::OnMouseEnter() noexcept
    {
        _inWindow = true;

        _eventBuffer.emplace(Enter, _x, _y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnMouseLeave() noexcept
    {
        _inWindow = false;

        _eventBuffer.emplace(Leave, _x, _y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnLeftPressed(const int x, const int y) noexcept
    {
        _leftPressed = true;

        _eventBuffer.emplace(LeftDown, x, y, true, false);

        TrimEventBuffer();
    }

    void Mouse::OnLeftReleased(const int x, const int y) noexcept
    {
        _leftPressed = false;

        _eventBuffer.emplace(LeftUp, x, y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnRightPressed(const int x, const int y) noexcept
    {
        _rightPressed = true;

        _eventBuffer.emplace(RightDown, x, y, false, true);

        TrimEventBuffer();
    }

    void Mouse::OnRightReleased(const int x, const int y) noexcept
    {
        _rightPressed = false;

        _eventBuffer.emplace(RightUp, x, y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnWheelUp(const int x, const int y) noexcept
    {
        _eventBuffer.emplace(WheelUp, x, y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnWheelDown(const int x, const int y) noexcept
    {
        _eventBuffer.emplace(WheelDown, x, y, false, false);

        TrimEventBuffer();
    }

    void Mouse::OnWheelDelta(const int x, const int y, const int delta) noexcept
    {
        _wheelDelta = _wheelDelta + delta;

        while (_wheelDelta >= WHEEL_DELTA)
        {
            _wheelDelta = _wheelDelta - WHEEL_DELTA;
            OnWheelUp(x, y);
        }
        while (_wheelDelta <= -WHEEL_DELTA)
        {
            _wheelDelta = _wheelDelta + WHEEL_DELTA;
            OnWheelDown(x, y);
        }
    }

    void Mouse::TrimEventBuffer() noexcept
    {
        while (_eventBuffer.size() > _bufferSize)
        {
            _eventBuffer.pop();
        }
    }

    void Mouse::TrimRawEventBuffer() noexcept
    {
        while (_rawEventBuffer.size() > _bufferSize)
        {
            _rawEventBuffer.pop();
        }
    }
}