#include "pch.h"
#include "Thread/Thread.h"
#include "ThreadTest.h"

#include "Core/Signal.h"
#include "Core/Delegate.h"

using namespace Sandbox;

template <class F, class... Args>
void VariadicFunction(F&& function, Args&&... args)
{
	std::invoke(function, args);
}

void SomeFunction(int Int, std::string String)
{
	std::cout << Int << " " << String << std::endl;
}

void foo(int a, float b, const char c) {

	std::cout << a << b << c << std::endl;
	// Do something with the arguments...
}

void bar(const char* c, double d) {
	// Do something with the arguments...
}
class MyClass {
public:
	static void add(int x, int y) { std::cout << x + y << std::endl; }
	void remove(int x, int y)
	{
		std::cout << x - y << std::endl;
	}
};

struct pouet
{
	int i = 8;
};

class Listener
{
public:
	void OnSignal(const pouet& data)
	{
		std::cout << data.i << std::endl;
	}
	void Callback(int a, float b)
	{
		std::cout << a << " " << b << std::endl;
	}
};

void Callback(int a, int* b)
{
	std::cout << a << " " << b << std::endl;
}



template<typename T>
class O
{
public:
	O(T* ptr) : p(ptr) {}
	T* p;
};

namespace Sandbox

{


	void ThreadTest()
	{
		/*WorkerThread worker;
		worker.StartThread();
		sptr<TaskData> data = makesptr<TaskData>();
		data->data = 1;
		worker.QueueTask(data);

		for (int i = 0; i < 1000; i++)
		{
			std::cout << "main thread" << std::endl;
		}*/

		//sptr O ptr = makesptr<O>(nullptr);

		Listener list;

		Delegate delegate(&Callback);

		Delegate other(delegate);

		other.Call();


	}


}