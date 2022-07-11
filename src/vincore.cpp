#define GLFW_INCLUDE_NONE

#include <vincore.hpp>
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdexcept>
#include <ecs.hpp>
#include <component.hpp>
#include <logger.hpp>

namespace Vin::Core {

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

	void Run()
	{
		while (!glfwWindowShouldClose(_window.get())) {

			if (_update)
				_update();

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

	bool CheckForShaderCompilationErr(unsigned int shaderId) {
		int result;
		char infoLog[512];
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
			Logger::Warn("Shader compilation error : {}", infoLog);
			return false;
		}
	}

	bool CheckForProgramCompilationErr(unsigned int programId) {
		int result;
		char infoLog[512];
		glGetProgramiv(programId, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			Logger::Warn("Program compilation error : {}", infoLog);
			return false;
		}
	}

	unsigned int CompileShader(const char* vSrc, const char* fSrc)
	{
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vSrc, NULL);
		glCompileShader(vertexShader);
		if (!CheckForShaderCompilationErr(vertexShader))
			return -1;

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &vSrc, NULL);
		glCompileShader(fragmentShader);
		if (!CheckForShaderCompilationErr(fragmentShader))
			return -1;

		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		if (!CheckForProgramCompilationErr(shaderProgram))
			return -1;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

}