// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;


#include <src/utils/shader.hpp>
#include <src/utils/controls.hpp>
#include <src/utils/texture_loader.hpp>


static GLfloat g_vertex_buffer_data[] = { 
	//x     y     z     r     g     b        tex_id         tex_cords   
	-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          0.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          1.0f, 0.0f, // bottom right
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          1.0f, 1.0f, // top right

	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          1.0f, 1.0f, // top right
	-1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          0.0f, 1.0f, // top left 
	-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f,    0.0f,          0.0f, 0.0f,  // bottom left



	 0.5f, -1.0f, 0.0f, 0.5f, 1.0f, 1.0f,    1.0f,          0.0f, 0.0f, // bottom left
	 1.0f, -1.0f, 0.0f, 0.5f, 1.0f, 1.0f,    1.0f,          1.0f, 0.0f, // bottom right
	 1.0f, -0.5f, 0.0f, 1.0f, 0.5f, 1.0f,    1.0f,          1.0f, 1.0f, // top right

	 1.0f, -0.5f, 0.0f, 1.0f, 0.5f, 1.0f,    1.0f,          1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f,    1.0f,          0.0f, 1.0f, // top left 
	 0.5f, -1.0f, 0.0f, 1.0f, 1.0f, 0.5f,    1.0f,          0.0f, 0.0f  // bottom left
};

class OGLTexture
{
public:
	OGLTexture(){
		glGenVertexArrays(1, &vertexArrayID);

		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glBindVertexArray(vertexArrayID);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			9 * sizeof(float),                  // stride
			(void*)0            // array buffer offset
		);


		// 2nd attribute buffer : vertices
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));

		// 3rd attribute buffer : vertices
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));

		// 4th attribute buffer : vertices
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	}

	GLuint InitShaders(const char * vertex_file_path,const char * fragment_file_path){
		// Create and compile our GLSL program from the shaders
		programID = LoadShaders(vertex_file_path, fragment_file_path);
		return programID;
	}


	GLuint LoadTexture(const char *imagePath, const char *samplerUniform){
		int idx = textureIDs.size();
		int width, height, channels;
		GLuint textureID = load_texture(imagePath, &width, &height, &channels, idx);
		
		// Get a handle for our "texture_sampler" uniform fragment shader
		glUseProgram(programID);
		glUniform1i(glGetUniformLocation(programID, samplerUniform), idx);

		return textureID;
	}

	void AddTexture(const char *imagePath, const char *samplerUniform){
		GLuint textureID = LoadTexture(imagePath, samplerUniform);
		textureIDs.push_back(textureID);
	}

	void Bind(){
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureIDs[1]);


		glBindVertexArray(vertexArrayID);

	}

	void Render(){
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3*2*2); // 6 indices 2 triangle
	}

	void UnBind(){
		
	}

	~OGLTexture(){
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Cleanup VBO
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteProgram(programID);
	}
	GLuint GetProgramID(){return programID;}
private:
	GLuint vertexArrayID;
	GLuint vertexBufferID;
	GLuint programID;
	std::vector<GLuint> textureIDs;
};





GLFWwindow* window;

int InitWindow(){

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
	window = glfwCreateWindow( 640, 480, "Keyboard Controls", NULL, NULL);
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

}





int main( void )
{
	
	// no gl stuff
	if(InitWindow() == -1){
		printf("Exiting\n");
	}

	// Dark gray background
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);


	OGLTexture ogl_texture;
	ogl_texture.InitShaders("res/vertex.shader", "res/fragment.shader");
	ogl_texture.AddTexture("res/back.jpg", "texture1");
	ogl_texture.AddTexture("res/mehul.jpg", "texture2");




	do{

		
		ogl_texture.Bind();

		// Use our shader
		glUseProgram(ogl_texture.GetProgramID());

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window, 640, 480);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(glGetUniformLocation(ogl_texture.GetProgramID(), "MVP"), 1, GL_FALSE, &MVP[0][0]);

		ogl_texture.Render();
		ogl_texture.UnBind();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}