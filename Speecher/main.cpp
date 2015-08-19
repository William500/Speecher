#include "PrecompiledHeader.h"
#include "KeywordRecognizer.h"
#include "RoInitializeWrapper.h"

#include "XAudio2\AudioManager.h"
#include "XAudio2\Sound.h"

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	RoInitializeWrapper roInit;

	KeywordRecognizer recognizer(ABI::Windows::Media::SpeechRecognition::SpeechRecognitionConfidence_Low,
	{
		L"Hello, world!",
		L"Fuck you",
		L"DAMN SON",
		L"Allahu akbar",
		L"Google",
		L"Gmail",
		L"Why won't you fuck off"
	});

	AudioManager::Initialize();

	Sound allahu(L"ALLAHU_AKBAR", false, false);

	auto quitEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
	Assert(quitEvent != nullptr);

	recognizer.AddCallback([&allahu, &quitEvent](const std::wstring& word)
	{
		OutputDebugStringW(word.c_str());
		OutputDebugStringW(L"\r\n");

		if (word.compare(L"Allahu akbar") == 0)
		{
			allahu.Play();
		}
		else if (word.compare(L"Google") == 0)
		{
			ShellExecute(NULL, L"open", L"http://www.google.com", NULL, NULL, SW_SHOWDEFAULT);
		}
		else if (word.compare(L"Gmail") == 0)
		{
			ShellExecute(NULL, L"open", L"http://www.gmail.com", NULL, NULL, SW_SHOWDEFAULT);
		}
		else if (word.compare(L"Why won't you fuck off") == 0)
		{
			SetEvent(quitEvent);
		}
	});

	recognizer.StartAsync();

	WaitForSingleObject(quitEvent, INFINITE);
	return 0;
};