# Dreams Eater
Final project for the Game programming course at IT University of Copenhagen

# Introduction
Dreams Eater is a Roguelike - RPG shooter 2d game. The protagonist is a
wraith, whose goal is to explore a dungeon of rooms and fight all the enemies
that have come to haunt his dreams. The game is based on randomly generated
levels of increasing complexity. The player gets access to the following level
when he’s able to kill the boss of the level. Rooms may contain powerups,
special badges that can be collected by the player to enhance his abilities.

The game leverages on Box2d library to handle physics.\
Multiple types of keyboard inputs are supported and can be chosen through the starting menu,
as well as the starting difficulty.\
The game is using Simple Render Engine as game engine.

> See a more detailed report here -> [Report](/Report.pdf)

# Game

## Main menu
<img src="https://user-images.githubusercontent.com/79710064/221548720-d691ee3f-f930-4fc5-830f-b0f4f5fe2067.gif" alt="Dreams Eater Main Menu" width="600" /> 

## Gameplay
![DreamsEaterGameplay](https://user-images.githubusercontent.com/79710064/221548062-231463eb-bee0-486f-8534-bf40aa0dcb22.gif)


# How to build
- Follow the guide to download Simple Render Engine with all the dependencies at [SimpleRenderEngineProject](https://github.com/mortennobel/SimpleRenderEngineProject)
- Place this directory as subdirectory of the Simple Render Engine Project directory
- Append to the file SimpleRenderEngineProject/`CMakeLists.txt` the line 

  ```
  add_subdirectory(Dreams-Eater)
  ```
- Open the SimpleRenderEngineProject with Visual Studio or your desired IDE
- Build it

# The team
- Alessandro Baribero
- Giovanni Giacometti
- Lucas Schütt Nielsen
