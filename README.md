# Pixel-Arena
COMP313 Group Project  - Pixel arena a short 2d top down "Boss Rush" game developed in UE4

## Architecture

The game consists of two levels, Main and Menu. Main is the main game level and menu is the main menu screen.
The game loop is as follows:
  - Menu -> Class Select -> Main Game Level
  - Main Game Level -> Lose -> Menu
  - Main Game Level -> Win -> Menu
  
  All blueprints are in the PixelArena/Blueprints directory. The code structure is set up as base classes handle a blueprints state and supply helper functions/properties that any inheriting blueprints would require. Then using blueprint implementable events the inheriting blueprint can define the state logic specific to its self. This is to avoid repeating code/nodes between blueprints and makes adding different variations easier.
  
  All visual assets in the game were created by the design team.
  
  ## Installation
  Download the latest release from [Releases](https://github.com/Blukzen/Pixel-Arena/releases).
  Unzip the contents.
  Execute PixelArena.exe
  
  **WARNING** There is a current bug where sometimes on class selection the game crashes.
  Gameplay instructions are provided in game.
