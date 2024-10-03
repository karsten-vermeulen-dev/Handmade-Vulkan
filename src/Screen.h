#pragma once
#include <map>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

class Screen
{

public:

	static Screen* Instance();

	static bool IsXClicked();

	static bool IsWindowResized();

	bool Initialize();

	void Refresh() const;
	void Present() const;
	void Shutdown() const;

	static void CloseWindowCallback(GLFWwindow* window);
	static void WindowResizeCallback(GLFWwindow* window, int width, int height);

private:

	Screen() {}
	Screen(const Screen&);
	Screen& operator=(Screen&);

	inline static bool isXClicked{ false };
	//inline static glm::ivec2 resolution{ 0 };
	inline static bool isWindowResized{ false };
	
	GLFWwindow* window{ nullptr };

};