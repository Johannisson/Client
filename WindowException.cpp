#include <sstream>

#include "WindowException.h"

namespace Client
{
    WindowException::WindowException(const std::string& message, const DWORD dWord, const std::source_location& location) noexcept
        : Exception(message, location), _dWord(dWord)
    {
    }

    std::string WindowException::GetType() const noexcept
    {
        return "WindowException";
    }

    std::string WindowException::GetDescription() const noexcept
    {
        std::ostringstream oss;

        oss << _message << "\n\n" << TranslateErrorCode(_dWord) << "\n" << _filename << " (Line: " << _line << ")";

        return oss.str();
    }

    std::string WindowException::TranslateErrorCode(const DWORD dWord) noexcept
    {
        char* pMessageBuffer = nullptr;
        const auto length = FormatMessage
        (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dWord,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&pMessageBuffer),
            0,
            nullptr
        );

        if (length == 0)
        {
            return "Undefined error code";
        }

        std::string result = pMessageBuffer;
        LocalFree(pMessageBuffer);

        return result;
    }
}