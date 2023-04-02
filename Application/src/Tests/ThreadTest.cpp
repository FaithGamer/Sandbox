#include "pch.h"
#include "Sandbox/Worker.h"
#include "ThreadTest.h"

#include "Sandbox/Signal.h"
#include "Sandbox/Delegate.h"

using namespace Sandbox;



void task(int j)
{
	for (int i = 0; i < 1; i++)
	{
		std::cout << j << std::endl;
	}
}

void task2(int j)
{
	for (int i = 0; i < 1; i++)
	{
		std::cout << j << std::endl;
	}
}


void task3(int j)
{
	for (int i = 0; i < 1; i++)
	{
		std::cout << j << std::endl;
	}
}

class Bunny
{
public:
	void manger()
	{

	}
};


void Peter(const sptr<Bunny>& constref, std::vector<int> vec)
{
	std::cout << constref << std::endl;
}


namespace Sandbox

{

	void ThreadTest()
	{



		WorkerThread worker;
		worker.StartThread();


		Delegate d1(&task, 1);
		Delegate d2(&task2, 2);
		Delegate d3(&task3, 3);

		auto t1 = makesptr<Task<void, int>>(d1);
		auto t2 = makesptr<Task<void, int>>(d2);
		auto t3 = makesptr<Task<void, int>>(d3);
		worker.QueueTask(t1);
		worker.QueueTask(t2);
		worker.QueueTask(t3);



	}


}