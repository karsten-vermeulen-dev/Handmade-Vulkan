#pragma once
#include <string>
#include <vulkan.h>

class Utility
{
public:

	//Mapped my own flags for readiblity
	enum class Severity
	{
		Verbose = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
		Info = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT,
		Warning = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
		Error = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
	};

	enum class MessageType
	{
		General = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
		Validation = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
		Performance = VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
	};

	//To be replaced by robust ImGui message boxes
	static void Log(const std::string& message);

	static VKAPI_ATTR VkBool32 VKAPI_CALL OnError(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
												  VkDebugUtilsMessageTypeFlagsEXT messageType,
												  const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
												  void* userData);

	inline static VkDebugUtilsMessengerEXT debugger{ nullptr };

private:


};