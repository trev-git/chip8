/**
 * Copyright (c) 2025 trev-git
 * This program is published under the MIT license.
 * Refer to the LICENSE file for more information.
 */

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <raylib.h>

#include "chip8.hpp"
#include "keyboard.hpp"
#include "audio.hpp"

#define INSTRUCTIONS_PER_FRAME 1

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::fprintf(stderr, "Usage: %s <path_to_rom>\n", argv[0]);
        std::exit(EXIT_FAILURE);
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::fprintf(stderr, "[ERROR] Couldn't open file!\n");
        std::exit(EXIT_FAILURE);
    }

    size_t file_size = std::filesystem::file_size(argv[1]);
    if (file_size > 0x1000 - 0x200)
    {
        std::fprintf(stderr, "[ERROR] File too big!\n");
        std::exit(EXIT_FAILURE);
    }

    InitWindow(640, 320, "Chip8");
    SetTargetFPS(60);

    std::vector<uint8_t> program(file_size);
    std::copy_n(std::istreambuf_iterator<char>(file), file_size, program.begin());

    Chip8 ch8(program);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        update_input(ch8.keys);
        for (int i = 0; i < INSTRUCTIONS_PER_FRAME; i++)
        {
            ch8.tick();
        }
        auto display = ch8.get_display();
        for (int y = 0; y < 32; y++)
        {
            for (int x = 0; x < 64; x++)
            {
                if (display[y*64+x])
                {
                    DrawRectangle(x*10, y*10, 10, 10, WHITE);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
