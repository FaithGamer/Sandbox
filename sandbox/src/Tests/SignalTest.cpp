#include "pch.h"
#include "Core/Log.h"
#include "SignalTest.h"

using namespace Sandbox;

struct AdditionSignal
{
	int left;
	int right;
};

int id = 0;

class Listener
{
public:
	void OnAddition(AdditionSignal signal)
	{
		std::cout << "Addition received on object_" << m_id << " addition: " << signal.left << " + " << signal.right << std::endl;
	}
	int m_id = id++;
};
void OnAdditionFree(AdditionSignal signal)
{
	std::cout << "Addition received on freefunction addition: " << signal.left << " + " << signal.right << std::endl;
}

class Sender
{
public:
	void Addition(int left, int right)
	{
		AdditionSignal a(left, right);
		additionSignal.SendSignal(a);
	}
	SignalSender<AdditionSignal> additionSignal;
};
namespace Sandbox
{
	void TestSignal()
	{
		Listener receiver;
		Sender sender;

		Delegate del(&OnAdditionFree);
		std::cout << del.IsSameFunction(&OnAdditionFree) << std::endl;

		//del.Call(AdditionSignal());

		sender.additionSignal.AddListener(&Listener::OnAddition, &receiver);
		sender.additionSignal.AddListener(&OnAdditionFree);

		sender.Addition(4, 8);
		sender.additionSignal.RemoveListener(&receiver);

		sender.Addition(2, 0);
		sender.additionSignal.RemoveListener(&OnAdditionFree);

		sender.Addition(3, 5);


	}
}