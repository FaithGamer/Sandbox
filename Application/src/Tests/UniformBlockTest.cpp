#include "pch.h"
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
#include "Sandbox/Log.h"
#include "Sandbox/TypeId.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <entt/entt.hpp>
#include "Sandbox/Engine.h"
#include "Sandbox/Time.h"
using namespace Sandbox;

void UniformBlockTest()
{
    Engine::Init();
    std::shared_ptr<Shader> shaderRed = makesptr<Shader>("assets/shaders/ubt.vert", "assets/shaders/ubt.frag");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[]
    {
        -0.5f, -0.5f, 0, 
         0.5f, -0.5f, 0,  
         0.5f,  0.5f, 0,  
         -0.5f,  0.5f, 0
    };
    //VAO
    AttributeLayout l{ {ShaderDataType::Vec3f, "aPos"} };

    sptr<VertexBuffer> vbo = makesptr<VertexBuffer>(sizeof(vertices), l);
    vbo->SetData(vertices, sizeof(vertices));
    unsigned int indices[]
    {
        0,1,2,
        2,3,0
    };

    sptr<IndexBuffer> ibo = makesptr<IndexBuffer>(indices, 6);
    
    VertexArray vao(vbo, ibo);

    {
        uint32_t layerIndices[]
        {
            0, 1, 2,
            2, 3, 0
        };


        sptr<IndexBuffer> layerIndexBuffer = makesptr<IndexBuffer>(layerIndices, 6, GL_STATIC_DRAW);
    }
    // configure a uniform buffer object
    // ---------------------------------
    // first. We get the relevant block indices
    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shaderRed->GetID(), "Matrices");
    // then we link each shader's uniform block to this uniform binding point
    glUniformBlockBinding(shaderRed->GetID(), uniformBlockIndexRed, 0);

    // Now actually create the buffer
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    Camera cam;
    cam.Pitch(0);
    //cam.Yaw();
    cam.SetPosition({ 0, 0, 1 });
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cam.GetProjectionMatrix()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // render loop
    // -----------
    bool run = true;


    SDL_Event e;
    while (run)
    {
    
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                run = false;
            }

        }
        // render
        // ------
        Window::ClearWindow();

        // set the view and projection matrix in the uniform block - we only have to do this once per draw call
        glm::mat4 view = cam.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        // draw 4 cubes 
        // RED
        vao.Bind();
        shaderRed->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
        shaderRed->SetUniform("model", model);
        glDrawElements(GL_TRIANGLES, vao.GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);

        Window::RenderWindow();
    }
}