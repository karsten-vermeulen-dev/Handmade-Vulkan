#pragma once
#include <map>
#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

class Screen
{

public:

	struct PhysicalDevice
	{
		VkPhysicalDevice physicalDevice;
		VkPhysicalDeviceProperties deviceProperties;
		std::vector<VkQueueFamilyProperties> familyProperties;
		std::vector<VkBool32> supportsPresent;
		std::vector<VkSurfaceFormatKHR> surfaceFormats;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		VkPhysicalDeviceMemoryProperties memoryProperties;
		std::vector<VkPresentModeKHR> presentModes;
		VkPhysicalDeviceFeatures deviceFeatures;
	};

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
	
	//This is the hardware that the Vulkan instance will expose (GPU/CPU)
	std::vector<PhysicalDevice> physicalDevices;

	GLFWwindow* window{ nullptr };

	//The interface between the application and the hardware/devices
	//Similar to the older OpenGL context/state machine but more robust
	VkInstance instance{ nullptr };

	//The surface acts as a type of 'canvas' or 
	//rendering target based on the OS you have
	VkSurfaceKHR surface{ nullptr };

};