#pragma once

class KeywordRecognizer
{
private:
	Microsoft::WRL::ComPtr<ABI::Windows::Media::SpeechRecognition::ISpeechRecognizer> m_Recognizer;
	Microsoft::WRL::ComPtr<ABI::Windows::Media::SpeechRecognition::ISpeechRecognizer2> m_Recognizer2;
	Microsoft::WRL::ComPtr<ABI::Windows::Media::SpeechRecognition::ISpeechContinuousRecognitionSession> m_Session;
	std::vector<EventRegistrationToken> m_Callbacks;
	ABI::Windows::Media::SpeechRecognition::SpeechRecognitionConfidence m_MinimumConfidence;

public:
	KeywordRecognizer(ABI::Windows::Media::SpeechRecognition::SpeechRecognitionConfidence minimumConfidence, const std::vector<std::wstring>& keywords);
	~KeywordRecognizer();

	void AddCallback(std::function<void(const std::wstring&)> callback);
	void StartAsync();
	void StopAsync();
};