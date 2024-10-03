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
		return false;
	}

	//-----------------------------------------------------------------------------
	//Create instance
	//-----------------------------------------------------------------------------

	//Vulkan relies on stacks of layers to perform
	//specific tasks (more will be added later on)
	std::vector<const char*> validationLayers;
	validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
	
	//The extensions are used to add extra functionality to Vulkan and 
	//as such are not part of the Vulkan core API (similar to OpenGL)
	std::vector<const char*> extensions;
	extensions.emplace_back("VK_KHR_surface");           //binds the GLFW window to Vulkan
	extensions.emplace_back("VK_KHR_win32_surface");     //Windows specific 
	extensions.emplace_back("VK_EXT_debug_utils");       //for debugging 

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
	instanceCreatInfo.enabledLayerCount = validationLayers.size();
	instanceCreatInfo.ppEnabledLayerNames = validationLayers.data();
	instanceCreatInfo.enabledExtensionCount = extensions.size();
	instanceCreatInfo.ppEnabledExtensionNames = extensions.data();

	if (vkCreateInstance(&instanceCreatInfo, nullptr, &instance) != VK_SUCCESS)
	{
		Utility::Log("Vulkan instance not created.");
		return false;
	}

	//-----------------------------------------------------------------------
	//Create surface
	//-----------------------------------------------------------------------
	
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		Utility::Log("Vulkan surface not created.");
		return false;
	}
	
	//-----------------------------------------------------------------------
	//Setup physical devices
	//-----------------------------------------------------------------------

	uint32_t totalDevices{ 0 };

	//Get the total hardware devices found/supported on this machine
	if (vkEnumeratePhysicalDevices(instance, &totalDevices, nullptr) != VK_SUCCESS)
	{
		Utility::Log("Error enumerating physical devices.");
		return false;
	}

	Utility::Log("Total physical device(s) found: " + std::to_string(totalDevices));

	physicalDevices.resize(totalDevices);

	std::vector<VkPhysicalDevice> devices;  //PhysDev
	devices.resize(totalDevices);

	//Call this a second time, this time knowing the device total and to acquire the devices
	vkEnumeratePhysicalDevices(instance, &totalDevices, devices.data());

	//Loop through all devices found/supported and get ALL the details for each one
	//For each device in 'devices' we have to explicitly extract the hardware data
	for (uint32_t i = 0; i < totalDevices; i++)
	{
		physicalDevices[i].physicalDevice = devices[i];

		vkGetPhysicalDeviceProperties(devices[i], &physicalDevices[i].deviceProperties);

		//Display device name, version, etc. Would be better to do this somewhere else------------------
		Utility::Log("Device name: " + std::string(physicalDevices[i].deviceProperties.deviceName));

		auto apiVersion = physicalDevices[i].deviceProperties.apiVersion;

		Utility::Log("API version: " + std::to_string(VK_API_VERSION_VARIANT(apiVersion)) + "." +
			std::to_string(VK_API_VERSION_MAJOR(apiVersion)) + "." +
			std::to_string(VK_API_VERSION_MINOR(apiVersion)) + "." +
			std::to_string(VK_API_VERSION_PATCH(apiVersion)));
		//----------------------------------------------------------------------------------------------

		//Queue families are groups of command queues that are filled with rendering instructions
		uint32_t totalQueueFamilies{ 0 };
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &totalQueueFamilies, nullptr);
		Utility::Log("Total queue families: " + std::to_string(totalQueueFamilies));

		physicalDevices[i].familyProperties.resize(totalQueueFamilies);
		physicalDevices[i].supportsPresent.resize(totalQueueFamilies);

		vkGetPhysicalDeviceQueueFamilyProperties(devices[i],
			&totalQueueFamilies,
			physicalDevices[i].familyProperties.data());

		for (uint32_t j = 0; j < totalQueueFamilies; j++)
		{
			const VkQueueFamilyProperties& queueFamilyProperty = physicalDevices[i].familyProperties[j];

			Utility::Log("Queue Family " +
				std::to_string(j) +
				std::to_string(queueFamilyProperty.queueCount) +
				" queues");

			VkQueueFlags flags = queueFamilyProperty.queueFlags;

			//Use a mask to check for true/false. Bug here - labels not displaying, only the yes/no
			Utility::Log("GFX: " + std::string((flags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No"));
			Utility::Log("Compute: " + std::string((flags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No"));
			Utility::Log("Transfer: " + std::string((flags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No"));
			Utility::Log("Sparse binding: " + std::string((flags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No"));

			vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], j, surface, &(physicalDevices[i].supportsPresent[j]));
		}

		uint32_t totalFormats{ 0 };

		if (vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i], surface, &totalFormats, nullptr) != VK_SUCCESS)
		{
			Utility::Log("Error getting surface formats");
		}

		assert(totalFormats > 0);

		physicalDevices[i].surfaceFormats.resize(totalFormats);

		vkGetPhysicalDeviceSurfaceFormatsKHR(devices[i],
			surface,
			&totalFormats,
			physicalDevices[i].surfaceFormats.data());


		for (uint32_t j = 0; j < totalFormats; j++)
		{
			const auto& surfaceFormat = physicalDevices[i].surfaceFormats[j];
			Utility::Log("Format: " + std::to_string(surfaceFormat.format));
			Utility::Log("Color space: " + std::to_string(surfaceFormat.colorSpace));
		}

		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(devices[i], surface, &(physicalDevices[i].surfaceCapabilities)) != VK_SUCCESS)
		{
			Utility::Log("Error getting device surface capabilities");
		}

		//This is a separate function - todo later!
		//PrintImageUsageFlags(m_devices[i].m_surfaceCaps.supportedUsageFlags);

		//Present modes are various options for framebuffer rendering. OpenGL had one 
		//which allowed swapping the framebuffer. Vulkan has multiple rendering options
		uint32_t totalPresentModes{ 0 };

		if (vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], surface, &totalPresentModes, nullptr) != VK_SUCCESS)
		{
			Utility::Log("Error gettig device surface present modes");
		}

		assert(totalPresentModes != 0);
		physicalDevices[i].presentModes.resize(totalPresentModes);

		vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], 
												  surface, 
												  &totalPresentModes, 
												  physicalDevices[i].presentModes.data());
		
		Utility::Log("Total presentation modes: " + std::to_string(totalPresentModes));

		vkGetPhysicalDeviceMemoryProperties(devices[i], &(physicalDevices[i].memoryProperties));

		auto totalMemoryTypes{ physicalDevices[i].memoryProperties.memoryTypeCount };

		Utility::Log("Total memory types: " + std::to_string(totalMemoryTypes));
		
		for (uint32_t j = 0; j < totalMemoryTypes; j++)
		{
			Utility::Log(std::to_string(j) + ": " +
				std::to_string(physicalDevices[i].memoryProperties.memoryTypes[j].propertyFlags) +
				std::to_string(physicalDevices[i].memoryProperties.memoryTypes[j].heapIndex));

			//Another function to copy later - todo!
			//PrintMemoryProperty(m_devices[i].m_memProps.memoryTypes[j].propertyFlags);
		}

		Utility::Log("Total heap types: " + std::to_string(physicalDevices[i].memoryProperties.memoryHeapCount));

		vkGetPhysicalDeviceFeatures(physicalDevices[i].physicalDevice, &physicalDevices[i].deviceFeatures);
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

	//Destroy surface
	auto vkDestroySurface = 
	(PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");

	if (!vkDestroySurface) 
	{
		Utility::Log("Cannot find address of 'vkDestroySurfaceKHR'");
	}

	vkDestroySurface(instance, surface, nullptr);

	//Destroy debugger
	auto vkDestroyDebugUtilsMessenger =
	(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (!vkDestroyDebugUtilsMessenger)
	{
		Utility::Log("Cannot find address of 'vkDestroyDebugUtilsMessenger'");
	}

	vkDestroyDebugUtilsMessenger(instance, Utility::debugger, nullptr);

	vkDestroyInstance(instance, nullptr);
}