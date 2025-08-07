#pragma once

#include <bitset>
#include <queue>

#include "KeyboardEvent.h"

namespace Client
{
    class Keyboard
    {
        friend class Window;

    public:
        Keyboard() = default;
        ~Keyboard() = default;

        Keyboard(const Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;

        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard& operator=(Keyboard&& _) = delete;

        bool IsKeyPressed(unsigned char keyCode) noexcept;
        void ClearKeyState() noexcept;

    private:
        void OnKeyPressed(unsigned char keyCode) noexcept;
        void OnKeyReleased(unsigned char keyCode) noexcept;
        void TrimKeyboardBuffer() noexcept;

        static constexpr unsigned int _keyboardBufferSize = 16;

        std::bitset<256> _keyState;
        std::queue<KeyboardEvent> _keyboardBuffer;
    };
}