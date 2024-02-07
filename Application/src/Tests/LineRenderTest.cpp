#include "pch.h"

#include "LineRenderTest.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Random.h"
#include "Sandbox/Math.h"

using namespace Sandbox;

void LineRenderTest()
{
	Engine::Init();

	Systems::CreateWorld();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	cam.worldToScreenRatio = 0.05;
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);

	Entity line;


	auto trans = line.AddComponent<Transform>();
	//trans->SetScale(5, 5, 5);
	trans->SetPosition(-5, 0, 0);
	//trans->SetRotationZAxis(40);
	std::cout << Vec3f(-0.2, 1, 1).Normalized().Cross(Vec3f(-0.1, 1, 1).Normalized()).ToString() << std::endl;
	auto lineRender = line.AddComponent<LineRenderer>(200);
	auto p = Vec3f(0, 0, 0);
	Random::Seed(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	int lastAngle = 0;
	lineRender->SetWidth(1, 0);
	lineRender->SetWidth(4, 1);
	lineRender->SetWidth(3, 2);
	lineRender->SetWidth(0.2,3);
	lineRender->SetWidth(1, 4);
	for (int i = 0; i < 20; i++)
	{
		lastAngle += Random::Range(-20, 20);
		//p += Vec3f(Math::AngleToVec(lastAngle));
		p += {1, 0, 0};
		lineRender->AddPoint(p);
	}
	
	Engine::Launch();
}