#pragma once

template <typename TSender, typename TArgs>
class EventHandler :
	public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ABI::Windows::Foundation::ITypedEventHandler<TSender, TArgs>>
{
public:
	typedef ABI::Windows::Foundation::ITypedEventHandler<TSender, TArgs> TBase;

	typedef typename ABI::Windows::Foundation::Internal::GetAbiType<typename TBase::TSender_complex>::type TSender_Interface;
	typedef typename ABI::Windows::Foundation::Internal::GetAbiType<typename TBase::TArgs_complex>::type TArgs_Interface;

private:
	std::function<void(TSender_Interface, TArgs_Interface)> m_Handler;

public:
	EventHandler(std::function<void(TSender_Interface, TArgs_Interface)> handler) :
		m_Handler(std::move(handler))
	{
	}
	
	virtual HRESULT STDMETHODCALLTYPE Invoke(TSender_Interface sender, TArgs_Interface args) override
	{
		m_Handler(sender, args);
		return S_OK;
	}
};
