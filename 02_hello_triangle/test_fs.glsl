#version 410

in vec3 colour;
in vec2 texcoord;

out vec4 frag_colour;

uniform sampler2D ourTexture;

void main() {
	//frag_colour = vec4 (colour, 1.0);
	frag_colour = texture(ourTexture, texcoord) * vec4 (colour, 1.0);
}
