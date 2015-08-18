#pragma once

#define WINVER 0x0601
#define _WIN32_WINNT 0x0601

#include <Windows.h>

#include <string>
#include <vector>

#if DEBUG
#define Assert(x) do { if (!(x)) __debugbreak(); } while (false, false)
#else
#define Assert(x) do { } while (false, false)
#endif