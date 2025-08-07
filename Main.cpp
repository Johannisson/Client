// ReSharper disable CppParameterNeverUsed

#include <Windows.h>

#include "Application.h"
#include "Exception.h"

using namespace Client;

/**
 * The standard Windows application entry point, replacing the typical `main` function
 * used in console applications. It initializes the application and enters the message loop.
 */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    try
    {
        auto application = Application(640, 480);

        application.Run();
    }
    catch (const Exception& e)
    {
        MessageBox(nullptr, e.GetDescription().c_str(), e.GetType().c_str(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "Unknown", "Unknown", MB_OK | MB_ICONEXCLAMATION);
    }

    return -1;
}
