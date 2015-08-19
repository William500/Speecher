#pragma once

#include "PrecompiledHeader.h"

struct SoundCacheKey
{
	std::wstring path;
	bool loopForever;
	bool hasReverb;

	SoundCacheKey(const std::wstring& path, bool loopForever, bool hasReverb) :
		path(path), loopForever(loopForever), hasReverb(hasReverb)
	{
	}

	bool operator==(const SoundCacheKey& other) const
	{
		return path == other.path && loopForever == other.loopForever && hasReverb == other.hasReverb;
	}
};

template <>
struct std::hash<SoundCacheKey>
{
	size_t operator()(const SoundCacheKey& value) const
	{
		return std::hash<std::wstring>()(value.path) ^ std::hash<int>()(value.loopForever + 2 * value.hasReverb);
	}
};
