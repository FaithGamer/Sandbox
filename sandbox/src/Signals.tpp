#include "TypeId.h"

namespace Sandbox
{
	//////////////////////////////////
	/// Signal
	//////////////////////////////////

	template <typename MessageType>
	SignalDelegate<MessageType>::SignalDelegate(void (*callback)(const MessageType&))
		: m_callbackNoData(callback)
	{

	}

	template <typename MessageType>
	SignalDelegate<MessageType>::SignalDelegate(void (*callback)(const MessageType&, void*, const std::any&))
		: m_callback(callback)
	{

	}

	template <typename MessageType>
	SignalDelegate<MessageType>::SignalDelegate(void (*callback)(const MessageType&, void*, const std::any&), std::any data)
		: m_callback(callback), m_additionalData(data)
	{

	}

	template <typename MessageType>
	void SignalDelegate<MessageType>::operator()(void* const listener, const MessageType& message)
	{
		std::invoke(m_callback, message, listener, m_additionalData);
	}

	template <typename MessageType>
	void SignalDelegate<MessageType>::operator()(const MessageType& message)
	{
		std::invoke(m_callbackNoData, message);
	}

	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////

	template<typename MessageType>
	void SignalDispatcher::Push(SignalDelegate<MessageType> signal, SignalPriority priority)
	{
		m_delegatesNoData[TypeId::GetId<MessageType>()].insert(ListenerSignalPriority(nullptr, signal, priority));
	}

	template<typename MessageType>
	void SignalDispatcher::Push(void* listener, SignalDelegate<MessageType> signal, SignalPriority priority)
	{
		m_delegates[TypeId::GetId<MessageType>()].insert(ListenerSignalPriority(listener, signal, priority));
	}

	template<typename MessageType>
	void SignalDispatcher::RemoveListenerFrom(void* listener)
	{
		auto delegates_it = m_delegates.find(TypeId::GetId<MessageType>());
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

	template <typename MessageType>
	void SignalDispatcher::Dispatch(const MessageType& signalData)
	{
		{
			auto it_signals = m_delegates.find(TypeId::GetId<MessageType>());
			if (it_signals != m_delegates.end())
			{
				for (auto& signalPriority : it_signals->second)
				{
					std::any_cast<SignalDelegate<MessageType>>(signalPriority.signal)(signalPriority.listener, signalData);
				}
			}
		}

		auto it_signals = m_delegatesNoData.find(TypeId::GetId<MessageType>());
		if (it_signals != m_delegatesNoData.end())
		{
			for (auto& signalPriority : it_signals->second)
			{
				std::any_cast<SignalDelegate<MessageType>>(signalPriority.signal)(signalData);
			}
		}
	}

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////

	template <typename MessageType>
	void SignalSender::AddListener(void (*callback)(const MessageType&), SignalPriority priority)
	{
		m_dispatcher.Push(SignalDelegate<MessageType>(callback), priority);
	}

	template <typename MessageType>
	void SignalSender::AddListener(void (*callback)(const MessageType&, void* const, const std::any&), void* const listener, SignalPriority priority, std::any data)
	{
		m_dispatcher.Push(listener, SignalDelegate<MessageType>(callback, data), priority);
	}

	template <typename MessageType>
	void SignalSender::RemoveListenerFrom(void* listener)
	{
		m_dispatcher.RemoveListenerFrom<MessageType>(listener);
	}

	template<typename MessageType>
	void SignalSender::SendSignal(const MessageType& signalData)
	{
		m_dispatcher.Dispatch(signalData);
	}
}