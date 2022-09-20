#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Render/Vertex.h"
#include "Log.h"


using namespace sandbox;
VertexBuffer createBuffer()
{
	float vertices[]{
		 -0.5f, -0.5f, 0.0f, 0, 1, 0, 1, //bottom left
		 0.5f, -0.5f, 0.0f, 1, 0.1, 0.4, 1, //bottom right
		 0.5f, 0.5f, 0.0f, 1, 1, 0.1, 1,//top right
		 -0.5f, 0.5f, 0.0f, 0, 1, 1, 1, }; //top left
	unsigned int s = sizeof(vertices);


	return VertexBuffer(vertices, s);

}
int main(int argc, char* argv[])
{

	Log::Init();
	LOG_INFO("Logger initialiazed");

	WindowGLContext window("hello window", Vec2i(500, 500));


	SDL_Event e;
	bool run = true;

	ShaderProgram shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");

	//We need a Vertex Array Objet wich stores all the vertex data
	//and attribute configuration we gonna set. It makes switch between states easier.
	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//VertexBuffer vertexBuffer;
	//Creating our vertices in normalized screen coordinates

	std::vector<Vertex> vert{
		{Vec2f(-0.5f, -0.5f), Color(100, 1, 1)},
		{Vec2f(0.5f, -0.5f), Color(1, 155, 1)},
		{Vec2f(0.5f, 0.5f), Color(1, 1, 1)},
		{Vec2f(-0.5f, 0.5f), Color(150, 1, 1)}
	};

	//VertexBuffer vertexBuffer
	VertexBuffer vertexBuffer(vert);

	//Element array data that will tell in wich order to draw the vertex, and can also be used to generate more vertices out of our Vertex Array
	//(in this case and draw a rectangle with triangles)
	unsigned int indices[]{
		0,1,2,
		2,3,0
	};



	IndexBuffer elementBuffer(indices, sizeof(indices));

	//How the buffer data is laid out, this will help conveniently call glVertexAttribPointer how we need it.
	{
		AttributeLayout layout{
			{ShaderDataType::Vec3f, "aPos"},
			{ShaderDataType::Vec4f, "aColor"}
		};


		vertexBuffer.SetLayout(layout);
	}


	//We need to tell OpenGL how to interpret the data in the Vertex Array Buffer
	int layoutIndex = 0;
	const auto& layout = vertexBuffer.GetLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(layoutIndex);
		glVertexAttribPointer(layoutIndex,
			ShaderDataTypeCount(element.type),
			ShaderDataTypeGLType(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(void*)element.offset);

		layoutIndex++;
	}


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

		glBindVertexArray(VAO);

		//Function drawing primitives using the currently bound shader and VertexArray
		glDrawElements(GL_TRIANGLES, elementBuffer.GetCount(), GL_UNSIGNED_INT, 0);
		window.Render();
	}


	return 0;
}