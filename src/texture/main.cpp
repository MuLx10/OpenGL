// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


#include <src/utils/shader.hpp>
#include <src/utils/texture_loader.hpp>

int main( void )
{
	GLFWwindow* window;
	
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 640, 480, "OGL Texture", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark gray background
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

	static const GLfloat g_vertex_buffer_data[] = { 
		//x     y     z    
		-0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 1.0f,

		 0.5f,  0.5f, 0.0f, 1.0f, 0.5f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.5f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f
	};

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


	glBindVertexArray(VertexArrayID);
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "src/texture/res/vertex.shader", "src/texture/res/fragment.shader" );

	int width, height, channels;
	GLuint texture = load_texture("src/texture/res/mehul.jpg", &width, &height, &channels);
	
	// Get a handle for our "texture_sampler" uniform
	GLuint textureID  = glGetUniformLocation(programID, "texture_sampler");

	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		glBindVertexArray(VertexArrayID);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			6 * sizeof(float),                  // stride
			(void*)0            // array buffer offset
		);


		// 2nd attribute buffer : vertices
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			6 * sizeof(float),                  // stride
			(void*)(3 * sizeof(float))            // array buffer offset
		);


		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3*2); // 6 indices 2 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}