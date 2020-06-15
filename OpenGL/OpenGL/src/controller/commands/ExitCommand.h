#include "ICommand.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class ExitCommand : public ICommand
{
public:
	ExitCommand(GLFWwindow* Window) {
		window = Window;
	}

	void execute()
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
private:
	GLFWwindow* window;
};