#include "PrecompiledHeader.h"
#include "KeywordRecognizer.h"
#include "NotificationSink.h"

using namespace Microsoft::WRL;

KeywordRecognizer::KeywordRecognizer()
{
	auto hr = CoCreateInstance(CLSID_SpSharedRecoContext, nullptr, CLSCTX_INPROC_SERVER, __uuidof(ISpRecoContext), &m_RecoContext);
	Assert(SUCCEEDED(hr));
	
	auto notificationCallback = [this]() { OnRecoContextNotification(); };
	auto notificationSink = Make<NotificationSink<decltype(notificationCallback)>>(std::move(notificationCallback));
	
	hr = m_RecoContext->SetNotifySink(notificationSink.Get());
	Assert(SUCCEEDED(hr));
	
	ULONGLONG interestingEvents = SPFEI(SPEI_RECOGNITION);
	hr = m_RecoContext->SetInterest(interestingEvents, interestingEvents);
	Assert(SUCCEEDED(hr));
	
	hr = m_RecoContext->CreateGrammar(0, &m_Grammar);
	Assert(SUCCEEDED(hr));
}

KeywordRecognizer::~KeywordRecognizer()
{
	
}

void KeywordRecognizer::SetKeywords(const std::vector<std::wstring>& /*keywords*/)
{
	
}

void KeywordRecognizer::StartAsync()
{

}

void KeywordRecognizer::StopAsync()
{
}