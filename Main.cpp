// ReSharper disable CppParameterNeverUsed

#include <Windows.h>

#include "Application.h"

using namespace Client;

/**
 * The standard Windows application entry point, replacing the typical `main` function
 * used in console applications. It initializes the application and enters the message loop.
 */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto application = Application(640, 480);

    application.Run();

    return -1;
}
