#version 330 core

in vec2 textCoord;
out vec4 frag_color;

uniform sampler2D myTexture;
uniform sampler2D myTexture2;

void main()
{
	frag_color = mix(texture(myTexture, textCoord), texture(myTexture2, textCoord), 0.2);
}
