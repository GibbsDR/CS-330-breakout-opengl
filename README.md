# CS-330-breakout-opengl
2D Breakout-style OpenGL game with physics-based collision, multi-ball mechanics, and interactive brick destruction.
# 2D Breakout Game (OpenGL / GLFW)

## Overview
This project is a 2D Breakout-style animation built using OpenGL and GLFW. The goal of the game is to destroy all bricks using bouncing balls while preventing them from falling below the screen. The project demonstrates real-time rendering, collision detection, and basic physics-based movement.

## Features
- Triangular brick layout for visual variety
- Multi-hit bricks with color changes based on durability
- Two independent balls with velocity-based movement
- Collision detection with bricks, walls, and paddle
- Paddle controlled via keyboard input (A/D keys)
- Life system (3 lives total)
- Game over and win states
- Reset and restart functionality (R key)

## Controls
- A / D → Move paddle left and right
- SPACE → Launch balls
- R → Restart game after win/lose

## Concepts Used
- Collision detection and resolution
- Velocity-based motion (basic physics simulation)
- State management (win/lose/lives system)
- Modular programming structure
- OpenGL rendering pipeline (GLFW)

## How to Run
1. Open the Visual Studio solution file (.sln)
2. Build the project (Debug or Release)
3. Run the executable

## Author
Created as part of CS330 Computational Graphics coursework.

## Notes
This project focuses on foundational 2D physics simulation and interactive animation techniques using OpenGL.
