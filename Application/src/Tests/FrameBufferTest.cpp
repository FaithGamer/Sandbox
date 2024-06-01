#include "UniformBlockTest.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Shader.h"
#include "Sandbox/Render/Buffer.h"
#include "Sandbox/Render/VertexArray.h"
#include "Sandbox/Render/Texture.h"
#include "Sandbox/Render/Transform.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Core/Log.h"
#include "Sandbox/Core/TypeId.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sandbox/Entt.h"
#include "Sandbox/Engine.h"
#include "Sandbox/Core/Time.h"
#include "Sandbox/ECS/System.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/RenderTexture.h"

#include "Sandbox/Core/std_macros.h"

#include "FrameBufferTest.h"

using namespace Sandbox;

class RenderFramebuffer : public System
{
public:
	void OnStart()
	{
		float vertices[]
		{
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
		};

		float verticesLayer[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
		};

		float verticesLayerI[]
		{
		-1.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,
		};

		AttributeLayout layout1({
				{ ShaderDataType::Vec3f, "aPosition" },
				{ ShaderDataType::Vec2f, "aTexCoords" } });


		sptr<VertexBuffer> vb = makesptr<VertexBuffer>(vertices, sizeof(vertices), layout1);
		sptr<VertexBuffer> vbl = makesptr<VertexBuffer>(verticesLayer, sizeof(verticesLayer), layout1);

		m_shader = makesptr<Shader>("assets/shaders/model_view_projection.vert", "assets/shaders/texture.frag");
		m_shaderLayer = makesptr<Shader>("assets/shaders/default_layer.vert", "assets/shaders/texture.frag");

		m_shader->SetUniform("tex", 0);
		m_shaderLayer->SetUniform("tex", 0);

		m_texture = makesptr<Texture>("assets/textures/trollface.png");

		m_render = makesptr<RenderTexture>(Window::GetSize());

		uint32_t indices[]
		{
			0, 1, 2,
			2, 3, 0
		};

		sptr<IndexBuffer> ib = makesptr<IndexBuffer>(indices, 6, GL_STATIC_DRAW);

		m_vao = makesptr<VertexArray>(vb, ib);
		m_vaoLayer = makesptr<VertexArray>();
		m_vaoLayer->AddVertexBuffer(vbl);

	}

	void OnUpdate(Time delta) override
	{
	
		Transform transform;

		Camera camera;
		camera.Pitch(0);
		camera.Yaw(0);
		camera.SetPosition({ 0, 0, 5 });

		glEnable(GL_DEPTH_TEST);

		m_render->Clear();
		m_render->Bind();
		m_shader->Bind();
		m_shader->SetUniform("model", transform.GetTransformMatrix());
		m_shader->SetUniform("view", camera.GetViewMatrix());
		m_shader->SetUniform("projection", camera.GetProjectionMatrix());
		m_texture->Bind();
		m_vao->Bind();

		glDrawElements(GL_TRIANGLES, m_vao->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_vaoLayer->Bind();
		m_render->BindTexture(0);
		m_shaderLayer->Bind();

		glDisable(GL_DEPTH_TEST);
		//glDrawElements(GL_TRIANGLES, m_vaoLayer->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

private:
	sptr<VertexArray> m_vao;
	sptr<VertexArray> m_vaoLayer;
	sptr<Texture> m_texture;
	sptr<RenderTexture> m_render;
	sptr<Shader> m_shader;
	sptr<Shader> m_shaderLayer;
};

void FrameBufferTest()
{

	Engine::Init();


	Systems::Push<RenderFramebuffer>();

	Engine::Launch();



}