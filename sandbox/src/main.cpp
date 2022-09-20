#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Render/Vertex.h"
#include "Render/VertexArray.h"
#include "Log.h"


using namespace sandbox;

int main(int argc, char* argv[])
{

	Log::Init();
	LOG_INFO("Logger initialiazed");

	//Create a window and an opengl context with SDL
	WindowGLContext window("hello window", Vec2i(500, 500));

	ShaderProgram shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");

	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer
	sptr<VertexArray> vertexArray = makesptr<VertexArray>();

	//Creating our vertices in normalized screen coordinates
	std::vector<Vertex> vert{
		{Vec2f(-0.5f, -0.5f), Color(100, 1, 1)},
		{Vec2f(0.5f, -0.5f), Color(1, 155, 1)},
		{Vec2f(0.5f, 0.5f), Color(1, 1, 1)},
		{Vec2f(-0.5f, 0.5f), Color(150, 1, 1)}
	};

	//VertexBuffer store all the data about the vertices and the attribute layout
	sptr<VertexBuffer> vertexBuffer = makesptr<VertexBuffer>(vert);

	//Element array data that will tell in wich order to draw the vertex, and can also be used to generate more vertices out of our Vertex Array
	//(in this case and draw a rectangle with triangles)
	uint32_t indices[]{
		0,1,2,
		2,3,0
	};

	//IndexBuffer
	sptr<IndexBuffer> elementBuffer = makesptr<IndexBuffer>(indices, sizeof(indices));

	vertexArray->AddVertexBuffer(vertexBuffer);
	vertexArray->SetIndexBuffer(elementBuffer);

	SDL_Event e;
	bool run = true;

	while (run)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				run = false;
			}
		}

		window.Clear();

		//Activate the shader program
		shader.Bind();

		vertexArray->Bind();

		//Function drawing primitives using the currently bound shader and VertexArray
		glDrawElements(GL_TRIANGLES, elementBuffer->GetCount(), GL_UNSIGNED_INT, 0);
		window.Render();
	}


	return 0;
}