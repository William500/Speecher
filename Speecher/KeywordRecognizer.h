#pragma once

class KeywordRecognizer
{
private:
	Microsoft::WRL::ComPtr<ISpRecoContext> m_RecoContext;
	Microsoft::WRL::ComPtr<ISpRecoGrammar> m_Grammar;
	
	void OnRecoContextNotification();
	
public:
	KeywordRecognizer();
	~KeywordRecognizer();
	
	void SetKeywords(const std::vector<std::wstring>& keywords);
	void StartAsync();
	void StopAsync();
};