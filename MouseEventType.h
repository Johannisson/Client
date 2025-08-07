#pragma once

namespace Client
{
    enum MouseEventType : char
    {
        LeftDown,
        LeftUp,
        RightDown,
        RightUp,
        WheelUp,
        WheelDown,
        Move,
        Enter,
        Leave
    };
}