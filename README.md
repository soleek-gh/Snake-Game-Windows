# Snake Game
Simple snake terminal game made in c++ for windows


## Requirements

- CMake version 4.3.3 or newer
- One of:
  - Visual Studio 2019+ (or Build Tools) with the "Desktop development with C++" workload
  - MinGW-w64 (e.g. via MSYS2, installing the `mingw-w64-ucrt-x86_64-gcc` package)
 
## Building

```bash
git clone https://github.com/soleek-gh/Snake-Game-Windows.git
cd snake-game-windows
cmake -S . -B build
cmake --build build --config Release
```
Make sure to launch the game in a new terminal to avoid graphical issues

## Controls

Use the **arrow keys** to move the snake:
 
- ↑ Up
- ↓ Down
- ← Left
- → Right
