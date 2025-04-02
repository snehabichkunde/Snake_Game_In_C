# Snake Game with Apple

## Overview
A simple Snake game in C using SDL2. The snake moves, eats apples to grow, and the game ends on collision.

## Features
- Classic snake movement
- Apple spawning & consumption
- Collision detection

## Technologies Used
- C
- SDL2

## Installation & Compilation
### Prerequisites
**Linux:**
```sh
sudo apt-get install libsdl2-dev
```
**macOS:**
```sh
brew install sdl2
```
**Windows:** Install SDL2 from [SDL's website](https://www.libsdl.org/).

### Compilation
```sh
gcc snake.c -o snake $(sdl2-config --cflags --libs)
```

### Running the Game
```sh
./snake
```

## How to Play
- Arrow keys to control direction.
- Eat apples to grow.
- Avoid collisions to survive.

## Future Enhancements
- Score system
- Difficulty levels
- Improved graphics & sounds

## Author
Sneha Bichkunde

