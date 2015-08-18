#include "PrecompiledHeader.h"

#include "AsyncOperationHandler.h"
#include "EventHandler.h"
#include "Iterable.h"
#include "KeywordRecognizer.h"
#include "SynchronousOperation.h"

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Media::SpeechRecognition;
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

KeywordRecognizer::KeywordRecognizer(SpeechRecognitionConfidence minimumConfidence, const std::vector<std::wstring>& keywords) :
	m_MinimumConfidence(minimumConfidence)
{
	ComPtr<IInspectable> recognizerInspectable;

	auto hr = RoActivateInstance(HStringReference(RuntimeClass_Windows_Media_SpeechRecognition_SpeechRecognizer).Get(), &recognizerInspectable);
	Assert(SUCCEEDED(hr));

	hr = recognizerInspectable.As(&m_Recognizer);
	Assert(SUCCEEDED(hr));

	hr = recognizerInspectable.As(&m_Recognizer2);
	Assert(SUCCEEDED(hr));

	std::vector<HSTRING> hstringKeywords;
	hstringKeywords.reserve(keywords.size());

	for (const auto& keyword : keywords)
	{
		HSTRING hstring;
		WindowsCreateString(keyword.c_str(), static_cast<uint32_t>(keyword.length()), &hstring);
		hstringKeywords.push_back(hstring);
	}

	auto iterable = Make<Iterable<HSTRING>>(std::move(hstringKeywords));

	ComPtr<ISpeechRecognitionListConstraintFactory> listConstraintFactory;
	hr = RoGetActivationFactory(HStringReference(RuntimeClass_Windows_Media_SpeechRecognition_SpeechRecognitionListConstraint).Get(), __uuidof(ISpeechRecognitionListConstraintFactory), &listConstraintFactory);
	Assert(SUCCEEDED(hr));

	ComPtr<ISpeechRecognitionListConstraint> listConstraint;
	hr = listConstraintFactory->Create(iterable.Get(), &listConstraint);
	Assert(SUCCEEDED(hr));

	ComPtr<ISpeechRecognitionConstraint> keywordConstraint;
	hr = listConstraint.As(&keywordConstraint);
	Assert(SUCCEEDED(hr));

	ComPtr<IVector<ISpeechRecognitionConstraint*>> constraints;
	hr = m_Recognizer->get_Constraints(&constraints);
	Assert(SUCCEEDED(hr));

	hr = constraints->Append(keywordConstraint.Get());
	Assert(SUCCEEDED(hr));

	ComPtr<IAsyncOperation<SpeechRecognitionCompilationResult*>> compilationOperation;
	hr = m_Recognizer->CompileConstraintsAsync(&compilationOperation);
	Assert(SUCCEEDED(hr));

	SpeechRecognitionResultStatus compilationStatus;
	auto compilationResult = SynchronousOperation<SpeechRecognitionCompilationResult*>(compilationOperation.Get()).Wait();
	hr = compilationResult->get_Status(&compilationStatus);
	Assert(SUCCEEDED(hr));

	Assert(compilationStatus == SpeechRecognitionResultStatus_Success);

	hr = m_Recognizer2->get_ContinuousRecognitionSession(&m_Session);
	Assert(SUCCEEDED(hr));
}

KeywordRecognizer::~KeywordRecognizer()
{
	for (auto callback : m_Callbacks)
	{
		m_Session->remove_ResultGenerated(callback);
	}
}

void KeywordRecognizer::AddCallback(std::function<void(const std::wstring&)> callback)
{
	auto handler = Make<EventHandler<SpeechContinuousRecognitionSession*, SpeechContinuousRecognitionResultGeneratedEventArgs*>>
		([callback, this](ISpeechContinuousRecognitionSession*, ISpeechContinuousRecognitionResultGeneratedEventArgs* args)
	{
		ComPtr<ISpeechRecognitionResult> recognitionResult;
		auto hr = args->get_Result(&recognitionResult);
		Assert(SUCCEEDED(hr));

		SpeechRecognitionConfidence confidence;
		hr = recognitionResult->get_Confidence(&confidence);
		Assert(SUCCEEDED(hr));

		if (confidence > m_MinimumConfidence)
			return;

		HSTRING value;
		hr = recognitionResult->get_Text(&value);
		Assert(SUCCEEDED(hr));

		Assert(value != nullptr);

		uint32_t dummy;
		callback(WindowsGetStringRawBuffer(value, &dummy));
	});

	EventRegistrationToken callbackToken;
	auto hr = m_Session->add_ResultGenerated(handler.Get(), &callbackToken);
	Assert(SUCCEEDED(hr));

	m_Callbacks.push_back(callbackToken);
}

void KeywordRecognizer::StartAsync()
{
	ComPtr<IAsyncAction> startAction;
	auto hr = m_Session->StartAsync(&startAction);
	Assert(SUCCEEDED(hr));
}

void KeywordRecognizer::StopAsync()
{

}
