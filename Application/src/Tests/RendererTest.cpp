#include "pch.h"

#include "RendererTest.h"

#include "Sandbox/Engine.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/InputSystem.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/Input/Inputs.h"
#include "Sandbox/Input/ButtonInput.h"

#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Render/Window.h"


using namespace Sandbox;

class CameraMovementSystem : public System
{

public:
	void OnStart() override
	{
		auto inputs = Inputs::CreateInputMap();
		auto button = inputs->CreateButtonInput("MoveCamFront");
		button->AddKey(KeyScancode::W);
		button->SetSendSignalOnRelease(true);
		button->signal.AddListener(&CameraMovementSystem::OnMoveFront, this);

		button = inputs->CreateButtonInput("MoveCamBack");
		button->AddKey(KeyScancode::S);
		button->SetSendSignalOnRelease(true);
		button->signal.AddListener(&CameraMovementSystem::OnMoveBack, this);

		button = inputs->CreateButtonInput("MoveCamLeft");
		button->AddKey(KeyScancode::A);
		button->SetSendSignalOnRelease(true);
		button->signal.AddListener(&CameraMovementSystem::OnMoveLeft, this);

		button = inputs->CreateButtonInput("MoveCamRight");
		button->AddKey(KeyScancode::D);
		button->SetSendSignalOnRelease(true);
		button->signal.AddListener(&CameraMovementSystem::OnMoveRight, this);
	}

	void OnUpdate(Time delta) override
	{
		ForeachEntities(&CameraMovementSystem::MoveCamera);
	}

	void MoveCamera(Entity entity, Camera& camera)
	{
		Time delta = Time::Delta();
		float speed = 10;
		float offset = (float)delta * speed;

		if (m_right)
		{
			camera.MoveWorld(Vec3f(offset, 0, 0));
		}
		if (m_front)
		{
			camera.MoveWorld(Vec3f(0, 0, offset));
		}
		if (m_left)
		{
			camera.MoveWorld(Vec3f(-offset, 0, 0));
		}
		if (m_back)
		{
			camera.MoveWorld(Vec3f(0, 0, -offset));
		}
	}
	void OnMoveFront(InputSignal* input)
	{
		m_front = !m_front;
	}
	void OnMoveBack(InputSignal* input)
	{
		m_back = !m_back;
	}
	void OnMoveLeft(InputSignal* input)
	{
		m_left = !m_left;
	}
	void OnMoveRight(InputSignal* input)
	{
		m_right = !m_right;
	}
private:
	bool m_left = false;
	bool m_front = false;
	bool m_back = false;
	bool m_right = false;

};

class RenderSys : public System
{
public:
	void OnResizeWindow(Vec2u size)
	{
		ForeachComponents<Camera>([size](Camera& camera) {
			camera.SetAspectRatio(size);
			});
	}
	void OnStart()
	{
		auto world = Systems::GetMainWorld();

		auto camera = Entity::Create();
		auto cam = camera.AddComponent<Camera>();
		cam->SetOrthographicZoom(0.25f);
		cam->SetOrthographic(true);
		cam->SetAspectRatio(Window::GetAspectRatio());

		Systems::SetMainCamera(cam);

		Window::GetResizeSignal()->AddListener(&RenderSys::OnResizeWindow, this);

		cam->Pitch(0);
		cam->Yaw(0);
		cam->SetPosition({ 0, 0, 20 });


		m_otherShader = makesptr<Shader>("assets/shaders/batch_renderer.vert", "assets/shaders/batch_renderer2.frag");
		m_texture1 = makesptr<Texture>("assets/textures/trollface.png");
		m_texture2 = makesptr<Texture>("assets/textures/image.png");


		m_maskedShader = makesptr<Shader>("assets/shaders/default_layer.vert", "assets/shaders/masked.frag");

		uint32_t layerid = Renderer2D::Instance()->AddLayer("MyLayer");
		uint32_t layeridmasked = Renderer2D::Instance()->AddLayer("maskedLayer", m_maskedShader, nullptr);
		uint32_t maskLayer = Renderer2D::Instance()->AddOffscreenLayer("mask", 1);

		m_maskPipeline = Renderer2D::Instance()->GetBatchId(maskLayer, nullptr, nullptr);
		m_pipeline = Renderer2D::Instance()->GetBatchId(layerid, nullptr, nullptr);
		m_pipelineMasked = Renderer2D::Instance()->GetBatchId(layeridmasked, nullptr, nullptr);

	}

	void OnRender()
	{

		std::vector<Vec2f> texCoords{ { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }
		};


		int count = 0;
		auto white = Vec4f(1, 1, 1, 1);

		for (float i = -20; i < 20; i += 1.0f)
		{
			for (float j = -20; j < 20; j += 1.0f)
			{
				float y = i / 1.0f;
				float x = j / 1.0f;
				Transform transform;
				transform.SetPosition(x, y, 0.0f);
				transform.SetScale(1.0f, 1.0f, 1.0f);

				if (count % 2)
				{
					Renderer2D::Instance()->DrawTexturedQuad(transform, m_texture1, texCoords, white, m_maskPipeline);
				}
				else
				{
					Renderer2D::Instance()->DrawTexturedQuad(transform, m_texture2, texCoords, white, m_pipeline);
				}
				count++;
			}
		}
		Transform transform;
		transform.SetScale(10, 10, 1);
		Renderer2D::Instance()->DrawTexturedQuad(transform, m_texture1, texCoords, white, m_pipelineMasked);

	}
private:
	sptr<Shader> m_otherShader;
	sptr<Texture> m_texture1;
	sptr<Texture> m_texture2;
	sptr<Shader> m_maskedShader;

	
	uint32_t m_pipeline;
	uint32_t m_pipelineMasked;
	uint32_t m_maskPipeline;

};

void RendererTest()
{
	Engine::Init();
	Systems::CreateWorld();
	Systems::Push<CameraMovementSystem>();
	Systems::Push<RenderSys>();
	Engine::Launch();

}