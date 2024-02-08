#include "pch.h"

#include "LineRenderTest.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Random.h"
#include "Sandbox/Math.h"

using namespace Sandbox;

class LineSystem : public System
{
public:
	void OnStart() override
	{

	}
	void OnUpdate(Time delta) override
	{
		timer += (float)delta * 100;

		ForEachComponent<LineRenderer>([this, delta](LineRenderer& line)
			{
				for (int i = 0; i < 5; i++)
				{
					float t = (Math::Sin((float)timer + i * 90) + 1 ) / 2 * 3;
					line.SetWidth(t, i);
				}
			});
	}

	std::vector<float> widths;
	Time timer;
};
void LineRenderTest()
{
	Engine::Init();

	Systems::CreateWorld();
	Systems::Push<LineSystem>();

	Camera cam;
	cam.SetOrthographic(true);
	Systems::SetMainCamera(&cam);
	cam.worldToScreenRatio = 0.05;
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);

	Entity line;


	auto trans = line.AddComponent<Transform>();
	//trans->SetScale(5, 5, 5);
	trans->SetPosition(-12, 0, 0);
	//trans->SetRotationZAxis(40);
	std::cout << Vec3f(-0.2, 1, 1).Normalized().Cross(Vec3f(-0.1, 1, 1).Normalized()).ToString() << std::endl;
	auto lineRender = line.AddComponent<LineRenderer>(200);
	auto p = Vec3f(0, 0, 0);
	Random::Seed(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	int lastAngle = 0;
	lineRender->SetWidth(0, 0);
	lineRender->SetWidth(1, 1);
	lineRender->SetWidth(3, 2);
	lineRender->SetWidth(4, 3);
	lineRender->SetWidth(0, 4);
	for (int i = 0; i < 70; i++)
	{
		lastAngle += Random::Range(-20, 20);
		//p += Vec3f(Math::AngleToVec(lastAngle));
		p += {0.3, 0, 0};
		lineRender->AddPoint(p);
	}
	
	Engine::Launch();
}