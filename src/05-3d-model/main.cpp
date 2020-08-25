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
#include <src/utils/obj_loader.hpp>
#include <src/utils/3Dcontrols.hpp>


class Model3D
{
public:
	Model3D(){
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);
	}

	GLuint InitShaders(const char * vertex_file_path,const char * fragment_file_path){
		// Create and compile our GLSL program from the shaders
		programID = LoadShaders(vertex_file_path, fragment_file_path);
		return programID;
	}

	bool LoadOBJ(const char *file_path){
		bool res = loadOBJ(file_path, vertices, uvs, normals);

		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	void Bind(){
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

	}

	void Render(){
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
	}

	void UnBind(){
		
	}

	~Model3D(){
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Cleanup VBO and shader
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteBuffers(1, &uvBufferID);
		glDeleteProgram(programID);
		glDeleteVertexArrays(1, &vertexArrayID);
	}

	GLuint GetProgramID(){return programID;}
private:
	GLuint vertexArrayID;
	GLuint vertexBufferID, uvBufferID;
	GLuint programID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
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


	Model3D obj;
	obj.InitShaders("res/vertex.shader", "res/fragment.shader");
	obj.LoadOBJ("res/cube.obj");

	do{
		obj.Bind();

		// Use our shader
		glUseProgram(obj.GetProgramID());

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs(window, 640, 480);
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(glGetUniformLocation(obj.GetProgramID(), "MVP"), 1, GL_FALSE, &MVP[0][0]);

		obj.Render();
		obj.UnBind();

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