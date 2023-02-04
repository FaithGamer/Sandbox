#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Render/Vertex.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Transform.h"
#include "Render/Camera.h"
#include "Input/ButtonInput.h"
#include "Input/InputMap.h"
#include "Log.h"
#include "TypeId.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/src/entt/entt.hpp>

#include "Timing.h"

#include <entt/entt.hpp>
#include "entt_macros.h"

#include "Tools/Toolbox.h"

#include "Signals.h"

using namespace Sandbox;


struct move
{
	PointableComponent
	Vec2f direction;

};
struct position
{
	Vec2f pos;
};
struct control
{
	char dummy;
};

entt::registry reg;

struct SignalD
{
	int pouet = 10;
};

struct SignalA
{
	float pouet = 10;
};
class SomeListener
{
public:

	void OnA(const SignalA& message)
	{
		std::cout << "received " << message.pouet << std::endl;
	}
	static void ReceiveA(const SignalA& message, void* const listener, const std::any& data)
	{
		static_cast<SomeListener*>(listener)->OnA(message);
	}

	void OnD(const SignalD& message, const std::any& data)
	{
		std::cout << "received " << message.pouet << " "<< std::any_cast<int>(data) << std::endl;
	}
	static void ReceiveD(const SignalD& message, void* const listener, const std::any& data)
	{
		static_cast<SomeListener*>(listener)->OnD(message, data);
	}
};

void OnPressButton(const ButtonInput::State& btnState, void* const listener, const std::any& data)
{
	LOG_INFO("the button has been pressed, state: " + std::to_string(btnState.pressed));
}

class SomeSystem : public SignalSender
{
public:
	void DCall()
	{
		SendSignal(SignalD(42));
	}
	void ACall()
	{
		SendSignal(SignalA(75.5));
	}
};


void teachEntt()
{


	SomeListener Li;
	SomeListener Lu;
	SomeSystem Bu;

	Bu.AddListener(&SomeListener::ReceiveD, &Li, SignalPriority::low, 61);
	Bu.AddListener(&SomeListener::ReceiveD, &Lu, SignalPriority::medium, 33);
	Bu.ACall();
	Bu.DCall();
	Bu.RemoveListenerFrom<SignalD>(&Li);
	Bu.ACall();
	Bu.DCall();
	//SignalSender::listen<SignalD>(&foo::signal_receiver, F);

	std::vector<entt::entity> entities;

	Clock c;

	std::vector<int> VV{1, 1,1,1,1,16, 5,1,1,1,1, 4, 1,1,1,1,1, 1, 7, 4, 1,1 , 1,1,1,1,1};
	int K = Toolbox::VectorRemove(1, VV);
	LOG_INFO(K);
	for (auto j : VV)
	{
		std::cout << j << std::endl;
	}

	for (int i = 0; i < 100000; i++)
	{
		entities.emplace_back(reg.create());
		if (i % 2)
		{
			reg.emplace<control>(entities.back());
		}
		if (i % 3)
		{
			reg.emplace<position>(entities.back());
		}
		reg.emplace<move>(entities.back());
	}

	LOG_INFO("Created entities in: " + std::to_string(c.GetElapsed()) + "s");

	int destroyed = 0;

	reg.view<move, control>().each([&destroyed](auto id, move& m, control& c) {
		reg.destroy(id);

		destroyed++;
		});



	Clock b;

	reg.view<move>().each([](auto entity, move& m)
		{
			m.direction.x = 5;
		});

	LOG_INFO("Acted upon move component in:" + std::to_string(b.GetElapsed()) + "s");



}

int main(int argc, char* argv[])
{

	
	Log::Init();
	LOG_INFO("Logger initialiazed");

	//Create a window and an opengl context with SDL
	WindowGLContext window("hello window", Vec2i(500, 500));

	sptr<ShaderProgram> shader = makesptr<ShaderProgram>("shaders/model_view_projection.vert", "shaders/texture.frag");
	sptr<ShaderProgram> shaderBillboard = makesptr<ShaderProgram>("shaders/billboardY.vert", "shaders/texture.frag");

	sptr<Texture> texture = makesptr<Texture>("image.png");
	teachEntt();
	Transform transform;
	transform.Rotate(90);
	transform.SetTranslation({ 0, 0, 0 });		
	transform.SetScale(Vec3f(0.5, 0.5, 0.5));

	Transform transform2;
	transform2.SetTranslation({ 0, 0, 0 });
	transform2.SetOrigin({ 0, -0.5, 0 });
	transform2.SetScale(Vec3f(0.25, 0.25, 0.25));

	/*Mat4 view = glm::translate(Mat4(1.f), glm::vec3(0, 0, -10));
	Mat4 perspective = glm::perspective(glm::radians(45.f), (float)500 / 500, 0.1f, 100.f);
	Mat4 orthographic = glm::ortho(0.f, 500.f, 0.f, 500.f, -1.f, 1.f);*/

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
	sptr<IndexBuffer> indexBuffer = makesptr<IndexBuffer>(indices, sizeof(indices));

	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer
	sptr<VertexArray> vertexArray = makesptr<VertexArray>();
	vertexArray->SetIndexBuffer(indexBuffer);
	vertexArray->AddVertexBuffer(vertexBuffer);

	InputMap inputs("InputsMap");

	auto button = makesptr<ButtonInput>("ButtonTest");

	button->BindKey(SDL_SCANCODE_W);

	inputs.AddInput(button);

	auto btn = inputs.GetInput("ButtonTest");

	btn->AddListener(&OnPressButton, nullptr);

	Vec3f o = Vec3f(0, 0, 0);

	LOG_INFO(o.x);

	SDL_Event e;
	bool run = true;

	//shader.SetUniform("offset", 0.2f);
	cam.Pitch(0);
	cam.Yaw(-90);
	cam.SetPosition({ 0, 0, 1 });
	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			inputs.Update(e);
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

		window.Clear();

		//Activate the shader program
		vertexArray->Bind();
		shader->Bind();
		texture->Bind();


		float x = std::sin((float)SDL_GetTicks() / 1000);
		float z = std::cos((float)SDL_GetTicks() / 1000);

		shaderBillboard->Bind();
		shaderBillboard->SetUniform("model", transform2.GetTransformMatrix());
		shaderBillboard->SetUniform("view", cam.GetViewMatrix());
		shaderBillboard->SetUniform("projection", cam.GetProjectionMatrix());

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		//cam.SetPosition({ x, 1, z });
		shader->SetUniform("view", cam.GetViewMatrix());
		shader->SetUniform("projection", cam.GetProjectionMatrix());

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		
		

		window.Render();


	}
	return 0;
}