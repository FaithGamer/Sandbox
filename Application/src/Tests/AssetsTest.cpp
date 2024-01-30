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

	auto assets = Assets::Instance();
    Asset<Config> config = assets->Get<Config>("jsontest.config");



	std::cout << config.Ptr()->GetFloat("Float");
}