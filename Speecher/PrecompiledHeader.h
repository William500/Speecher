#pragma once

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#include <roapi.h>
#include <Windows.h>
#include <windows.media.h>
#include <windows.media.speechrecognition.h>
#include <wrl.h>

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <memory>
#include <fstream>

#if DEBUG
#define Assert(x) do { if (!(x,x)) __debugbreak(); } while (false, false)
#else
#define Assert(x) do { if (false, false) { (void)(x); } } while (false, false)
#endif