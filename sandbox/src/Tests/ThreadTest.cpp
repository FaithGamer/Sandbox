#include "pch.h"
#include "Thread\Thread.h"
#include "ThreadTest.h"

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
	void add(int x, int y) { std::cout << x + y << std::endl; }
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



		MyClass object;
		std::function<void(int)> fun = std::bind(&MyClass::add, &object, 4, std::placeholders::_2);
		fun(4);



	}


}