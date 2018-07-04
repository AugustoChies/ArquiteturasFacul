#version 410

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texc; 

out vec2 TexCoords;

void main()
{
	TexCoords = texc;
    gl_Position = vec4(aPos, 1.0);
} 