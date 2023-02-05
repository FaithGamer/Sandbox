#include "pch.h"
#include "Log.h"
#include "SignalTest.h"

using namespace Sandbox;

/* Signals is an implementation of the Listener pattern, here's an example */

	//Creates structures that hold the data sent by the signal, it can be anything we wanna send on a message
struct FooMessage
{
	int integer = 0;
	std::string characters = "0";
};

struct BarMessage
{
	std::string characters = "0";
	std::string moreCharacters = "0";
};

int fooId = 0;
int foobarId = 0;

/* There is three way of receiving signals
The function receiving the signal must follow one of the three following parameters pattern */

//->First one is receiving message on a free function, it just takes a const ref to the message type
void OnFoo1(const FooMessage& message)
{
	std::cout << "OnFoo1 Receive on a free function: " << message.integer << " & " << message.integer << std::endl;
}

// The two others one involve an object that is gonna listen to the signal
// note that the function receiving the signal MUST be static and public
class FooListener
{

public:

	FooListener()
	{
		m_id = fooId++;
	}

	//->Second one we have a void* const pointer, that is guaranteed of being of the same class
	static void ReceiveFooMessage(const FooMessage& message, void* const listener)
	{
		//The variable listener here will be the instance receiving the message,
		//So we can perform the static_cast with no worries and call the processing methods
		static_cast<FooListener*>(listener)->OnFoo2(message);
	}

	/*->Third one, we also have any type of data that is gonna remain the same at any time this method is called,
	and will be determined at the time of starting the listening.
	One must know what type of data is sent to perform the adequat cast on data. */
	static void ReceiveFooMessage(const FooMessage& message, void* const listener, const std::any& data)
	{
		//The variable listener here will be the instance receiving the message.
		static_cast<FooListener*>(listener)->OnFoo3(message, data);
	}

private:

	//The methods that will actually process the data within the receiving object:

	void OnFoo2(const FooMessage& message)
	{
		std::cout << "FooListener " << m_id << ", Receive on a member method : " << message.integer << " & " << message.characters << std::endl;
	}
	void OnFoo3(const FooMessage& message, const std::any& data)
	{
		//Here we decided the additional data is integer
		int additionalData = std::any_cast<int>(data);

		std::cout << "FooListener " << m_id << ", Receive on a member method with additional data : " <<
			message.integer << " & " <<
			message.characters << " data: " <<
			additionalData << std::endl;
	}
	int m_id;
};

//Let's have another listener, that will also listen other type of message
class FooBarListener
{

public:

	FooBarListener()
	{
		m_id = foobarId++;
	}

	static void ReceiveBarMessage(const BarMessage& message, void* const listener)
	{
		static_cast<FooBarListener*>(listener)->OnBar(message);
	}

	static void ReceiveFooMessage(const FooMessage& message, void* const listener)
	{
		static_cast<FooBarListener*>(listener)->OnFoo(message);
	}

private:

	void OnBar(const BarMessage& message)
	{
		std::cout << "FoobarListener " << m_id << ", receiving Bar: " << message.characters << " & " << message.moreCharacters << std::endl;
	}
	void OnFoo(const FooMessage& message)
	{
		std::cout << "FoobarListener " << m_id << ", receiving Foo " << message.integer << " & " << message.characters << std::endl;
	}

	int m_id;
};

// We need a SignalSender that is gonna store the listeners and perform the signal calls
class Sender : public SignalSender
{
public:
	void SendFoo()
	{
		FooMessage message;
		message.integer = 69;
		message.characters = "blaze it";

		/* The function SendSignal is inherited by SignalSender and will call the appropriate listeners
		according to the message type they listen to. */
		SendSignal(message);
	}

	void SendBar()
	{
		BarMessage message;
		message.characters = "This is";
		message.moreCharacters = "Bar";
		SendSignal(message);
	}
};

namespace Sandbox
{
	void TestSignal()
	{
		LOG_INFO("Test Signal");

		//Create a sender
		Sender sender;

		std::vector<FooListener> foo;
		std::vector <FooBarListener> foobar;

		//create listeners
		for (int i = 0; i < 3; i++)
		{
			foo.push_back(FooListener());
			foobar.push_back(FooBarListener());
		}

		//Add listener for the Foo message (deduced by the argument of ReceiveFooMessageMethod)
		sender.AddListener(&FooListener::ReceiveFooMessage, &foo[0]);

		//With additional data (that is gonna remain the same on every call)
		sender.AddListener(&FooListener::ReceiveFooMessage, &foo[1], std::any((int)42));
		sender.AddListener(&FooListener::ReceiveFooMessage, &foo[2], std::any((int)420));
		
		//Listening on foo with another type of listeners
		sender.AddListener(&FooBarListener::ReceiveFooMessage, &foobar[0]);

		//Listening on foo with an higher priority (default is medium)
		sender.AddListener(&FooBarListener::ReceiveFooMessage, &foobar[1], SignalPriority::high);

		//Listening on foo AND bar
		sender.AddListener(&FooBarListener::ReceiveFooMessage, &foobar[2]);
		sender.AddListener(&FooBarListener::ReceiveBarMessage, &foobar[2]);

		std::cout << std::endl;

		//Send signals
		sender.SendFoo();
		sender.SendBar();

		//Remove a listener from on type of message only
		sender.RemoveListenerFrom<BarMessage>(&foobar[2]);

		std::cout << std::endl;
		std::cout << "Removed Foobar2 from listening Bar Message." << std::endl;
		std::cout << std::endl;

		sender.SendFoo();
		sender.SendBar();

		std::cout << std::endl;

	}
}