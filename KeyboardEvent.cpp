#include "KeyboardEvent.h"

namespace Client
{
    KeyboardEvent::KeyboardEvent(const KeyboardEventType type, const unsigned char keyCode) noexcept
        : _type(type), _keyCode(keyCode)
    {
    }

    bool KeyboardEvent::IsPress() const noexcept
    {
        return _type == KeyboardEventType::KeyDown;
    }

    bool KeyboardEvent::IsRelease() const noexcept
    {
        return _type == KeyboardEventType::KeyUp;
    }

    unsigned char KeyboardEvent::GetKeyCode() const noexcept
    {
        return _keyCode;
    }
}