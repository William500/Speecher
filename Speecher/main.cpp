#include "PrecompiledHeader.h"
#include "KeywordRecognizer.h"
#include "RoInitializeWrapper.h"

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	RoInitializeWrapper roInit;

	KeywordRecognizer recognizer(ABI::Windows::Media::SpeechRecognition::SpeechRecognitionConfidence_Low,
	{
		L"Hello, world!",
		L"Fuck you",
		L"DAMN SON"
	});

	recognizer.AddCallback([](const std::wstring& word)
	{
		OutputDebugStringW(word.c_str());
		OutputDebugStringW(L"\r\n");
	});

	recognizer.StartAsync();

	Sleep(100000000);
	return 0;
};