#include <vin/vin.h>
#include <vin/allocator/stdallocator.h>
#include <iostream>
#include <vector>
#include <chrono>

class BaseModule {
public:
    virtual void Update(size_t i) = 0;
};

class App {
public:
    void AddModule(BaseModule* module) {
        modules.emplace_back(module);
    }

    void CallUpdate(size_t i) {
        for (auto& module : modules)
            module->Update(i);
    }
private:
    std::vector<BaseModule*> modules{};
};

class Module1 : public BaseModule {
public:
    void Update(size_t i) final {
        v += 2 * i;
        if (v % 457875634 == 0)
            printf("%ld\n", v);
    }
private:
    size_t v = 0;
};

class Module2 : public BaseModule {
public:
    void Update(size_t i) final {
        v += 4 + i;
        if (v % 457875634 == 0)
            printf("%ld\n", v);
    }
private:
    size_t v = 0;
};

template<class... T>
class AppS {
public:
    inline void CallUpdate(size_t i) {
    }
private:
    
};

int main() {
    App app{};
    app.AddModule(new Module1{});
    app.AddModule(new Module2{});

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 10000000; ++i)
        app.CallUpdate(i);

    auto end = std::chrono::high_resolution_clock::now();
    printf("Virtual Module took %ld ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
}
