#include "TypeId.h"

namespace sandbox
{
	//////////////////////////////////
	/// Signal
	//////////////////////////////////

	template <typename SignalData>
	SignalDelegate<SignalData>::SignalDelegate(void (*callback)(const SignalData&, void*, const std::any&))
		: m_callback(callback)
	{

	}

	template <typename SignalData>
	SignalDelegate<SignalData>::SignalDelegate(void (*callback)(const SignalData&, void*, const std::any&), std::any data)
		: m_callback(callback), m_additionalData(data)
	{

	}

	template <typename SignalData>
	void SignalDelegate<SignalData>::operator()(void * const listener, const SignalData& data)
	{
		std::invoke(m_callback, data, listener, m_additionalData);
	}

	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////

	template<typename SignalData>
	void SignalDispatcher::Push(void* listener, SignalDelegate<SignalData> signal, SignalPriority priority)
	{
		m_delegates[TypeId::GetId<SignalData>()].insert(ListenerSignalPriority(listener, signal, priority));
	}

	template<typename SignalData>
	void SignalDispatcher::RemoveListenerFrom(void* listener)
	{
		auto delegates_it = m_delegates.find(TypeId::GetId<SignalData>());
		if (delegates_it != m_delegates.end())
		{
			auto& listeners = delegates_it->second;
			for (auto listenerSignalPriority = listeners.begin(); listenerSignalPriority != listeners.end();)
			{
				if (listenerSignalPriority->listener == listener)
				{
					listeners.erase(listenerSignalPriority++);
				}
				else
				{
					listenerSignalPriority++;
				}
			}
		}
	}

	template <typename SignalData>
	void SignalDispatcher::Dispatch(const SignalData& signalData)
	{
		auto it_signals = m_delegates.find(TypeId::GetId<SignalData>());
		if (it_signals != m_delegates.end())
		{
			for (auto& signalPriority : it_signals->second)
			{
				std::any_cast<SignalDelegate<SignalData>>(signalPriority.signal)(signalPriority.listener, signalData);
			}
		}
	}

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////

	template <typename SignalData>
	void SignalSender::Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener)
	{
		m_dispatcher.Push(listener, SignalDelegate<SignalData>(callback), SignalPriority::medium);
	}

	template <typename SignalData>
	void SignalSender::Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener, SignalPriority priority)
	{
		m_dispatcher.Push(listener, SignalDelegate<SignalData>(callback) , priority);
	}

	template <typename SignalData>
	void SignalSender::Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener, SignalPriority priority, std::any data)
	{
		m_dispatcher.Push(listener, SignalDelegate<SignalData>(callback, data), priority);
	}
	template <typename SignalData>
	void SignalSender::RemoveListenerFrom(void * listener)
	{
		m_dispatcher.RemoveListenerFrom<SignalData>(listener);
	}

	template<typename SignalData>
	void SignalSender::SendSignal(const SignalData& signalData)
	{
		m_dispatcher.Dispatch(signalData);
	}
}