#include <vin/application/application.h>

void Vin::Application::App::Run() {
    for (auto& module : modules)
        module->Initialize();

    Loop();

    for (auto module = modules.rbegin(); module != modules.rend(); ++module)
        module->Get()->Uninitialize();
}

void Vin::Application::App::Loop() {

    bool shouldClose = false;

    while(!shouldClose) {
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->EarlyUpdate();
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->Update();
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->LateUpdate();
    }

}