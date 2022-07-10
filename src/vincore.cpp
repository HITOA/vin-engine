#define GLFW_INCLUDE_NONE

#include <vincore.hpp>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdexcept>
#include <ecs.hpp>
#include <component.hpp>

namespace Vin {

	struct GlfwWindowDeleter {
		void operator()(GLFWwindow* p) {
			glfwDestroyWindow(p);
		}
	};

	namespace {
		bool _isInit;
		std::string _wTitle;
		int _wWidth, _wHeight;
		std::unique_ptr<GLFWwindow, GlfwWindowDeleter> _window;
		std::function<void()> _update;
	}

	void GlfwErrorCallback(int errcode, const char* description) {
		printf("Err %i : %s", errcode, description);
	}

	void InitGlfw() {
		glfwSetErrorCallback(GlfwErrorCallback);

		if (!glfwInit())
			throw std::runtime_error{ "Glfw failed to init." };

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = std::unique_ptr<GLFWwindow, GlfwWindowDeleter>{
			glfwCreateWindow(_wWidth, _wHeight, _wTitle.data(), NULL, NULL)
		};

		glfwMakeContextCurrent(_window.get());
	}

	void InitGlad() {
		if (!gladLoadGL(glfwGetProcAddress))
			throw std::runtime_error("Glad failed to init.");

		glViewport(0, 0, _wWidth, _wHeight);
	}

	void Init(const std::string& wTitle, int wWidth, int wHeight)
	{
		if (_isInit)
			return;

		_wTitle = wTitle;
		_wWidth = wWidth;
		_wHeight = wHeight;

		if (wTitle.empty())
			throw std::runtime_error{ "Window title is empty." };

		if ((_wWidth & _wHeight) == 0)
			throw std::runtime_error{ "Window size is 0." };

		InitGlfw();
		InitGlad();

		_isInit = true;
	}

	void RenderEntity(EntityId entityId, Component::Renderable& dummy) {

	}

	void Run()
	{
		while (!glfwWindowShouldClose(_window.get())) {

			if (_update)
				_update();

			EntityManager::ForEach(RenderEntity);

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(_window.get());
			glfwPollEvents();
		}
	}

	void Terminate()
	{
		if (!_isInit)
			return;

		glfwTerminate();

		_isInit = false;
	}

	void SetUpdateCallback(std::function<void()> callback)
	{
		_update = callback;
	}

}