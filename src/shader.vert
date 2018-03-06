#version 330 core

layout (location = 0) in vec3 pos;

uniform vec2 positionOffset;

void main()
{
	gl_Position = vec4(pos.x + positionOffset.x, pos.y + positionOffset.y, pos.z, 1);	
}