#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 size;
layout (location = 3) in mat4 toWorldMat;

out VS_OUT
{
	vec2 uv;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() 
{
	vec3 scaledPosition = vec3(position.x * size.x, position.y * size.y, position.z);
    //transform the world space coordinates from the spritebatch to clip coordinates with the ortho projection matrix
    gl_Position = projection * view * toWorldMat * vec4(scaledPosition, 1.0f);
    
    vs_out.uv = position.xy + vec2(0.5f, 0.5f);
    vs_out.color = color;
    
}