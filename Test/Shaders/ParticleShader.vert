#version 330 core

layout (location = 0) in vec4 position;
// layout (location = 1) in vec2 uv;
layout (location = 1) in vec4 color;

out VS_OUT
{
	vec4 color;
	float width;
} vs_out;

// uniform mat4 projection;
// uniform mat4 view;
// uniform mat4 model;

void main() 
{
	// gl_Position = projection * view * vec4(position, 1.0f);
    gl_Position = vec4(position.xyz, 1.0f);
    vs_out.width = position.w;
    vs_out.color = color;
	
	//make the model matrix rotation and scale component be the transpose of the view matrix's component
	//this makes it so that the ModelView result matrix has 0 rotation so it always faces the camera
	// model[0][0] = view[0][0];
	// model[0][1] = view[1][0];
	// model[0][2] = view[2][0];
	// model[1][0] = view[0][1];
	// model[1][1] = view[1][1];
	// model[1][2] = view[2][1];
	// model[2][0] = view[0][2];
	// model[2][1] = view[1][2];
	// model[2][2] = view[2][2];

}