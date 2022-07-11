#include <vin.hpp>
#include <error.hpp>
#include <exception>
#include <luaaa/luaaa.hpp>
#include <ecs.hpp>
#include <iostream>
#include <logger.hpp>

struct Transform {
    float x, y;
    float rx, ry;
    float sx, sy;
};

struct Collider {
    float extendx, extendy;
    float centerx, centery;
};

void MySystem(Vin::EntityId entityId, Transform& transform, Collider& collider) {
    std::cout << entityId << std::endl;
}

int main(int argc, char* argv[]) {
	try {
        Vin::Logger::AddLogOutputStream(&std::cout);
        Vin::Logger::Log("Log working.");

        Vin::Init();

        Vin::Terminate();
	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}
}