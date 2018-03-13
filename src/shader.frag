#version 330 core

in vec2 textCoord;
out vec4 frag_color;

uniform sampler2D myTexture;

void main()
{	
	frag_color = texture(myTexture, textCoord);
}
