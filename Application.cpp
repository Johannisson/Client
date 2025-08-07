#include "Application.h"

namespace Client
{
    Application::Application(const int width, const int height)
        : _window(width, height)
    {
    }

    WPARAM Application::Run()
    {
        while (true)
        {
            if (const auto code = _window.ProcessMessages())
            {
                return *code;
            }
        }
    }
}
