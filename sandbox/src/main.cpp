#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Render/Vertex.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Transform.h"
#include "Render/Camera.h"
#include "Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace sandbox;

int main(int argc, char* argv[])
{
	Log::Init();
	LOG_INFO("Logger initialiazed");

	//Create a window and an opengl context with SDL
	WindowGLContext window("hello window", Vec2i(500, 500));

	sptr<ShaderProgram> shader = makesptr<ShaderProgram>("shaders/model_view_projection.vert", "shaders/texture.frag");

	sptr<Texture> texture = makesptr<Texture>("image.png");

	Transform transform;
	transform.SetTranslation({0, 0, 0});
	//transform.SetOrigin({ 0, 0, 0 });
	transform.SetScale(Vec3f(0.5, 0.5, 0.5));
	

	/*Mat4 view = glm::translate(Mat4(1.f), glm::vec3(0, 0, -10));
	Mat4 perspective = glm::perspective(glm::radians(45.f), (float)500 / 500, 0.1f, 100.f);
	Mat4 orthographic = glm::ortho(0.f, 500.f, 0.f, 500.f, -1.f, 1.f);*/

	Camera cam;

	cam.SetPosition(Vec3f( 1, 0, 1 ));

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

	window.SetSize({ 900, 300 });

	//IndexBuffer
	sptr<IndexBuffer> indexBuffer = makesptr<IndexBuffer>(indices, sizeof(indices));

	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer
	sptr<VertexArray> vertexArray = makesptr<VertexArray>();
	vertexArray->SetIndexBuffer(indexBuffer);
	vertexArray->AddVertexBuffer(vertexBuffer);


	SDL_Event e;
	bool run = true;

	//shader.SetUniform("offset", 0.2f);
	
	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
		}

		float x = std::sin((float)SDL_GetTicks()/1000);
		float z = std::cos((float)SDL_GetTicks()/1000);

		cam.SetPosition(Vec3f(x, 0, 1));
		shader->SetUniform("view", cam.GetViewMatrix());
		shader->SetUniform("projection", cam.GetProjectionMatrix());

		window.Clear();

		//Activate the shader program
		vertexArray->Bind();
		shader->Bind();
		texture->Bind();

		//Function drawing primitives using the currently bound shader and VertexArray
		
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

		window.Render();
	}


	return 0;
}