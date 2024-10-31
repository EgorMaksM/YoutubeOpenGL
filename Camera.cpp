#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos)
{
	Camera::width = width;
	Camera::height = height;
	Camera::Position = pos;
};

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projection * view;
};

void Camera::Matrix(Shader& shader, const char* uniform)
{
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* win)
{
	                         // KEYBOARD
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) //MoveForward
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) //MoveForward -1
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) //MoveRight
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) //MoveRight -1
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) //MoveUpward
	{
		Position += speed * Up;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //MoveUpward -1
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) //MoveUpward -1
	{
		speed = 0.04f;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) //MoveUpward -1
	{
		speed = 0.01f;
	}

	                              // MOUSE
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick)
		{
			glfwSetCursorPos(win, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX, mouseY;
		glfwGetCursorPos(win, &mouseX, &mouseY);

		// Note: rotX is VERTICAL ROTATION, as in 2D graph, not horizontal like in UE4
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		// Note: which means rotY is HORIZONTAL ROTATION, so rotX shall be clamped, not rotY
		float rotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		if (!((glm::angle(newOrientation, Up) <= glm::radians(5.0f)) or (glm::angle(newOrientation, -Up) <= glm::radians(5.0f))))
		{
			// clamped X-axis(VERTICAL) rotation, in order to prevent barrel rolls
			Orientation = newOrientation;
		}

		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
		glfwSetCursorPos(win, (width / 2), (height / 2));
	}
	if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
};