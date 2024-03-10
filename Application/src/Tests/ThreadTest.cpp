#include "pch.h"
#include "Sandbox/Core/Worker.h"
#include "ThreadTest.h"

#include "Sandbox/Core/Signal.h"
#include "Sandbox/Core/Delegate.h"

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