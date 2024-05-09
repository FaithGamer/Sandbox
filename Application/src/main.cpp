#include "pch.h"

#include "Tests/ImGuiSystemTest.h"
#include "Tests/ECSTest.h"
#include "Tests/UniformBlockTest.h"
#include "Tests/RendererTest.h"
#include "Tests/FrameBufferTest.h"
#include "Tests/SpriteRenderTest.h"
#include "Tests/InputSystemTest.h"
#include "Tests/BillboardTest.h"
#include "Tests/ImGuiSystemTest.h"
#include "Tests/BlendingTest.h"
#include "Tests/JsonTest.h"
#include "Tests/AssetsTest.h"
#include "Tests/LineRenderTest.h"
#include "Tests/EarcutTest.h"
#include "Editor/EntryPoint.h"
#include "Tests/RaycastTest.h"
#include "Tests/PhysicsQueries.h"
#include "Tests/LayerBlending.h"
#include "Tests/ColliderDebug.h"
#include "Tests/MouseWheelTest.h"
#include "Tests/AnimTest.h"
#include "Tests/AudioTest.h"
#include "Tests/CrashDebug.h"


int main(int argc, char** argv)
{
	//UniformBlockTest();
	//BillboardTest();
	//RendererTest();
	//SpriteRenderTest();
	//ImGuiSystemTest();
	//InputSystemTest();

	//Editor();
	//BlendingTest();
	//VectorUnitTest();
	//JsonTest();


	//AssetsTest();
	//EarcutTest();
	//LineRenderTest();

	//RaycastTest();
	//ECSTest();

	//PhysicsQueries();

	//LayerBlending();
	//ColliderDebug();
	//MouseWheelTest();

	//AnimTest();

	//AudioTest();

	CrashDebug();
	
	return 0;
}