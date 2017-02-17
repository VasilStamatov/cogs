#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;


out VS_OUT
{
	vec3 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() 
{
    //transform the world space coordinates from the spritebatch to clip coordinates with the ortho projection matrix
    gl_Position = projection * view * vec4(position, 1.0);
    
    vs_out.color = color;
}