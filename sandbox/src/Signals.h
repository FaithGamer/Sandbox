#pragma once

#include <unordered_set>
#include "std_macros.h"


namespace sandbox
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
	template<typename SignalData>
	class SignalDelegate
	{

	public:
		SignalDelegate(void (*callback)(const SignalData&, void* const, const std::any&));
		SignalDelegate(void (*callback)(const SignalData&, void* const, const std::any&), std::any data);
		void operator()(void* const listener, const SignalData& data);
	private:
		std::any m_additionalData;
		void (*m_callback)(const SignalData&, void* const, const std::any&);

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
	/// Holds a collection of SignalDelegates mapped with their TypeId (SignalData) and sorted by priority
	/// </summary>
	class SignalDispatcher
	{
	public:

		template<typename SignalData>
		void Push(void* const listener, SignalDelegate<SignalData> signal, SignalPriority priority);
		template<typename SignalData>
		void RemoveListenerFrom(void* listener);
		void RemoveListener(void* listener);
		template <typename SignalData>
		void Dispatch(const SignalData& signalData);

	private:
		struct compare
		{
			bool operator()(const ListenerSignalPriority& l, const ListenerSignalPriority& r) const
			{
				return l.priority < r.priority;
			}
		};
		std::unordered_map<int, std::multiset<ListenerSignalPriority, compare>> m_delegates;

	};

	//////////////////////////////////
	/// SignalSender
	//////////////////////////////////

	/// <summary>
	/// Base class for signal sender like Inputs or Systems
	/// Give the ability to call callbacks with any custom data type, to any type of listener.
	/// Internally hold a collection of Signals, wich are callbacks bound a listener.
	/// </summary>
	class SignalSender
	{
	public:
		/*template <typename SignalData>
		void Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener);
		template <typename SignalData>
		void Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener, SignalPriority priority);*/
		template <typename SignalData>
		void Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener, 
			SignalPriority priority = SignalPriority::medium, std::any data = std::any());
		template <typename SignalData>
		void Listen(void (*callback)(const SignalData&, void* const, const std::any&), void* const listener,
			std::any data = std::any(), SignalPriority priority = SignalPriority::medium);

		void RemoveListener(void* listener);
		template <typename SignalData>
		void RemoveListenerFrom(void* listener);
	protected:
		template<typename SignalData>
		void SendSignal(const SignalData& signalData);
	private:
		SignalDispatcher m_dispatcher;

	};
}
#include "Signals.tpp"