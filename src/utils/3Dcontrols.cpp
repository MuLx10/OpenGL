#include "3Dcontrols.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

double PI = 3.1412;

// Field of View
float FoV = 100.0f;

float speed = 3.0f; // 3 units / second (I don't know what unit, I just added a random value)

float R = 3.0f; // Distance from origin/size of object

float mouseSpeed = 0.01f;

void computeMatricesFromInputs(GLFWwindow* window, int width, int height){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);


	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		R -=  deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		R +=  deltaTime * speed;
	}

	float angleXZ = mouseSpeed * float(width/2 - xpos );
	float angleYZ = mouseSpeed * float( height/2 - ypos );
	glm::vec3 camera = glm::vec3(R*cos(angleXZ)*cos(angleYZ), R*sin(angleYZ) ,R*sin(angleXZ)*cos(angleYZ));
	glm::vec3 look = glm::vec3(0,0,0);


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
																	camera,           // Camera is here
																	look, // and looks here : at the same position, plus "direction"
																	glm::vec3(0,1,0)                  // Head is up (set to 0,-1,0 to look upside-down)
															   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}