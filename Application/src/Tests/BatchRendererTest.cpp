#include "pch.h"

#include "BatchRendererTest.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Time.h"
#include "Sandbox/Delegate.h"
#include "Sandbox/Task.h"
#include "Sandbox/Worker.h"
#include "Sandbox/std_macros.h"
#include "Sandbox/Render/Shader.h"

#include "Sandbox/Engine.h"

using namespace Sandbox;

void BatchRendererTest()
{
	Engine::Init();
	Renderer2D renderer;

	Camera cam;
	cam.Pitch(0);
	cam.Yaw(0);
	cam.SetPosition({ 0, 0, 2 });

	sptr<Shader> otherShader = makesptr<Shader>("assets/shaders/batch_renderer.vert", "assets/shaders/batch_renderer2.frag");
	sptr<Texture> texture1 = makesptr<Texture>("assets/textures/trollface.png");
	sptr<Texture> texture2 = makesptr<Texture>("assets/textures/image.png");
	std::vector<Vec2f> texCoords{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
	};

	uint32_t pipeline = renderer.AddQuadPipelineUser(0, otherShader, nullptr);
	bool run = true;
	SDL_Event e;

	Clock clock;
	float minYaw = -20;
	float maxYaw = 20;

	WorkerThread renderingThread;
	renderingThread.StartThread();
	bool threadRendering = false;

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

		if (threadRendering)
		{
			Delegate clear(&Window::Clear);
			auto clearTask = makesptr<Task<void>>(clear);
			renderingThread.QueueTask(clearTask);

			Delegate beginScene(&Renderer2D::BeginScene, &renderer, cam);
			sptr < Task<void, const Camera&>> beginSceneTask = makesptr <Task<void, const Camera&>>(beginScene);
			renderingThread.QueueTask(beginSceneTask);
		}
		else
		{
			Window::ClearWindow();
			renderer.BeginScene(cam);
		}

		int count = 0;
		auto white = Vec4f(1, 1, 1, 1);

		for (float i = -25; i < 25; i += 1.0f)
		{
			for (float j = -25; j < 25; j += 1.0f)
			{
				float y = i / 100.0f;
				float x = j / 100.0f;
				Transform transform;
				transform.SetPosition(x, y, 0.0f);
				transform.SetScale(0.01f, 0.01f, 1.0f);
				transform.SetRotationZAxis((float)clock.GetElapsed() * 40);

				if (count % 2)
				{
					if (threadRendering)
					{
						Delegate<void, const Transform&, sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&, uint32_t>
							drawQuad(&Renderer2D::DrawTexturedQuad, &renderer, transform, texture1, texCoords, white, 0);

						auto drawTask = makesptr <Task<void, const Transform&, sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&, uint32_t>>(drawQuad);
						renderingThread.QueueTask(drawTask);
					}
					else
					{
						renderer.DrawTexturedQuad(transform, texture1, texCoords, white, pipeline);
					}

				}
				else
				{
					if (threadRendering)
					{
						Delegate<void, const Transform&, sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&, uint32_t>
							drawQuad(&Renderer2D::DrawTexturedQuad, &renderer, transform, texture2, texCoords, white, 0);

						auto drawTask = makesptr <Task<void, const Transform&, sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&, uint32_t>>(drawQuad);
						renderingThread.QueueTask(drawTask);
					}
					else
					{
						renderer.DrawTexturedQuad(transform, texture2, texCoords, white, 0);
					}
				}
				count++;
				
			}
		
		}
		
		if (threadRendering)
		{
			Delegate<void> endScene(&Renderer2D::EndScene, &renderer);
			Delegate<void> render(&Window::RenderWindow);

			sptr<Task<void>> endSceneTask = makesptr <Task<void>>(endScene);
			sptr<Task<void>> renderTask = makesptr <Task<void>>(render);

			renderingThread.QueueTask(endSceneTask);
			renderingThread.QueueTask(renderTask);

		}
		else
		{
			renderer.EndScene();
			Window::RenderWindow();
		}
	}
}
