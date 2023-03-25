#include "Core/TypeId.h"
#include "Core/Signals.h"

namespace Sandbox
{


	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////


	template<typename MessageType>
	void SignalDispatcher::Push(std::function<void(const MessageType&)> callback, void* const listener, SignalPriority priority)
	{
		std::any Any = std::make_any<std::function<void(const MessageType&)>>(callback);
		m_callbacks[TypeId::GetId<MessageType>()].insert(SignalCallback(Any, listener, priority));
	}

	template<typename MessageType>
	void SignalDispatcher::Push(void(*callback)(const MessageType&), SignalPriority priority)
	{
		std::any Any = std::make_any<void(*)(const MessageType&) >> (callback);
		m_callbacks[TypeId::GetId<MessageType>()].insert(SignalCallback(Any, listener, priority));
	}

	template<typename MessageType>
	void SignalDispatcher::RemoveListener(void* const listener)
	{
		auto delegates_it = m_callbacks.find(TypeId::GetId<MessageType>());
		if (delegates_it == m_callbacks.end())
		{
			return;
		}
		auto& listeners = delegates_it->second;
		for (auto functionPriority = listeners.begin(); functionPriority != listeners.end();)
		{
			if (functionPriority->listener == listener)
			{
				listeners.erase(functionPriority++);
			}
			else
			{
				functionPriority++;
			}
		}
	}

	template <typename MessageType>
	void SignalDispatcher::Dispatch(const MessageType& signalData)
	{
		auto it_signals = m_callbacks.find(TypeId::GetId<MessageType>());
		if (it_signals == m_callbacks.end())
		{
			return;
		}
		for (auto& signalPriority : it_signals->second)
		{
			std::any_cast<std::function<void(MessageType)>>(signalPriority.callback)(signalData);
		}
	}

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////


	template <typename ListenerType, typename MessageType>
	void SignalSender::AddListener(void (ListenerType::* callback)(const MessageType&), ListenerType* const listener, SignalPriority priority)
	{
		std::function<void(MessageType)> function = std::bind(callback, listener, std::placeholders::_1);
		m_dispatcher.Push<MessageType>(function, listener, priority);
	}

	template <typename MessageType>
	void SignalSender::AddListener(void (*callback)(const MessageType&), SignalPriority priority)
	{
		m_dispatcher.Push<MessageType>(callback, priority);
	}

	template <typename MessageType>
	void SignalSender::RemoveListener(void* listener)
	{
		m_dispatcher.RemoveListener<MessageType>(listener);
	}

	template <typename MessageType>
	void SignalSender::RemoveListener(void (*callback)(const MessageType&))
	{

	}

	void SignalSender::RemoveListenerFromAll(void* const listener)
	{

	}

	template<typename MessageType>
	void SignalSender::SendSignal(const MessageType& signalData)
	{
		m_dispatcher.Dispatch(signalData);
	}
}