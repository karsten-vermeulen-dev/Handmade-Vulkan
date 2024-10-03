#include <iostream>
#include "Utility.h"

void Utility::Log(const std::string& message)
{
	std::cout << message << std::endl;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Utility::OnError(VkDebugUtilsMessageSeverityFlagBitsEXT severity, 
	                                            VkDebugUtilsMessageTypeFlagsEXT messageType, 
	                                            const VkDebugUtilsMessengerCallbackDataEXT* callbackData, 
	                                            void* userData)
{
	std::string errorMessage;

	switch (severity)
	{
	case ((int)Severity::Info):
		errorMessage += "[INFO] ";
		break;
	
	case ((int)Severity::Warning):
		errorMessage += "[WARNING] ";
		break;

	case ((int)Severity::Verbose):
		errorMessage += "[VERBOSE] ";
		break;

	case ((int)Severity::Error):
		errorMessage += "[ERROR] ";
		break;
	}
	
	//Add on the actual error message
	errorMessage += std::string(callbackData->pMessage) + " ";

	switch (messageType)
	{
	case ((int)MessageType::General):
		errorMessage += "(General)";
		break;

	case ((int)MessageType::Performance):
		errorMessage += "(Performance)";
		break;

	case ((int)MessageType::Validation):
		errorMessage += "(Validation)";
		break;
	}
	
	Log(errorMessage);
	Log("Objects concerned: ");

	for (int i = 0; i < callbackData->objectCount; i++) 
	{
		Log("Object " + std::to_string(i) + ": " + std::to_string(callbackData->pObjects[i].objectHandle));
	}

	return VK_FALSE;
}
