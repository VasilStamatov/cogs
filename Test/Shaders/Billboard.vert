#version 330 core

layout (location = 0) in vec3 position;

void main() 
{
    //set the gl position to the position passed in world space (hopefully)
    gl_Position = vec4(position, 1.0f);   
}