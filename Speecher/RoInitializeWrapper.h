#pragma once

struct RoInitializeWrapper
{
	RoInitializeWrapper()
	{
		auto hr = RoInitialize(RO_INIT_MULTITHREADED);
		Assert(SUCCEEDED(hr));
	}

	~RoInitializeWrapper()
	{
		RoUninitialize();
	}
};