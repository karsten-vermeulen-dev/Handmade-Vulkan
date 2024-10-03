#pragma once

#include "Screen.h"

//Massive WIP!
//Some helpful docs for GLFW events:
//https://www.glfw.org/docs/3.3/group__window.html
//https://www.glfw.org/docs/3.3/input_guide.html#events
//https://www.glfw.org/docs/3.3/group__input.html#ga1caf18159767e761185e49a3be019f8d

class Input
{

public:

	static Input* Instance();

	struct State
	{
		int keyUp{ 0 };
		int keyDown{ 0 };
		bool isKeyPressed{ false };

		//Will add GLM later
		//glm::dvec2 mouseWheel{ 0 };
		//glm::dvec2 mouseMotion{ 0 };
		//glm::dvec2 mousePosition{ 0 };

		bool isLeftButtonClicked{ false };
		bool isRightButtonClicked{ false };
		bool isMiddleButtonClicked{ false };
	};

	const State& GetState() const;

	void AssignCallbacks(GLFWwindow* window);

	//GLFW will invoke these functions when keyboard and mouse are used
	static void OnMouseMove(GLFWwindow* window, double x, double y);
	static void OnMouseClick(GLFWwindow* window, int button, int action, int mods);
	static void OnKeyPress(GLFWwindow* window, int keyCode, int scancode, int action, int mods);
	static void OnMouseWheel(GLFWwindow* window, double x, double y);

	void Update();

private:

	Input() {}
	Input(const Input&);
	Input& operator=(const Input&);

	inline static State state;

};