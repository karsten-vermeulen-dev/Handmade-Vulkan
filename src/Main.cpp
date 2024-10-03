#include <iostream>
#include "Input.h"
#include "Screen.h"
#include "Utility.h"

int main()
{
	bool isAppRunning{ true };

	if (!Screen::Instance()->Initialize())
	{
		Utility::Log("Application did not initialize properly. Shutting down...");
		Screen::Instance()->Shutdown();
		system("pause");
		return 0;
	}

	Utility::Log("Hello Vulkan!");

	while (isAppRunning)
	{
		Input::Instance()->Update();
		isAppRunning = !Screen::IsXClicked();
	}

	Screen::Instance()->Shutdown();

	return 0;
}