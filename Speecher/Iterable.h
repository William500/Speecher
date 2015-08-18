#pragma once

template <typename T>
class Iterable :
	public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::WinRtClassicComMix>, ABI::Windows::Foundation::Collections::IIterable<T>>
{
private:
	std::vector<T> m_Items;

	class Iterator :
		public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::WinRtClassicComMix>, ABI::Windows::Foundation::Collections::IIterator<T>>
	{
	private:
		const Iterable<T>& m_Collection;
		int m_Index;

	public:
		Iterator(const Iterable<T>& collection) :
			m_Collection(collection),
			m_Index(0)
		{
		}

		virtual HRESULT STDMETHODCALLTYPE get_Current(T *current) override
		{
			if (m_Index < m_Collection.m_Items.size())
			{
				*current = m_Collection.m_Items[m_Index];
				return S_OK;
			}

			return E_BOUNDS;
		}

		virtual HRESULT STDMETHODCALLTYPE get_HasCurrent(boolean* hasCurrent) override
		{
			*hasCurrent = m_Index < m_Collection.m_Items.size();
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE MoveNext(boolean* hasCurrent) override
		{
			m_Index++;
			return get_HasCurrent(hasCurrent);
		}

		virtual HRESULT STDMETHODCALLTYPE GetMany(unsigned, T*, unsigned*) override
		{
			Assert(false);
			return E_NOTIMPL;
		}
	};

	friend class Iterator;

public:
	Iterable(std::vector<T> items) :
		m_Items(std::move(items))
	{
	}

	virtual HRESULT STDMETHODCALLTYPE First(ABI::Windows::Foundation::Collections::IIterator<T>** first) override
	{
		auto result = Make<Iterator>(*this);
		*first = result.Get();
		(*first)->AddRef();
		return S_OK;
	}
};