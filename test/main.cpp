#include <vin.hpp>

class TestApp : public Vin::Application {
public:
	TestApp(const Vin::ApplicationInfo& info) : Application{ info } {}
};

//int main(int argc, char* argv[]) {
	//testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();
//}

Vin::Application* Vin::CreateApp() {
	Vin::ApplicationInfo info{};

	info.name = "Application test";

	return new TestApp{ info };
}

void Vin::DestroyApp(Vin::Application* app) {
	delete app;
}
