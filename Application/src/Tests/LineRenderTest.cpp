#include "pch.h"

#include "LineRenderTest.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Core/Random.h"
#include "Sandbox/Core/Math.h"

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

		ForeachComponents<LineRenderer>([this, delta](LineRenderer& line)
			{
				for (int i = 0; i < 5; i++)
				{
					float t = Math::Lerp(0.2, 2, (Math::Sin((float)timer + i * 75) + 1 ) / 2);
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


	/// line____


	/*Entity line = Entity::Create();

	auto trans = line.AddComponent<Transform>();

	trans->SetPosition(-5, -5, 0);

	auto lineRender = line.AddComponent<LineRenderer>(200);
	auto p = Vec3f(0, 0, 0);
	Random::Seed((unsigned int)std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
	int lastAngle = 0;

	lineRender->SetEndCapVertices(21);
	for (int i = 0; i < 70; i++)
	{
		lastAngle += Random::Range(-5, 5);
		p += Math::AngleToVec(lastAngle)*0.2;
		lineRender->AddPoint(p);
	}*/

	/// wire _____

	Entity wireEnt = Entity::Create();

	auto wire = wireEnt.AddComponent<WireRender>(29);
	wireEnt.AddComponent<Transform>();
	wire->SetLayer(Renderer2D::GetLayerId("DebugLayer"));
	wire->AddPoint({ 0, 0, 0 });
	wire->AddPoint({ 4, 0, 0 });
	wire->AddPoint({ 7, 2, 0 });
	wire->AddPoint({ 3, 6, 0 });
	wire->AddPoint({ 5, 2, 0 });


	
	Engine::Launch();
}