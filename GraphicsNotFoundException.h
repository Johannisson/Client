#pragma once

#include <string>

#include "Exception.h"

namespace Client
{
    class GraphicsNotFoundException final : public Exception
    {
    public:
        using Exception::Exception;

        [[nodiscard]] std::string GetType() const noexcept override;
    };
}
