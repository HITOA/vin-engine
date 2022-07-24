require "main"

print("Lua is working!")

function anotherfunc()
    print("chien")
end

vin.init("opengl")
--vin.setwindowsize(1000, 1000)
vin.register("vin_update", update) --register update function
vin.register("vin_update", anotherfunc)
vin.run()

print(vin)
print(vin.event)
print(vin.vin_update)
print(#vin.event)
print(vin.event[0])
print(vin.event.vin_update)
print(#vin.event.vin_update)
print(vin.event.vin_update[0])
print(vin.event.vin_update[1])
print(package.searchers)
print(package.searchers[1])

vin.event.vin_update[1]();

print("End of init.lua");