# chip8++
CHIP-8 emulator in C++

---
Requirements:

  SDL2 - https://www.libsdl.org/

---
Building:

  Windows: Use MS Visual Studio
  
  Linux: from the root of the repo
    
    mkdir bin
    g++ -lSDL2 ./chip8++/CPU.cpp ./chip8++/Memory.cpp ./chip8++/Display.cpp ./chip8++/chip8++.cpp -o ./bin/chip8++

---

Usage:

`chip8++ [program]` where program is a chip-8 binary
