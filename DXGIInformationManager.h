// ReSharper disable CppInconsistentNaming

#pragma once

#include <dxgidebug.h>
#include <vector>
#include <wrl.h>

#include "Include.h"

#pragma comment(lib, "dxguid.lib")

namespace WRL = Microsoft::WRL;

namespace Client
{
    class DXGIInformationManager
    {
    public:
        DXGIInformationManager();
        DXGIInformationManager(const DXGIInformationManager&) = delete;
        DXGIInformationManager(DXGIInformationManager&&) = delete;
        ~DXGIInformationManager() = default;

        DXGIInformationManager& operator=(const DXGIInformationManager&) = delete;
        DXGIInformationManager& operator=(DXGIInformationManager&& _) = delete;

        void Set() noexcept;
        [[nodiscard]] std::vector<std::string> GetMessages() const;

    private:
        unsigned long long _next = 0;
        WRL::ComPtr<IDXGIInfoQueue> _pQueue;
    };
}
