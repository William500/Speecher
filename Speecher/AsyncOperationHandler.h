#pragma once

template <typename T>
class AsyncOperationHandler :
	public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ABI::Windows::Foundation::IAsyncOperationCompletedHandler<T>>
{
public:
	typedef typename ABI::Windows::Foundation::Internal::GetAbiType<typename ABI::Windows::Foundation::IAsyncOperationCompletedHandler<T>::TResult_complex>::type T_Interface;

private:
	std::function<void(T_Interface)> m_Callback;

public:
	AsyncOperationHandler(std::function<void(T_Interface)> callback) :
		m_Callback(std::move(callback))
	{
	}

	virtual HRESULT STDMETHODCALLTYPE Invoke(ABI::Windows::Foundation::IAsyncOperation<T>* asyncInfo, ABI::Windows::Foundation::AsyncStatus status) override
	{
		Assert(status == ABI::Windows::Foundation::AsyncStatus::Completed);

		T_Interface result;
		auto hr = asyncInfo->GetResults(&result);
		Assert(SUCCEEDED(hr));

		m_Callback(result);
		return S_OK;
	}
};