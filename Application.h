#pragma once

#include <Windows.h>

#include "Window.h"

namespace Client
{
    class Application
    {
    public:
        Application(int width, int height);

        WPARAM Run();

    private:
        void Frame();

        Window _window;
    };
}
