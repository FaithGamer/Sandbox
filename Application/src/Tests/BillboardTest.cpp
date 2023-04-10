#pragma once
#include "pch.h"

#include "BillboardTest.h"

#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/ShaderProgram.h"
#include "Sandbox/Render/Buffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Transform.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/BatchRenderer.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Log.h"
#include "Sandbox/TypeId.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>

#include "Sandbox/Time.h"
#include "Sandbox/entt_macros.h"
#include "Sandbox/Vector.h"
#include "Sandbox/Signal.h"


using namespace Sandbox;

void OnPressButton(const ButtonInput::State& btnState, void* const listener)
{
	LOG_INFO("the button has been pressed, state: " + std::to_string(btnState.pressed));
}

void BillboardTest()
{

	sptr<ShaderProgram> shader = makesptr<ShaderProgram>("assets/shaders/model_view_projection.vert", "assets/shaders/texture.frag");
	sptr<ShaderProgram> shaderBillboard = makesptr<ShaderProgram>("assets/shaders/billboard.vert", "assets/shaders/texture.frag");

	sptr<Texture> texture = makesptr<Texture>("assets/textures/image.png");

	Transform transform;
	transform.RotateZAxis(90);
	transform.SetPosition({ 0, 0, 0 });
	transform.SetScale(Vec3f(0.5, 0.5, 0.5));

	Transform transform2;
	transform2.SetPosition({ 0, 0, 0 });
	transform2.SetOrigin({ 0, -0.5, 0 });
	transform2.SetScale(Vec3f(0.25, 0.25, 0.25));


	Camera cam;

	cam.SetPosition(Vec3f(1.12, -1.4, 1.3));

	shader->SetUniform("model", transform.GetTransformMatrix());
	shader->SetUniform("view", cam.GetViewMatrix());
	shader->SetUniform("projection", cam.GetProjectionMatrix());

	float vertices[]
	{
		-0.5f, -0.5f, 0,  0.0f, 1.0f,
		 0.5f, -0.5f, 0,  1.0f, 1.0f,
		 0.5f,  0.5f, 0,  1.0f, 0.0f,
		 -0.5f,  0.5f, 0,  0.0f, 0.0f,
	};

	AttributeLayout layout{
		{ ShaderDataType::Vec3f, "aPos" },
		{ ShaderDataType::Vec2f, "aTexCoords" }
	};

	//VertexBuffer store all the data about the vertices and the attribute layout
	sptr<VertexBuffer> vertexBuffer = makesptr<VertexBuffer>(vertices, sizeof(vertices), layout);

	//Element array data that will tell in wich order to draw the vertex, and can also be used to generate more vertices out of our Vertex Array
	//(in this case and draw a rectangle with triangles)
	uint32_t indices[]{
		0,1,2,
		2,3,0,
	};

	//IndexBuffer
	sptr<IndexBuffer> indexBuffer = makesptr<IndexBuffer>(indices, 6);

	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer
	sptr<VertexArray> vertexArray = makesptr<VertexArray>();
	vertexArray->SetIndexBuffer(indexBuffer);
	vertexArray->AddVertexBuffer(vertexBuffer);

	//Inputs
//InputMap inputs("InputsMap");

	//auto button = makesptr<ButtonInput>("ButtonTest");

	//button->BindKey(KeyScancode::A);

	//inputs.AddInput(button);

	//auto btn = inputs.GetInput("ButtonTest");

	//btn->AddListener(&OnPressButton, nullptr);

	Vec3f o = Vec3f(0, 0, 0);

	LOG_INFO(o.x);

	SDL_Event e;
	bool run = true;

	//shader.SetUniform("offset", 0.2f);

	cam.SetPosition({ 0, 0, 1 });

	BatchRenderer renderer;
	std::vector<Vec2f> texCoords{ { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
	};


	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			//inputs.OnEvent(e);
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				float offset = 0.1f;
				switch (e.key.keysym.sym)
				{
				case SDLK_z:
					cam.MoveLocalZ(offset);
					break;
				case SDLK_q:
					cam.MoveLocalX(-offset);
					break;
				case SDLK_s:
					cam.MoveLocalZ(-offset);
					break;
				case SDLK_d:
					cam.MoveLocalX(offset);
					break;
				case SDLK_UP:
					cam.Pitch(-offset * 50);
					break;
				case SDLK_LEFT:
					cam.Yaw(-offset * 50);
					break;
				case SDLK_DOWN:
					cam.Pitch(offset * 50);
					break;
				case SDLK_RIGHT:
					cam.Yaw(offset * 50);
					break;
				}
			}
			if (e.type == SDL_TEXTINPUT)
			{

			}
		}


		Window::Clear();

		//Activate the shader program
		vertexArray->Bind();
		shader->Bind();
		texture->Bind();

		shader->SetUniform("view", cam.GetViewMatrix());
		shader->SetUniform("projection", cam.GetProjectionMatrix());

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		shaderBillboard->Bind();
		shaderBillboard->SetUniform("model", transform2.GetTransformMatrix());
		shaderBillboard->SetUniform("view", cam.GetViewMatrix());
		shaderBillboard->SetUniform("projection", cam.GetProjectionMatrix());

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);


		Window::Render();
	}
}