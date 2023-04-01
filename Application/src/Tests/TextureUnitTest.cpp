#include "pch.h"
#include "TextureUnitTest.h"
#include "Include/Render.h"

using namespace Sandbox;

void TextureUnitTest()
{
	Window window("window", { 500, 500 });

	SDL_Event e;
	bool run = true;

	Camera cam;
	cam.Pitch(0);
	cam.SetPosition({ -1.5, 0, 1 });
	cam.Yaw(50);

	Transform trans1;
	Transform trans2;

	trans1.SetPosition({ -0.55f, 0, 0
		});
	trans2.SetPosition({ 0.55f, 0, 0
		});


	//Shader
	ShaderProgram shader("assets/shaders/texture_unit.vert", "assets/shaders/texture_unit.frag");

	//Bind texture unit to appropriate uniform sampled2D
	shader.SetUniform("texture1", 0);
	shader.SetUniform("texture2", 1);

	//Texture
	sptr<Texture> tex1 = makesptr<Texture>();
	sptr<Texture> tex2 = makesptr<Texture>("assets/textures/trollface.png");


	float vertices[]
	{
		-0.5f, -0.5f, 0,  0.0f, 1.0f, 1,
		 0.5f, -0.5f, 0,  1.0f, 1.0f, 1,
		 0.5f,  0.5f, 0,  1.0f, 0.0f, 1,
		 -0.5f,  0.5f, 0,  0.0f, 0.0f, 1
	};

	AttributeLayout layout{
		{ ShaderDataType::Vec3f, "aPos" },
		{ ShaderDataType::Vec2f, "aTexCoords" },
		{ ShaderDataType::Float, "aTexIndex" }
	};

	sptr<VertexBuffer> vbo = makesptr<VertexBuffer>(sizeof(vertices), layout);
	vbo->SetData(vertices, sizeof(vertices));
	unsigned int indices[]
	{
		0,1,2,
		2,3,0
	};

	//Index Buffer
	sptr<IndexBuffer> ibo = makesptr<IndexBuffer>(indices, 6);

	//Vertex Array
	VertexArray vao(vbo, ibo);

	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				run = false;
				break;
			}
		}

		window.Clear();

		//Bind texture to appropriate unit
		vao.Bind();
		//tex1->Bind(0);
		tex2->Bind(1);
		shader.Bind();


		shader.SetUniform("projectionView", cam.GetProjectionMatrix() * cam.GetViewMatrix());
		shader.SetUniform("model", trans2.GetTransformMatrix());
		
		glDrawElements(GL_TRIANGLES, vao.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		window.Render();
	}
}
