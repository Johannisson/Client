#include <sstream>

#include "Exception.h"

namespace Client
{
    Exception::Exception(std::string message, const std::source_location& location) noexcept
        : _message(std::move(message)), _filename(location.file_name()), _line(location.line())
    {
    }

    std::string Exception::GetType() const noexcept
    {
        return "Exception";
    }

    std::string Exception::GetDescription() const noexcept
    {
        std::ostringstream oss;

        oss << _message << "\n\n" << _filename << " (Line: " << _line << ")";

        return oss.str();
    }
}
