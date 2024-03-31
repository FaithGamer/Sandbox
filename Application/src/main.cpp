#include "pch.h"
#include "Sandbox/Engine.h"


#include "entt/entt.hpp"
#include "Sandbox/Core/Vec.h"
#include "Sandbox/Core/Time.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Core/std_macros.h"

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

#include <random>
#include <iostream>

namespace sb = Sandbox;

#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Core/Log.h"
#include "Sandbox/Core/Signal.h"
#include "Sandbox/Core/Easing.h"

void foo(int const* s)
{
	std::cout << *s << std::endl;
}
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
	MouseWheelTest();

	
	return 0;
}