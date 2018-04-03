#version 330 core

in vec2 textCoord;
out vec4 frag_color;

uniform sampler2D myTexture;
uniform vec3 lightColor;

void main()
{	
	frag_color = vec4(lightColor, 1.0f);
}
