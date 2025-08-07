#include "Keyboard.h"

namespace Client
{
    void Keyboard::OnKeyPressed(const unsigned char keyCode) noexcept
    {
        _keyState[keyCode] = true;
        _keyboardBuffer.emplace(KeyboardEventType::KeyDown, keyCode);
        TrimKeyboardBuffer();
    }

    void Keyboard::OnKeyReleased(const unsigned char keyCode) noexcept
    {
        _keyState[keyCode] = false;
        _keyboardBuffer.emplace(KeyboardEventType::KeyUp, keyCode);
        TrimKeyboardBuffer();
    }

    bool Keyboard::IsKeyPressed(const unsigned char keyCode) noexcept
    {
        return _keyState[keyCode];
    }

    void Keyboard::ClearKeyState() noexcept
    {
        _keyState.reset();
    }

    void Keyboard::TrimKeyboardBuffer() noexcept
    {
        while (_keyboardBuffer.size() > _keyboardBufferSize)
        {
            _keyboardBuffer.pop();
        }
    }
}