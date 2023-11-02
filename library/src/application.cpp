#include <vin/application/application.h>
#include <vin/time/timer.h>

void Vin::App::Run() {
    for (auto& module : modules)
        module->Initialize();

    Loop();

    for (auto module = modules.rbegin(); module != modules.rend(); ++module)
        module->Get()->Uninitialize();
}

void Vin::App::Loop() {

    Timer timer{};
    TimeStep lastStep = timer.GetTimeStep();

    bool shouldClose = false;

    while(!shouldClose) {
        TimeStep curr = timer.GetTimeStep();
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->EarlyUpdate(curr - lastStep);
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->Update(curr - lastStep);
        for (auto module = modules.rbegin(); module != modules.rend(); ++module)
            module->Get()->LateUpdate(curr - lastStep);
        Vin::Core::MemoryManager::GetInstance()->SwapAndReset();
        lastStep = curr;
    }

}