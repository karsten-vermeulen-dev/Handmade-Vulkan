#include <algorithm>
#include <assert.h>
#include <map>
#include <vector>

#include "Input.h"
#include "Screen.h"
#include "Utility.h"

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
bool Screen::Initialize()
{
	
	if (glfwInit() == GLFW_FALSE)
	{
		Utility::Log("GLFW did not initialize properly.");
		return false;
	}

	if (!glfwVulkanSupported())
	{
		Utility::Log("Vulkan is not supported by your graphics card.");
		return false;
	}

	//This is a Vulkan app, so switch OpenGL support off completely
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(1280, 720, "Handmade Vulkan", nullptr, nullptr);
	
	if (!window)
	{
		Utility::Log("Application window could not be created.");
		glfwTerminate();
		return false;
	}

	//-----------------------------------------------------------------------------
	//Create instance
	//-----------------------------------------------------------------------------

	//The layers and extensions seem to be optional right now as Vulkan still 
	//initializes without them. Return here later to manage and make changes

	//Vulkan relies on stacks of layers to perform specific tasks (more to be added later)
	std::vector<const char*> layers;
	layers.emplace_back("VK_LAYER_KHRONOS_validation");
	
	//The extensions are used to add extra functionality to Vulkan and 
	//as such are not part of the Vulkan core API (similar to OpenGL)
	std::vector<const char*> extensions;
	extensions.emplace_back("VK_KHR_surface");           //binds the GLFW window to Vulkan
	extensions.emplace_back("VK_KHR_win32_surface");     //Windows specific 
	extensions.emplace_back("VK_EXT_debug_utils");       //for debugging 
	//extensions.emplace_back("VK_EXT_debug_report");      //for debugging  (deprecated)

	VkApplicationInfo applicationInfo;
	
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;             
	applicationInfo.pNext = nullptr;                                        
	applicationInfo.pApplicationName = "Handmade Vulkan";
	applicationInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	applicationInfo.pEngineName = "Handmade Vulkan";
	applicationInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceCreatInfo;

	instanceCreatInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreatInfo.pNext = nullptr;
	instanceCreatInfo.flags = 0;
	instanceCreatInfo.pApplicationInfo = &applicationInfo;
	instanceCreatInfo.enabledLayerCount = layers.size();
	instanceCreatInfo.ppEnabledLayerNames = layers.data();
	instanceCreatInfo.enabledExtensionCount = extensions.size();
	instanceCreatInfo.ppEnabledExtensionNames = extensions.data();

	if (vkCreateInstance(&instanceCreatInfo, nullptr, &instance) != VK_SUCCESS)
	{
		Utility::Log("Vulkan instance not created.");
		return false;
	}

	//-----------------------------------------------------------------------
	// Setup debugging
	//-----------------------------------------------------------------------

	VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo;
	
	messengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messengerCreateInfo.pNext = nullptr;
	messengerCreateInfo.flags = 0;
	messengerCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
										  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	messengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	messengerCreateInfo.pfnUserCallback = &Utility::OnError;
	messengerCreateInfo.pUserData = nullptr;
	
	//Since this is an extension of core Vulkan, we need to get the function address 
	auto vkCreateDebugUtilsMessenger = 
	(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	
	if (!vkCreateDebugUtilsMessenger) 
	{
		Utility::Log("Cannot find address of 'vkCreateDebugUtilsMessenger'");
		return false;
	}

	if (vkCreateDebugUtilsMessenger(instance, &messengerCreateInfo, nullptr, &Utility::debugger) != VK_SUCCESS)
	{
		Utility::Log("Error creating Vulkan debugger");
		return false;
	}
	

	
	
	
	
	//-----------------------------------------------------------------------

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

	auto vkDestroyDebugUtilsMessenger =
	(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (!vkDestroyDebugUtilsMessenger)
	{
		Utility::Log("Cannot find address of 'vkDestroyDebugUtilsMessenger'");
		//return false;
	}

	vkDestroyDebugUtilsMessenger(instance, Utility::debugger, nullptr);

	vkDestroyInstance(instance, nullptr);
}