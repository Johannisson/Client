#include "MouseEvent.h"

namespace Client
{
    MouseEvent::MouseEvent(const MouseEventType type, const int x, const int y, const bool left, const bool right) noexcept
        : _type(type), _x(x), _y(y), _left(left), _right(right)
    {
    }

    MouseEventType MouseEvent::GetType() const noexcept
    {
        return _type;
    }

    int MouseEvent::GetX() const noexcept
    {
        return _x;
    }

    int MouseEvent::GetY() const noexcept
    {
        return _y;
    }

    bool MouseEvent::IsLeftPressed() const noexcept
    {
        return _left;
    }

    bool MouseEvent::IsRightPressed() const noexcept
    {
        return _right;
    }
}