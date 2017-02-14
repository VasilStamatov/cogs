#version 330 core

in VS_OUT
{
	vec3 uv;
} fs_in; 

out vec4 color;

uniform samplerCube skybox;

void main()
{
	color = texture(skybox, fs_in.uv);
}