#pragma once

int main(int argc, char* argv[]);

namespace Vin {
	struct ApplicationInfo {
		const char* name{ "Application" };
	};

	class Application {
	public:
		Application(const ApplicationInfo& info);
		virtual ~Application();
	private:
		ApplicationInfo m_ApplicationInfo;
	};

	Application* CreateApp();
	void DestroyApp(Application* app);
}
