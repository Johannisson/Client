#pragma once

#include "GraphicsException.h"

namespace Client
{
    class GraphicsDeviceRemovedException final : public GraphicsException
    {
    public:
        GraphicsDeviceRemovedException(const std::string& message, HRESULT hResult, const DXGIInformationManager& informationManager, const std::source_location& location = std::source_location::current()) noexcept;

        [[nodiscard]] std::string GetType() const noexcept override;
    };
}
