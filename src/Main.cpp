#include <iostream>
#include "Input.h"
#include "Screen.h"

int main()
{
	bool isAppRunning{ true };

	if (!Screen::Instance()->Initialize())
	{
		return 0;
	}
	
	std::cout << "Hello Vulkan!" << std::endl;

	while (isAppRunning)
	{
		Input::Instance()->Update();
		isAppRunning = !Screen::IsXClicked();
	}

	Screen::Instance()->Shutdown();

	return 0;
}