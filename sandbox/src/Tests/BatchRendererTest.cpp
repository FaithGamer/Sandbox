#include "pch.h"

#include "BatchRendererTest.h"
#include "Render/WindowGLContext.h"
#include "Render/BatchRenderer.h"
#include "Core/Time.h"
#include "std_macros.h"

using namespace Sandbox;

void Sandbox::BatchRendererTest()
{
	WindowGLContext window("hello window", Vec2i(500, 500));
	BatchRenderer renderer;

	Camera cam;
	cam.Pitch(0);
	cam.Yaw(0);
	cam.SetPosition({ 0, 0, 2 });


	sptr<Texture> texture1 = makesptr<Texture>("assets/textures/trollface.png");
	sptr<Texture> texture2 = makesptr<Texture>("assets/textures/image.png");
	std::vector<Vec2f> texCoords{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
	};

	bool run = true;
	SDL_Event e;

	Clock clock;
	float minYaw = -20;
	float maxYaw = 20;

	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{

			if (e.type == SDL_QUIT)
			{
				run = false;
			}

		}

		float yaw = std::sin(clock.GetElapsed()) * maxYaw;
		float zoom = 0.6 + std::sin(clock.GetElapsed()) * 0.5;
		//cam.SetYaw(yaw);
		cam.SetPosition({ 0, 0, zoom });

		window.Clear();

		renderer.BeginScene(cam);
		int count = 0;
		for (float i = -50; i < 50; i += 1.0f)
		{
			for (float j = -50; j < 50; j += 1.0f)
			{
				float y = i / 100.0f;
				float x = j / 100.0f;
				Transform transform;
				transform.SetPosition(x, y, 0.0f);
				transform.SetScale(0.01f, 0.01f, 1.0f);
				transform.SetRotationZAxis((float)clock.GetElapsed()*40);
				if (count % 2)
					renderer.DrawTexturedQuad(transform, texture1, texCoords);
				else
					renderer.DrawTexturedQuad(transform, texture2, texCoords);
				count++;
			}
		}

		renderer.EndScene();
		window.Render();
	}
}
