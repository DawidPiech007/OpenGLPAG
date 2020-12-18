
#include <MyCamera.hpp>
#include <glm\ext\matrix_transform.hpp>
#include <glad\glad.h>
#include <glm\ext\matrix_clip_space.hpp>

MyCamera::MyCamera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float scrWidth, float scrHeight):
	cameraPos(cameraPos), cameraFront(cameraFront), cameraUp(cameraUp), scrWidth(scrWidth), scrHeight(scrHeight)
{
    firstMouse = true;
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 45.0f;
    scrWidth = 1280;
    scrHeight = 720;

    projection = glm::perspective(glm::radians(fov), scrWidth / scrHeight, 0.1f, 100.0f);
}

glm::mat4 MyCamera::GetView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 MyCamera::GetProjection()
{
    return projection;
}


void MyCamera::InputKey(GLFWwindow* window, float deltaTime)
{
    float cameraSpeed = 2.0f * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

void MyCamera::InputMouse(float xpos, float ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void MyCamera::InputScroll(float yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;

    projection = glm::perspective(glm::radians(fov), scrWidth / scrHeight, 0.1f, 100.0f);
}
