#pragma once

#include <string>
#include <source_location>

namespace Client
{
    class Exception : std::exception
    {
    public:
        explicit Exception(std::string message, const std::source_location& location = std::source_location::current()) noexcept;
        Exception(const Exception&) = default;
        Exception(Exception&&) = default;

        ~Exception() override = default;

        Exception& operator=(const Exception&) = delete;
        Exception& operator=(Exception&&) = delete;

        [[nodiscard]] virtual std::string GetType() const noexcept;
        [[nodiscard]] virtual std::string GetDescription() const noexcept;

    protected:
        std::string _message;
        std::string _filename;
        unsigned int _line;
    };
}
