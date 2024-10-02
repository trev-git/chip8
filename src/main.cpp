#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "raylib.h"
#include "chip8.hpp"

#define WIDTH  64 * 16
#define HEIGHT 32 * 16

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::printf("Usage: %s <path to rom>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    InitWindow(WIDTH, HEIGHT, "CHIP-8");
    Chip8 chip(argv[1]);
    chip.cycle();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RED);
        EndDrawing();
    }

    CloseWindow();
}
