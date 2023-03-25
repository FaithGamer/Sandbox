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
	void OnAddition(const AdditionSignal& signal)
	{
		std::cout << "Addition received on object_" << m_id << " addition: " << signal.left << " + " << signal.right << std::endl;
	}
	int m_id = id++;
};

class Sender
{
public:
	void Addition(int left, int right)
	{
		additionSignal.SendSignal(AdditionSignal(left, right));
	}
	SignalSender<AdditionSignal> additionSignal;
};
namespace Sandbox
{
	void TestSignal()
	{
		Listener receiver;
		Sender sender;

		sender.additionSignal.AddListener(&Listener::OnAddition, &receiver);
	

		sender.Addition(4, 8);
		sender.additionSignal.RemoveListener(&receiver);

		sender.Addition(2, 0);
	}
}