#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;


out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view = mat4(1.0f);
uniform mat4 model = mat4(1.0f);

void main() 
{
    //transform the world space coordinates from the spritebatch to clip coordinates with the ortho projection matrix
    gl_Position = projection * view * model * vec4(position, 1.0);
    
	//pass the world space coordinates
    vs_out.position = vec3(model * vec4(position, 1.0));;
    vs_out.uv = vec2(uv.x, uv.y);
    vs_out.color = color;
    
}