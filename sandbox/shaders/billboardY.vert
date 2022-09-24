#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//2D sprite in a 3D world always facing the camera
void main()
{
    vTexCoords = aTexCoords;

    //Negate the component of the view matrix responsible for the rotation
    //So the rotation of the mesh is only affected my the model matrix
    mat4 billboardView = mat4(1.0);
    for(int i = 0; i < 3; i ++)
    {
        //We keep only the 3rd column containing the camera's position
        billboardView[3][i] = view[3][i];
    }

    //We keep the component responsible for the rotation of the mesh along the X axis
    //So even if it keep facing the camera, the mesh deforms accordingly if look from above or under.
    //For some reason we loose the perspective tho...
    

    gl_Position = projection * billboardView * model * vec4(aPos, 1.0);
}
