#version 330 core

layout (location = 0) in vec3 position_modelSpace;
layout (location = 1) in vec4 position_worldSpace_size;
layout (location = 2) in vec4 color;
layout (location = 3) in vec4 texOffsets;
layout (location = 4) in float blendFactor;

out VS_OUT
{
	vec4 color;
	vec2 uv1;
	vec2 uv2;
	float blend;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

uniform vec3 cameraRight_worldSpace;
uniform vec3 cameraUp_worldSpace;

uniform float texNumOfRows;
// uniform mat4 model;

void main() 
{
	float particleSize = position_worldSpace_size.w;
	vec3 particleCenter_worldSpace = position_worldSpace_size.xyz;
	
	vec3 vertexPosition_worldspace = particleCenter_worldSpace + cameraRight_worldSpace * position_modelSpace.x * particleSize + cameraUp_worldSpace * position_modelSpace.y * particleSize;
	
	gl_Position = projection * view * vec4(vertexPosition_worldspace, 1.0f);
	
	vec2 texCoords = position_modelSpace.xy + vec2(0.5f, 0.5f);
	texCoords /= texNumOfRows;
	
	vs_out.uv1 = texCoords + texOffsets.xy;
	vs_out.uv2 = texCoords + texOffsets.zw;
	vs_out.color = color;
	vs_out.blend = blendFactor;
}