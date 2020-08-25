#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float aTexId;
layout (location = 3) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out float TexID;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{
	if(aTexId>0) gl_Position = MVP*vec4(aPos, 1.0);
	else gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
	TexID = aTexId;
}