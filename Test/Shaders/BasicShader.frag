#version 330 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	vec4 color;
} fs_in;


void main() 
{
    color = fs_in.color;
}