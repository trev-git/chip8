#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <bit>

#include "chip8.hpp"
#include "raylib.h"

#define TO_LSB16(b) (((b & 0xff) << 8) | ((b & 0xff00) >> 8))

Chip8::Chip8(const std::string &filename)
{
    /* Sprite area, bitmap for 0-F sprites */
    this->ram = {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
                    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
                    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
                    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
                    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
                    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
                    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
                    0xF0, 0x80, 0xF0, 0x80, 0x80, /* F*/
                };
    this->PC = 0x200;

    std::ifstream f(filename, std::ios::in | std::ios::binary);

    size_t fsize = std::filesystem::file_size(filename);

    if (fsize > 4096 - 0x200)
    {
        std::fprintf(stderr, "File too large!\n");
        exit(EXIT_FAILURE);
    }

    std::copy_n(std::istreambuf_iterator<char>(f), fsize, ram.begin()+0x200);

    f.close();
}

Chip8::~Chip8()
{

}

void Chip8::cycle()
{
    uint16_t instruction = TO_LSB16(this->ram[this->PC]);
    uint16_t instruction_type = instruction & 0xf000;

    if (instruction == 0x00e0)
    {
        ClearBackground(BLACK);
    }
    else if (instruction == 0x00ee)
    {
        this->PC = this->stack[this->SP--];
    }
    else if (instruction_type == 0x1000)
    {
        this->PC = instruction & 0x0fff;
    }
    else if (instruction_type == 0x2000)
    {
        this->stack[++this->SP] = this->PC;
        this->PC = instruction & 0x0fff;
    }
    else if (instruction_type == 0x3000)
    {
        if (this->regs[instruction & 0x0F00] == (instruction & 0x00ff))
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0x4000)
    {
        if (this->regs[instruction & 0x0f00] != (instruction & 0x00ff))
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0x5000)
    {
        if (this->regs[instruction & 0x0f00] == this->regs[instruction & 0x00f0])
        {
            this->PC += 2;
        }
    }

    this->PC += 2;
}