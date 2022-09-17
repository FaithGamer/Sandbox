#include "pch.h"
#include "WindowGLContext.h"
#include "Log.h"

std::string ifstreamToString(std::ifstream& file)
{
    std::ostringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

std::string loadShaderSourceFromFile(std::string path)
{
    std::ifstream shaderFile;
    shaderFile.open(path);
    return ifstreamToString(shaderFile);
}

void checkShaderCompiled(unsigned int shader)
{
    int success;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Shader compilation failed " + std::string(infoLog));
    }
}

void checkShaderProgramLinked(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOG_ERROR("Shader program linkage failed " + std::string(infoLog));

    }
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main(int argc, char* argv[])
{
	Log::Init();
	LOG_INFO("Logger initialiazed");

	WindowGLContext window("hello window", Vec2i(500, 500));


	SDL_Event e;
	bool run = true;


   // std::string  vertexShaderStr = loadShaderSourceFromFile("shaders/vertex_shader.vert");
   // auto vertexShaderSource = vertexShaderStr.c_str();

    //Create the shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Attach the object to the source code
    //The second argument specifies how many strings we're passing as source code [..] "and we can leave the 4th parameter to NULL."
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //Compile the shader
    glCompileShader(vertexShader);

    //Check for errors during the shader compilation
    checkShaderCompiled(vertexShader);

    //Same for the fragment shader:
    //std::string fragShaderStr = loadShaderSourceFromFile("shaders/fargment_shader.frag");
   // const char *fragmentShaderSource = fragShaderStr.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompiled(fragmentShader);

    //Creating the shader program that is the linked version of multiple shaders
    //(here the vertex shader and the fragment shader linked to the shader program)

    //When linking the shaders into a programe it links the outputs of each shader
    //to the inputs of the next shader. This is alos where well get linking errors if
    //the outputs and inputs doesn't match.

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //Check for erros when linking the shader program
    checkShaderProgramLinked(shaderProgram);

    //At this point we no longer need the shaders objects since they are linked
    //Into the shader program. We delete them this way

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Defining three vertices in normalized screen coordinates
    float vertices[]{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f };

    //We need a Vertex Array Objet wich stores all the vertex data
    //and attribute configuration we gonna set. It makes switch between states easier.

    // A vertex array object stores the following:

    //Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
    //Vertex attribute configurations via glVertexAttribPointer.
    //Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Creating a VBO: OpenGL object used to handle the memory of multiple vertex into the GPU memory.
    //Like any OpenGL object, it's represented by an ID of type unsigned int.
    unsigned int VBO;
    //Generate the ID
    glGenBuffers(1, &VBO);
    //Bind the newly created buffer to the GL_ARRAY_BUFFER target,
    //to create a vertex buffer object (VBO) we can bind differents buffers to several target
    // as long as we use differents target.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //From that point on any buffer calls we make on the GL_ARRAY_BUFFER target
    //will be used to configure the currently bound buffer, which is VertexBufferObject.

    //glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer
    //We send our data vertices (specifying it's size in bytes), to our VBO (cause it's bound to the target specified as first argument)
    //The fourth parameter specifies how we want the graphics card to manage the given data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    //We need to tell OpenGL how the vertex shader will interpret the data we send him.
    //The following parameters works for the the vertex attribute at location 0.
    //In this example, the data consist on 3 floats, tightly packed together,
    //representing the vertex position, and starting at the very beginning of the data buffer.
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
        glUseProgram(shaderProgram);
        //bind the VAO (why?)
        glBindVertexArray(VAO);
        //Function drawing primitives using the currently active shader
        //Primitive type, starting index of the vertex array, number of vertices.
        glDrawArrays(GL_TRIANGLES, 0, 3);
		window.Render();
	}

	
	return 0;
}