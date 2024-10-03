#include <iostream>
#include "Input.h"
#include "Screen.h"

int main()
{
	bool isAppRunning{ true };

	std::cout << "Hello Vulkan!" << std::endl;

	Screen::Instance()->Initialize();

	while (isAppRunning)
	{
		Input::Instance()->Update();
		isAppRunning = !Screen::IsXClicked();
	}

	return 0;
}