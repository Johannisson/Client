#pragma once

#include "MouseEventType.h"

namespace Client
{
    class MouseEvent
    {
    public:
        MouseEvent(MouseEventType type, int x, int y, bool left, bool right) noexcept;

        [[nodiscard]] MouseEventType GetType() const noexcept;
        [[nodiscard]] int GetX() const noexcept;
        [[nodiscard]] int GetY() const noexcept;
        [[nodiscard]] bool IsLeftPressed() const noexcept;
        [[nodiscard]] bool IsRightPressed() const noexcept;

    private:
        MouseEventType _type;
        int _x;
        int _y;
        bool _left;
        bool _right;
    };
}