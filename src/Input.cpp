#include "Input.h"
#include "Screen.h"

//======================================================================================================
Input* Input::Instance()
{
	static Input* inputObject = new Input();
	return inputObject;
}

const Input::State& Input::GetState() const
{
	return state;
}

void Input::AssignCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, Input::OnKeyPress);
	glfwSetCursorPosCallback(window, Input::OnMouseMove);
	glfwSetMouseButtonCallback(window, Input::OnMouseClick);
	glfwSetScrollCallback(window, Input::OnMouseWheel);
}

void Input::OnMouseMove(GLFWwindow* window, double x, double y)
{
	//state.mouseMotion.x = (x - state.mousePosition.x) / Screen::Instance()->GetResolution().x;
	//state.mouseMotion.y = (y - state.mousePosition.y) / Screen::Instance()->GetResolution().y;
	
	//state.mousePosition.x = x;
	//state.mousePosition.y = y;
}

void Input::OnMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			state.isLeftButtonClicked = true;
			break;
		}

		case  GLFW_MOUSE_BUTTON_RIGHT:
		{
			state.isRightButtonClicked = true;
			break;
		}

		case  GLFW_MOUSE_BUTTON_MIDDLE:
		{
			state.isMiddleButtonClicked = true;
			break;
		}
		}

		break;
	}

	case GLFW_RELEASE:
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
		{
			state.isLeftButtonClicked = false;
			break;
		}

		case  GLFW_MOUSE_BUTTON_RIGHT:
		{
			state.isRightButtonClicked = false;
			break;
		}

		case  GLFW_MOUSE_BUTTON_MIDDLE:
		{
			state.isMiddleButtonClicked = false;
			break;
		}
		}
	}

	}
}

void Input::OnKeyPress(GLFWwindow* window, int keyCode, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
	{
		state.isKeyPressed = true;
		state.keyUp = keyCode;
		break;
	}

	case GLFW_RELEASE:
	{
		state.isKeyPressed = false;
		state.keyUp = keyCode;
		break;
	}

	}
}

void Input::OnMouseWheel(GLFWwindow* window, double x, double y)
{
	//state.mouseWheel.x = x;
	//state.mouseWheel.y = y;
}

//======================================================================================================
void Input::Update()
{
	
	//state.mouseWheel = glm::ivec2(0);
	
	glfwPollEvents();
	
}