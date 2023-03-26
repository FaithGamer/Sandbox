#include "pch.h"

#include "Core/Log.h"
#include "Tests/SignalTest.h"
#include "Tests/EnttTest.h"
#include "Tests/ToolboxTest.h"
#include "Tests/UniformBlockTest.h"
#include "Tests/BillboardTest.h"
#include "Tests/BatchRendererTest.h"
#include "Tests/TextureUnitTest.h"
#include "Tests/ThreadTest.h"
using namespace Sandbox;


int main(int argc, char* argv[])
{

	Log::Init();
	LOG_INFO("Logger initialiazed");

	ThreadTest();
	TestSignal();
	//TestEntt();
	//TestToolbox();
	//UniformBlockTest();
	//BatchRendererTest();
	//TextureUnitTest();
	return 0;

}