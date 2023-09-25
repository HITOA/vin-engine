#include <vin/application/application.h>

void Vin::Application::App::Run() {
    for (auto& module : modules)
        module->Initialize();
}