// ReSharper disable StringLiteralTypo

#include <dxgidebug.h>
#include <memory>

#include "DXGIInformationManager.h"
#include "Graphics.h"
#include "Window.h"
#include "WindowException.h"

#pragma comment(lib, "dxguid.lib")

namespace Client
{
#ifndef NDEBUG
    DXGIInformationManager::DXGIInformationManager()
    {
        typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

        const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (hModDxgiDebug == nullptr)
        {
            throw WindowException("Unable to load 'dxgidebug.dll'.", GetLastError());
        }

        const auto dxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface")));
        if (dxgiGetDebugInterface == nullptr)
        {
            throw WindowException("Unable to get procedure address for DXGIGetDebugInterface.", GetLastError());
        }

        const auto hResult = dxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &_pQueue);
        if (FAILED(hResult))
        {
            throw GraphicsException("Unable to get debug interface.", hResult);
        }
    }

    void DXGIInformationManager::Set() noexcept
    {
        _next = _pQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
    }

    std::vector<std::string> DXGIInformationManager::GetMessages() const
    {
        std::vector<std::string> messages;
        const auto end = _pQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
        for (auto i = _next; i < end; i++)
        {
            SIZE_T messageLength;

            auto hResult = _pQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);
            if (FAILED(hResult))
            {
                throw GraphicsException("Unable to get message length.", hResult);
            }

            auto bytes = std::make_unique<byte[]>(messageLength);
            const auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

            hResult = _pQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength);
            if (FAILED(hResult))
            {
                throw GraphicsException("Unable to get message.", hResult);
            }

            messages.emplace_back(pMessage->pDescription);
        }

        return messages;
    }
#else
    DXGIInformationManager::DXGIInformationManager()
    {
    }

    void DXGIInformationManager::Set() noexcept
    {
    }

    std::vector<std::string> DXGIInformationManager::GetMessages() const
    {
        return {};
    }
#endif
}
