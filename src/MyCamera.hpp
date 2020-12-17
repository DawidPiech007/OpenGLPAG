#ifndef MY_CAMERA
#define MY_CAMERA

#include <glm/mat4x4.hpp>
//#include "GraphNode.hpp"
#include <GLFW/glfw3.h>


class MyCamera
{
public:
	MyCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float scrWidth, float scrHeight);

	glm::mat4 GetView();
	glm::mat4 GetProjection();

	void InputKey(GLFWwindow* window, float deltaTime);
	void InputMouse(float xpos, float ypos);
	void InputScroll(float yoffset);

private:

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 projection;

	float lastX;
	float lastY;

	float yaw;
	float pitch;

	float fov;

	bool firstMouse;

	float scrWidth;
	float scrHeight;
};

#endif