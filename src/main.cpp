#include <vincore.hpp>
#include <error.hpp>
#include <exception>
#include <luaaa/luaaa.hpp>
#include <ecs.hpp>
#include <iostream>

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
        Vin::Init("App Test", 600, 400);
        Vin::Run();
        Vin::Terminate();

        Vin::EntityId entityA = Vin::EntityManager::CreateEntity();
        Vin::EntityId entityB = Vin::EntityManager::CreateEntity();

        Transform transform{};
        Collider collider{};

        Vin::EntityManager::AddComponent(entityA, transform);
        Vin::EntityManager::AddComponent(entityB, transform, collider);

        Vin::EntityManager::ForEach(MySystem);

        Vin::EntityManager::RemoveComponent<Transform>(entityA);
        Vin::EntityManager::RemoveComponent<Transform, Collider>(entityB);
        Vin::EntityManager::AddComponent(entityB, transform, collider);
        Vin::EntityManager::DestroyEntity(entityB);

        const luaL_Reg lualibs[] = {
        { LUA_COLIBNAME, luaopen_base },
        { LUA_LOADLIBNAME, luaopen_package },
        { LUA_TABLIBNAME, luaopen_table },
        { LUA_IOLIBNAME, luaopen_io },
        { LUA_OSLIBNAME, luaopen_os },
        { LUA_STRLIBNAME, luaopen_string },
        { LUA_MATHLIBNAME, luaopen_math },
        { LUA_DBLIBNAME, luaopen_debug },
        { NULL, NULL }
        };

		lua_State* ls = luaL_newstate();

        const luaL_Reg* lib = lualibs;
        for (; lib->func; lib++) {
            lua_pushcfunction(ls, lib->func);
            lua_pushstring(ls, lib->name);
            lua_call(ls, 1, 0);
        }

        int err = luaL_loadstring(ls, "print(\"Pute\")");

        if (err == 0) {
            err = lua_pcallk(ls, 0, 0, 0, 0, NULL);
        }

        if (err) {
            printf("lua err: %s", lua_tostring(ls, -1));
            lua_pop(ls, 1);
        }

        lua_close(ls);
	}
	catch (const std::exception& err) {
		Vin::HandleUnmanagedError(err, 1);
	}
}