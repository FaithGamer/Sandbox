#include "pch.h"

#include "AssetsTest.h"
#include "Sandbox/Render.h"
#include "Sandbox/Assets.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Json.h"

using namespace Sandbox;

void AssetsTest()
{
	Engine::Init();

    Asset<Config> config = Assets::Get<Config>("jsontest.config");

	auto intptr = makesptr<int>(2);
	auto ptr2 = intptr;

	std::cout << config.Ptr()->GetFloat("Float");
}