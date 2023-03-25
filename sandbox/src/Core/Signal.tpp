#include "Core/TypeId.h"
#include "Core/Signal.h"

namespace Sandbox
{
	template <typename SignalType>
	template <typename ListenerType>
	void SignalSender<SignalType>::AddListener(void (ListenerType::* callback)(const SignalType&), ListenerType* const listener, SignalPriority priority)
	{
		std::function<void(SignalType)> function = std::bind(callback, listener, std::placeholders::_1);
		m_listeners.insert(SignalCallback<SignalType>(function, listener, priority));
	}

	template <typename SignalType>
	void SignalSender<SignalType>::AddListener(void (*callback)(const SignalType&), SignalPriority priority)
	{
		std::function<void(SignalType)> function = std::bind(callback, std::placeholders::_1);
		m_listenersFreeFunction.insert(SignalCallbackFreeFunction<SignalType>(callback, priority));
	}

	template <typename SignalType>
	void SignalSender<SignalType>::RemoveListener(void* const listener)
	{
		for (auto callback = m_listeners.begin(); callback != m_listeners.end();)
		{
			if (callback->listener == listener)
			{
				m_listeners.erase(callback++);
			}
			else
			{
				callback++;
			}
		}
	}

	template <typename SignalType>
	void SignalSender<SignalType>::RemoveListener(void (*callback)(const SignalType&))
	{
		//to do
	}

	template<typename SignalType>
	void SignalSender<SignalType>::SendSignal(const SignalType& signal)
	{
		for (const SignalCallback<SignalType>& listener : m_listeners)
		{
			listener.callback(signal);
		}
		for (const SignalCallbackFreeFunction<SignalType>& listener : m_listenersFreeFunction)
		{
			listener.callback(signal);
		}
	}
}