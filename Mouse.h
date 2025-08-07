#pragma once

#include <optional>
#include <queue>

#include "MouseEvent.h"
#include "MouseRawEvent.h"

namespace Client
{
    class Mouse
    {
        friend class Window;

    public:
        Mouse(int width, int height);

        [[nodiscard]] int GetX() const noexcept;
        [[nodiscard]] int GetY() const noexcept;
        [[nodiscard]] float GetNormalizedX() const noexcept;
        [[nodiscard]] float GetNormalizedY() const noexcept;
        [[nodiscard]] bool IsInWindow() const noexcept;
        [[nodiscard]] bool IsLeftPressed() const noexcept;
        [[nodiscard]] bool IsRightPressed() const noexcept;
        [[nodiscard]] std::optional<MouseEvent> ReadEvent() noexcept;
        [[nodiscard]] std::optional<MouseRawEvent> ReadRawEvent() noexcept;
        [[nodiscard]] bool IsEmpty() const noexcept;
        void Flush() noexcept;

    private:
        void OnMouseMove(int x, int y) noexcept;
        void OnMouseMoveRaw(int x, int y) noexcept;
        void OnMouseEnter() noexcept;
        void OnMouseLeave() noexcept;
        void OnLeftPressed(int x, int y) noexcept;
        void OnLeftReleased(int x, int y) noexcept;
        void OnRightPressed(int x, int y) noexcept;
        void OnRightReleased(int x, int y) noexcept;
        void OnWheelUp(int x, int y) noexcept;
        void OnWheelDown(int x, int y) noexcept;
        void OnWheelDelta(int x, int y, int delta) noexcept;

        void TrimEventBuffer() noexcept;
        void TrimRawEventBuffer() noexcept;

        static constexpr unsigned int _bufferSize = 16;

        int _x = 0;
        int _y = 0;
        int _width;
        int _height;
        bool _leftPressed = false;
        bool _rightPressed = false;
        bool _inWindow = false;
        int _wheelDelta = 0;
        std::queue<MouseEvent> _eventBuffer;
        std::queue<MouseRawEvent> _rawEventBuffer;
    };
}