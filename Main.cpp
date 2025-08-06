// ReSharper disable CppParameterNeverUsed

#include <Windows.h>

#include "Application.h"

using namespace Client;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    auto application = Application(640, 480);

    application.Run();

    return -1;
}
