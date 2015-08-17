#include "PrecompiledHeader.h"

struct CoInitializeWrapper
{
	CoInitializeWrapper()
	{
		auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		Assert(SUCCEEDED(hr));
	}
	
	~CoInitializeWrapper()
	{
		CoUninitialize();
	}
};

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	CoInitializeWrapper coInit;
	
	
}