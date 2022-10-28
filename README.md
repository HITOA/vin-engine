# vin-engine
A custom game engine made with passion. currently working on it.  
First made for visual novel in mind. now it's just a 3D engine.
  
Currently work in progress.  
  
## Building VIN 
To build VIN you will need CMAKE and PYTHON.  
Install both if not already installed.  
  
Clone the repository : ```git clone --recursive https://github.com/HITOA/vin-engine.git```   
   
Install jinja2 package : ```pip install jinja2```   
Generate with cmake : ```cmake -B"./build" .```    
  
Last tested with (gcc, msvc, clang) on windows and linux : 10/24/2022  
  
## Profiling  
Optick is used to profile the engine : https://github.com/bombomby/optick  
There is an exe version in /tool.  
  
## Screenshot
The sponza scene rendered in the engine : (10/22/2022)  
![Sponza scene](https://i.imgur.com/FLY2bc9.jpeg)  
![Modern sponza scene](https://i.imgur.com/9y9cDtY.jpeg)  
  
## TODO 
- Seperate generale rendering function (shadow map etc) to a helper class or something like that  
- Make creating rendering pipeline much easier and high level  

- Loading GLTF/GLB (2.0) model. (Almost complete)
- PostProcessing Chain system (Simple way to do postprocessing on a rendetarget, you pass the render target to a post process chain, then it end up with all the effect. you would add post process effect like chain.Add(postprocessmat))  
- Implementing MIMALLOC for std & new/delete operator overload  
  
- Transform hierarchy, Need a lot of optimization  
    
- Proper lighting (Main light with shadow map, Additional light with Eventual shadow map)  
- Cubemap  
- GPU Instancing  
- Cascade shadow map (Entire frustum)  
- Frustum culling (Compute shader)  
- Hot reloading (shader)  
- UI/Font (https://github.com/freetype/freetype / https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf)   
   
## Things to considere  
- Better soft shadow algorithm (Better PCF)
- Vulkan implementation
- Optimizing more !! (always good)
- VR/XR/AR rendering pipeline ?

## Source
A lot of useful source for developing game engine related stuff :

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
Quake PAK format specification : https://quakewiki.org/wiki/.pak  
Order your graphics draw calls around : http://realtimecollisiondetection.net/blog/?p=86  
Optimizing 4x4 matrix multiplication : https://nfrechette.github.io/2017/04/13/modern_simd_matrix_multiplication/  
PCSS shadow : https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf  
Optimized Transform Hierarchy : https://alexsabourindev.wordpress.com/2019/04/14/creating-an-optimized-transform-hierarchy/  
