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
        Window _window;
    };
}
