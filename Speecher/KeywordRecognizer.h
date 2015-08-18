#pragma once

class KeywordRecognizer
{
private:

public:
	KeywordRecognizer(const std::vector<std::wstring>& keywords);
	~KeywordRecognizer();

	void StartAsync(std::function<void(const std::wstring&)> callback);
	void StopAsync();
};