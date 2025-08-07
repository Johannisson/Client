#pragma once

#include "DXGIInformationManager.h"
#include "WindowException.h"

namespace Client
{
    class GraphicsException : public WindowException
    {
    public:
        GraphicsException(const std::string& message, HRESULT hResult, const std::source_location& location = std::source_location::current()) noexcept;
        GraphicsException(const std::string& message, HRESULT hResult, const DXGIInformationManager& informationManager, const std::source_location& location = std::source_location::current()) noexcept;

        [[nodiscard]] std::string GetType() const noexcept override;
        [[nodiscard]] std::string GetDescription() const noexcept override;

    private:
        HRESULT _hResult;
        std::string _information;
    };
}
