// ReSharper disable IdentifierTypo

#pragma once

#include <string>

#define WSTRING(value) std::wstring((value).begin(), (value).end()).c_str()
