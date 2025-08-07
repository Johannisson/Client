#include "GraphicsNotFoundException.h"

namespace Client
{
    std::string GraphicsNotFoundException::GetType() const noexcept
    {
        return "GraphicsNotFoundException";
    }
}
