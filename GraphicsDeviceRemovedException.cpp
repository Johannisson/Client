#include "GraphicsDeviceRemovedException.h"

namespace Client
{
    GraphicsDeviceRemovedException::GraphicsDeviceRemovedException(const std::string& message, const HRESULT hResult, const DXGIInformationManager& informationManager, const std::source_location& location) noexcept
        : GraphicsException(message, hResult, informationManager, location)
    {
    }

    std::string GraphicsDeviceRemovedException::GetType() const noexcept
    {
        return "GraphicsDeviceRemovedException";
    }
}
