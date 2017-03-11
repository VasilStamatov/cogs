#version 330 core

layout (location = 0) in vec3 position_modelSpace;
layout (location = 1) in vec4 position_worldSpace_size;
layout (location = 2) in vec4 color;

out VS_OUT
{
	vec4 color;
	vec2 uv;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 cameraRight_worldSpace;
uniform vec3 cameraUp_worldSpace;
// uniform mat4 model;

void main() 
{
	float particleSize = position_worldSpace_size.w;
	vec3 particleCenter_worldSpace = position_worldSpace_size.xyz;
	
	vec3 vertexPosition_worldspace = particleCenter_worldSpace + cameraRight_worldSpace * position_modelSpace.x * particleSize + cameraUp_worldSpace * position_modelSpace.y * particleSize;
	
	gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);
	
	vs_out.uv = position_modelSpace.xy + vec2(0.5f, 0.5f);
	vs_out.color = color;
}