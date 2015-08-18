#pragma once

#include "AsyncOperationHandler.h"

template <typename T>
class SynchronousOperation
{
private:
	typedef typename AsyncOperationHandler<T>::T_Interface T_Result;

	Microsoft::WRL::ComPtr<AsyncOperationHandler<T>> m_Handler;
	T_Result m_Result;
	HANDLE m_Event;	

public:
	SynchronousOperation(ABI::Windows::Foundation::IAsyncOperation<T>* operation)
	{
		m_Event = CreateEventW(nullptr, FALSE, FALSE, nullptr);
		Assert(m_Event != nullptr);

		m_Handler = Make<AsyncOperationHandler<T>>([this](T_Result result)
		{
			m_Result = result;

			auto setResult = SetEvent(m_Event);
			Assert(setResult != FALSE);
		});

		auto hr = operation->put_Completed(m_Handler.Get());
		Assert(SUCCEEDED(hr));
	}

	~SynchronousOperation()
	{
		CloseHandle(m_Event);
	}

	T_Result Wait()
	{
		auto waitResult = WaitForSingleObjectEx(m_Event, INFINITE, FALSE);
		Assert(waitResult == WAIT_OBJECT_0);

		return m_Result;
	}
};