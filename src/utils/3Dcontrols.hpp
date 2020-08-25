#ifndef CONTROLS3D_HPP
#define CONTROLS3D_HPP

#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void computeMatricesFromInputs(GLFWwindow* window, int width, int height);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif