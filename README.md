# vin-engine
A custom game engine made with passion. currently working on it.  
First made for Visual Novel, but i hope i'll be able to continue afterward for a more complete engine.  
  
Currently work in progress.  
  
## Building VIN 
To build VIN you will need CMAKE and PYTHON.  
Install both if not already installed.  
  
Clone the repository : ```git clone --recursive https://github.com/HITOA/vin-engine.git```   
   
Install jinj2 package : ```pip install jinj2```   
Generate with cmake : ```cmake -B"./build" .```   

## TODO  
-Custom Allocator  
-Custom STL  
-Refacto, some things are not modular enough (Most importante things is removing dependency between Rendering API and Windowing API)  
    Ideally, things are separated into module. (Math module, Rendering module, Windowing module, etc..)
-Render graph & Render queue obviously  
-ECS (Based of bevy ecs system)

## Source
A lot of useful source for developing a game engine stuff :

Game Programming Patterns (Robert Nystrom) : https://gameprogrammingpatterns.com/contents.html  
Learn Vulkan : https://vulkan-tutorial.com/  
Learn Opengl : https://learnopengl.com/  
Godot Engine Repos (Most readable source code for game engine probably) : https://github.com/godotengine/godot  
The Cherno (Especially his game engine series) : https://www.youtube.com/c/TheChernoProject  
Chili Tomato Noodle : https://www.youtube.com/c/ChiliTomatoNoodle    
MonoGame Repos : https://github.com/MonoGame/MonoGame/tree/develop/MonoGame.Framework    
GLM Repos : https://github.com/g-truc/glm    
Cell (JoeyDeVries) : https://github.com/JoeyDeVries/Cell  
Bevy : https://github.com/bevyengine/bevy   
An Entity component system with data locality : https://indiegamedev.net/2020/05/19/an-entity-component-system-with-data-locality-in-cpp/  
Building an ECS : https://ajmmertens.medium.com/building-an-ecs-1-where-are-my-entities-and-components-63d07c7da742  
ES wiki : http://entity-systems.wikidot.com/  