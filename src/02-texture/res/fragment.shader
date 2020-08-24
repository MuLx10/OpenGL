#version 330 core

in vec3 f_color;
in vec2 f_coords;

// Ouput data
out vec4 color;

uniform sampler2D texture_sampler;

void main()
{
	color = texture(texture_sampler, f_coords);
}