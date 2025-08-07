#pragma once

#include <windows.h>
#include <source_location>

#include "Exception.h"

namespace Client
{
    class WindowException : public Exception
    {
    public:
        WindowException(const std::string& message, DWORD dWord, const std::source_location& location = std::source_location::current()) noexcept;

        [[nodiscard]] std::string GetType() const noexcept override;
        [[nodiscard]] std::string GetDescription() const noexcept override;

        [[nodiscard]] static std::string TranslateErrorCode(DWORD dWord) noexcept;

    private:
        DWORD _dWord;
    };
}
