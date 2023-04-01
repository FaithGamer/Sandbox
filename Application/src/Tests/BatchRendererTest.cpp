#include "pch.h"

#include "BatchRendererTest.h"
#include "Render/Window.h"
#include "Render/BatchRenderer.h"
#include "Core/Time.h"
#include "Core/Delegate.h"
#include "Thread/Task.h"
#include "Thread/Worker.h"
#include "std_macros.h"

using namespace Sandbox;

void BatchRendererTest()
{
	Window window("hello window", Vec2i(500, 500));
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
			Delegate clear(&Window::Clear, &window);
			auto clearTask = makesptr<Task<void>>(clear);
			renderingThread.QueueTask(clearTask);

			Delegate beginScene(&BatchRenderer::BeginScene, &renderer, cam);
			sptr < Task<void, const Camera&>> beginSceneTask = makesptr <Task<void, const Camera&>>(beginScene);
			renderingThread.QueueTask(beginSceneTask);
		}
		else
		{
			window.Clear();
			renderer.BeginScene(cam);
		}

		int count = 0;
		auto white = Vec4f(1, 1, 1, 1);

		for (float i = -5; i < 5; i += 1.0f)
		{
			for (float j = -5; j < 5; j += 1.0f)
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
						Delegate<void, const Transform&, const sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&>
							drawQuad(&BatchRenderer::DrawTexturedQuad, &renderer, transform, texture1, texCoords, white);

						auto drawTask = makesptr <Task<void, const Transform&, const sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&>>(drawQuad);
						renderingThread.QueueTask(drawTask);
					}
					else
					{
						renderer.DrawTexturedQuad(transform, texture1, texCoords);
					}

				}
				else
				{
					if (threadRendering)
					{
						Delegate<void, const Transform&, const sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&>
							drawQuad(&BatchRenderer::DrawTexturedQuad, &renderer, transform, texture2, texCoords, white);

						auto drawTask = makesptr <Task<void, const Transform&, const sptr<Texture>&, const std::vector<Vec2f>&, const Vec4f&>>(drawQuad);
						renderingThread.QueueTask(drawTask);
					}
					else
					{
						renderer.DrawTexturedQuad(transform, texture2, texCoords);
					}
				}
				count++;
				
			}
		
		}
		
		if (threadRendering)
		{
			Delegate<void> endScene(&BatchRenderer::EndScene, &renderer);
			Delegate<void> render(&Window::Render, &window);

			sptr<Task<void>> endSceneTask = makesptr <Task<void>>(endScene);
			sptr<Task<void>> renderTask = makesptr <Task<void>>(render);

			renderingThread.QueueTask(endSceneTask);
			renderingThread.QueueTask(renderTask);

		}
		else
		{
			renderer.EndScene();
			window.Render();
		}
	}
}
