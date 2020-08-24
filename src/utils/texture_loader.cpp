#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


GLuint load_texture(const char *imagepath, int* width, int* height, int* channels, int idx = 0){
	stbi_set_flip_vertically_on_load(true); 
	unsigned char *texture_data = stbi_load(imagepath, width, height, channels, 3);
	
	if(*width < 0 || *height < 0){
		printf("Unable to load %s\n", imagepath);
		return 0;
	}
	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, *(unsigned int*)width, *(unsigned int*)height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

	// OpenGL has now copied the texture_data. Free our own version
	stbi_image_free(texture_data);

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}