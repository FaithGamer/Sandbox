#include "pch.h"
#include "Sandbox/Engine.h"


#include "entt/entt.hpp"
#include "Sandbox/Vec.h"
#include "Sandbox/Time.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/std_macros.h"

#include "Tests/ImGuiSystemTest.h"
#include "Tests/ECSTest.h"
#include "Tests/UniformBlockTest.h"
#include "Tests/RendererTest.h"
#include "Tests/FrameBufferTest.h"
#include "Tests/SpriteRenderTest.h"

#include <random>
#include <iostream>

namespace sb = Sandbox;

#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Log.h"

int main(int argc, char** argv)
{

	SpriteRenderTest();
	//RendererTest();

	return 0;
}