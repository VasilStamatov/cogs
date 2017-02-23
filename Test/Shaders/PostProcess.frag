#version 330 core

in VS_OUT
{
	vec2 uv;
} fs_in; 

out vec4 color;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300;  

void main()
{
	 // color = texture(screenTexture, fs_in.uv);
	
	// color = vec4(vec3(1.0 - texture(screenTexture, fs_in.uv)), 1.0);
	
	// color = texture(screenTexture, fs_in.uv);
    // float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    // color = vec4(average, average, average, 1.0);
	
	 vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );
	
	//blur kernel
    // float kernel[9] = float[](
	// 1.0 / 16, 2.0 / 16, 1.0 / 16,
    // 2.0 / 16, 4.0 / 16, 2.0 / 16,
    // 1.0 / 16, 2.0 / 16, 1.0 / 16  
    // );
    
	//identity kernel
	float kernel[9] = float[](
		0, 0, 0,
        0, 1, 0,
        0, 0, 0 
    );
	
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, fs_in.uv.st + offsets[i]));
    }
    vec3 col = vec3(0.0f);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    color = vec4(col, 1.0f);
}