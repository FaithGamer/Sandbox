#pragma once

#include <unordered_set>
#include <unordered_map>
#include <set>
#include <any>
#include <functional>
#include "std_macros.h"


namespace Sandbox
{

	enum class SignalPriority : int
	{
		high = 0,
		medium,
		low
	};

	struct SignalCallback
	{
		SignalCallback(std::any Function, void* const Listener, SignalPriority Priority)
			: callback(Function), listener(Listener), priority(Priority)
		{

		}
		void* const listener;
		std::any callback;
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
		void Push(std::function<void(const MessageType&)> callback, void* const listener, SignalPriority priority);
		template<typename MessageType>
		void Push(void(*callback)(const MessageType&), SignalPriority priority);
		template<typename MessageType>
		void RemoveListener(void* const listener);
		void RemoveListenerFromAll(void* const listener);
		template <typename MessageType>
		void Dispatch(const MessageType& signalData);

	private:
		struct compare
		{
			bool operator()(const SignalCallback& l, const SignalCallback& r) const
			{
				return l.priority < r.priority;
			}
		};
		std::unordered_map<int, std::multiset<SignalCallback, compare>> m_callbacks;

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
		virtual ~SignalSender() {};



		template <typename ListenerType, typename MessageType>
		void AddListener(void (ListenerType::* callback)(const MessageType&), ListenerType* const listener, SignalPriority priority = SignalPriority::medium);
		template <typename MessageType>
		void AddListener(void (*callback)(const MessageType&), SignalPriority priority = SignalPriority::medium);




		template <typename MessageType>
		void RemoveListener(void (*callback)(const MessageType&));

		template <typename MessageType>
		void RemoveListener(void* const listener);

		void RemoveListenerFromAll(void* const listener);
	protected:
		template<typename MessageType>
		void SendSignal(const MessageType& signalData);
	private:
		SignalDispatcher m_dispatcher;

	};
}
#include "Signals.tpp"