#pragma once

template <typename Notification>
class NotificationSink : 
	public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, ISpNotifySink>
{
private:
	Notification m_Notification;
	
public:
	inline NotificationSink(Notification&& notification) :
		m_Notification(notification)
	{
	}

	HRESULT STDMETHODCALLTYPE Notify() override
	{
		m_Notification();
		return S_OK;
	}
};