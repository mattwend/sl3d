
#include <exception>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cout;
using std::cerr;
using std::string;

namespace sl3d
{
	template <const char *S>
	class glfw_error : public std::exception
	{
		virtual const char* what() const noexcept { return S; }
	};

	char _init_error[] = "Failed to initialize GLFW"; 
	class glfw_error<_init_error> glfw_init_error;
	char _createwindow_error[] = "Failed to create Window"; 
	class glfw_error<_createwindow_error> glfw_CreateWindow_error;

	class Backend_OGL
	{
	public:
		Backend_OGL() : Backend_OGL(640, 480, "Default_Window_Title", NULL) {};
		Backend_OGL(int width, int height, string title, GLFWmonitor* monitor) : 
			_window_width (width),
			_window_height (height),
			_window_title (title),
			_monitor (monitor)
		{
			glfwSetErrorCallback(error_callback);

			if(!glfwInit())
			{
				throw glfw_init_error;
			}

			_window = glfwCreateWindow(_window_height, _window_height, _window_title.c_str(), _monitor, NULL);
			if(!_window)
			{
				throw glfw_CreateWindow_error;
			}
			// TODO CONTEXT_VERSION
			
			glfwMakeContextCurrent(_window);
			glewInit();
		}

		~Backend_OGL()
		{
			glfwDestroyWindow(_window);
			glfwTerminate();
		}

		virtual void render() = 0;

		static void error_callback(int error, const char* description)
		{
			cerr << "Error" << description;
		}

		inline void set_window_title(string title) { _window_title = title; }
	private:
		int		_window_width;
		int		_window_height;
		string		_window_title;
	 	GLFWmonitor*	_monitor;
	protected:
		GLFWwindow*	_window;
	};

	class Backend_Vulkan
	{
	public:
		virtual void render() = 0;
	};
}

using namespace sl3d;

class TestApp : public Backend_OGL
{
public:
	TestApp()
	{
		while (!glfwWindowShouldClose(_window))
		{
			render();
		}
	}
	void render()
	{
		static const GLfloat blue[] = {0.0f, 0.0f, 1.0f};

		glClearBufferfv(GL_COLOR, 0, blue);
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
};

int main()
{
	try
	{
		TestApp my_app;
	}
	catch(std::exception& e)
	{
		cerr << e.what() << "\n";
		return -1;
	}

	return 0;
}
