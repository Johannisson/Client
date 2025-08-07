#pragma once

#include "KeyboardEventType.h"

namespace Client
{
    class KeyboardEvent
    {
    public:
        KeyboardEvent(KeyboardEventType type, unsigned char keyCode) noexcept;

        [[nodiscard]] bool IsPress() const noexcept;
        [[nodiscard]] bool IsRelease() const noexcept;
        [[nodiscard]] unsigned char GetKeyCode() const noexcept;

    private:
        KeyboardEventType _type;
        unsigned char _keyCode;
    };
}