#pragma once

#include <unordered_set>
#include <unordered_map>
#include <any>
#include "std_macros.h"


namespace Sandbox
{

	enum class SignalPriority : int
	{
		high = 0,
		medium,
		low
	};



	//////////////////////////////////
	/// Signal
	//////////////////////////////////

	/// <summary>
	/// Wrapper around a static function pointer, and a listener as void* wich lifetime must be guaranteed by user.
	/// Will call the function with additional data as first argument, and the stored void* as second argument
	/// </summary>
	/// <typeparam name="D">Type of data used by the callback</typeparam>
	template<typename MessageType>
	class SignalDelegate
	{

	public:
		SignalDelegate(void (*callback)(const MessageType&));
		SignalDelegate(void (*callback)(const MessageType&, void* const));
		SignalDelegate(void (*callback)(const MessageType&, void* const, const std::any&), std::any data);
		void operator()(void* const listener, const MessageType& message);
		void operator()(const MessageType& message);
	private:
		std::any m_additionalData;
		void (*m_callback)(const MessageType&, void* const, const std::any&);
		void (*m_callbackNoData)(const MessageType&, void* const);
		void (*m_callbackNoListenerNoData)(const MessageType&);

	};

	struct ListenerSignalPriority
	{
		ListenerSignalPriority(void* const Listener, std::any Signal, SignalPriority Priority)
			: listener(Listener), signal(Signal), priority(Priority)
		{

		}
		void* const listener;
		std::any signal;
		SignalPriority priority;
	};

	//////////////////////////////////
	/// SignalDispatcher
	//////////////////////////////////


	/// <summary>
	/// Holds a collection of SignalDelegates mapped with their TypeId (MessageType) and sorted by priority
	/// </summary>
	class SignalDispatcher
	{
	public:

		template<typename MessageType>
		void Push(SignalDelegate<MessageType> signal, SignalPriority priority);
		template<typename MessageType>
		void Push(void* const listener, SignalDelegate<MessageType> signal, SignalPriority priority);
		template<typename MessageType>
		void RemoveListenerFrom(void* listener);
		void RemoveListener(void* listener);
		template <typename MessageType>
		void Dispatch(const MessageType& signalData);

	private:
		struct compare
		{
			bool operator()(const ListenerSignalPriority& l, const ListenerSignalPriority& r) const
			{
				return l.priority < r.priority;
			}
		};
		std::unordered_map<int, std::multiset<ListenerSignalPriority, compare>> m_delegates;
		std::unordered_map<int, std::multiset<ListenerSignalPriority, compare>> m_delegatesNoListener;

	};

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////

	/// <summary>
	/// Base class for signal sender (like Inputs or Systems)
	/// Give the ability to call callbacks with any custom data type, to any type of listener.
	/// Internally hold a collection of Signals, wich are callbacks bound a listener.
	/// </summary>
	class SignalSender
	{
	public:
		virtual ~SignalSender(){};

		template <typename MessageType>
		void AddListener(void (*callback)(const MessageType&), SignalPriority priority = SignalPriority::medium);

		template <typename MessageType>
		void AddListener(void (*callback)(const MessageType&, void* const), void* const listener, 
			SignalPriority priority = SignalPriority::medium);

		template <typename MessageType>
		void AddListener(void (*callback)(const MessageType&, void* const, const std::any&), void* const listener, std::any data,
			SignalPriority priority = SignalPriority::medium);

		void RemoveListener(void* listener);
		template <typename MessageType>
		void RemoveListenerFrom(void* listener);
	protected:
		template<typename MessageType>
		void SendSignal(const MessageType& signalData);
	private:
		SignalDispatcher m_dispatcher;

	};
}
#include "Signals.tpp"