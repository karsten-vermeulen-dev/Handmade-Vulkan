#include <algorithm>
#include <assert.h>
#include <map>
#include "Input.h"
#include "Screen.h"

#include <iostream>

//======================================================================================================
Screen* Screen::Instance()
{
	static Screen* screenObject = new Screen();
	return screenObject;
}

bool Screen::IsXClicked()
{
	return isXClicked;
}

bool Screen::IsWindowResized()
{
	return IsWindowResized;
}

void Screen::CloseWindowCallback(GLFWwindow* window)
{
	isXClicked = true;
}

void Screen::WindowResizeCallback(GLFWwindow* window, int width, int height)
{
	//resolution.x = width;
	//resolution.y = height;
	isWindowResized = true;
}

//======================================================================================================
bool Screen::Initialize(const std::string& filename)
{
	std::map<std::string, std::string> dataMap;

	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "GLFW did not initialize properly." << std::endl;
		return false;
	}

	int profile = std::stoi(dataMap["Core"]) == 1 ? GLFW_OPENGL_CORE_PROFILE
		: GLFW_OPENGL_COMPAT_PROFILE;

	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, std::stoi(dataMap["Major"]));
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, std::stoi(dataMap["Minor"]));

	

	window = glfwCreateWindow(std::stoi(dataMap["Width"]),
							  std::stoi(dataMap["Height"]),
							  dataMap["Name"].c_str(), 
		                      nullptr, nullptr);
	
	if (!window)
	{
		std::cout << "Application window could not be created." << std::endl;
		glfwTerminate();
		return false;
	}

	//Make the context of the window the main context on the current thread
	glfwMakeContextCurrent(window);
	

	
	//resolution.x = std::stoi(dataMap["Width"]);
	//resolution.y = std::stoi(dataMap["Height"]);
	
	//SetViewport(0, 0, resolution.x, resolution.y);


	//Assign callback functions inside the Input manager
	Input::Instance()->AssignCallbacks(window);
	
	//Assign our own native callback functions
	glfwSetWindowCloseCallback(window, Screen::CloseWindowCallback);
	glfwSetFramebufferSizeCallback(window, Screen::WindowResizeCallback);
	


	return true;
}


//======================================================================================================
void Screen::Refresh() const
{
	
}
//======================================================================================================
void Screen::Present() const
{
	
	glfwSwapBuffers(window);
}
//======================================================================================================
void Screen::Shutdown() const
{
	glfwTerminate();
}