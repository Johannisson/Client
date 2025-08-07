#include <sstream>

#include "GraphicsException.h"
#include "WindowException.h"

namespace Client
{
    GraphicsException::GraphicsException(const std::string& message, const HRESULT hResult, const std::source_location& location) noexcept
        : WindowException(message, 0, location), _hResult(hResult)
    {
    }

    GraphicsException::GraphicsException(const std::string& message, const HRESULT hResult, const DXGIInformationManager& informationManager, const std::source_location& location) noexcept
        : WindowException(message, 0, location), _hResult(hResult)
    {
        for (const auto& informationMessage : informationManager.GetMessages())
        {
            _information += informationMessage + "\n";
            _information.push_back('\n');
        }
    }

    std::string GraphicsException::GetType() const noexcept
    {
        return "GraphicsException";
    }

    std::string GraphicsException::GetDescription() const noexcept
    {
        std::ostringstream oss;

        oss << _message << "\n\n" << TranslateErrorCode(_hResult) << "\n" << _filename << " (Line: " << _line << ")";

        if (!_information.empty())
        {
            oss << "\n\n" << _information;
        }

        return oss.str();
    }
}
