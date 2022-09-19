#include "pch.h"
#include "Render/WindowGLContext.h"
#include "Render/ShaderProgram.h"
#include "Render/Buffer.h"
#include "Log.h"


using namespace sandbox;

int main(int argc, char* argv[])
{
	
	Log::Init();
	LOG_INFO("Logger initialiazed");

	WindowGLContext window("hello window", Vec2i(500, 500));


	SDL_Event e;
	bool run = true;

	ShaderProgram shader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");


	//Defining 4 vertices in normalized screen coordinates


	//We need a Vertex Array Objet wich stores all the vertex data
	//and attribute configuration we gonna set. It makes switch between states easier.
	//VertexArray will store the state of the Vertex Array Buffer and Element Array Buffer

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	float vertices[]{
	 -0.5f, -0.5f, 0.0f, //bottom left
	 0.5f, -0.5f, 0.0f, //bottom right
	 0.5f, 0.5f, 0.0f, //top right
	 -0.5f, 0.5f, 0.0f }; //top left



	unsigned int s = sizeof(vertices);

	VertexBuffer vertexBuffer(vertices, sizeof(vertices));


	//Element array data that will tell in wich order to draw the vertex, and can also be used to generate more vertices out of our Vertex Array
	//(in this case and draw a rectangle with triangles)
	unsigned int indices[]{
		0,1,2,
		2,3,0
	};
    ElementBuffer elementBuffer(indices, sizeof(indices));


	//We need to tell OpenGL how to interpret the data in the Vertex Array Buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//Each vertex attribute takes its data from memory managed by a VBO and which
	//VBO it takes its data from (you can have multiple VBOs) is determined
	//by the VBO currently bound to GL_ARRAY_BUFFER when calling glVertexAttribPointer.
	//Since the previously defined VBO is still bound before calling glVertexAttribPointer
	//vertex attribute 0 is now associated with its vertex data.
	//The next function will enable the vertex attribute, cause they are disabled by default.
	glEnableVertexAttribArray(0);

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
		//Tell openGL to activate the shader program
		//Every shader and rendering call after glUseProgram will use this program object
		shader.Bind();

		glBindVertexArray(VAO);
		//elementBuffer.Bind();
		//Function drawing primitives using the currently active shader
		//Primitive type, starting index of the vertex array, number of vertices.
		glDrawElements(GL_TRIANGLES, vertexBuffer.GetCount(), GL_UNSIGNED_INT, 0);
		window.Render();
	}


	return 0;
}