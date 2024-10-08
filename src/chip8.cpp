#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

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
    this->SP = 0;
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
    int reg1 = (instruction & 0x0f00) >> 8;
    int reg2 = (instruction & 0x00f0) >> 8;
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
        if (this->regs[reg1] == (instruction & 0xff))
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0x4000)
    {
        if (this->regs[reg1] != (instruction & 0x00ff))
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0x5000)
    {
        if (this->regs[reg1] == this->regs[reg2])
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0x6000)
    {
        this->regs[reg1] = instruction & 0xff;
    }
    else if (instruction_type == 0x7000)
    {
        this->regs[reg1] += instruction & 0xff;
    }
    else if (instruction_type == 0x8000)
    {
        switch (instruction & 0xf)
        {
        case 0:
        {
            this->regs[reg1] = this->regs[reg2];
        }
        case 1:
        {
            this->regs[reg1] |= this->regs[reg2];
        }
        case 2:
        {
            this->regs[reg1] &= this->regs[reg2];
        }
        case 3:
        {
            this->regs[reg1] ^= this->regs[reg2];
        }
        case 4:
        {
            this->regs[reg1] += this->regs[reg2];
            this->regs[15] = this->regs[reg1] < this->regs[reg2];
        }
        case 5:
        {
            this->regs[15] = this->regs[reg1] > this->regs[reg2];
            this->regs[reg1] -= this->regs[reg2];
        }
        case 6:
        {
            this->regs[15] = this->regs[reg1] & 0x1;
            this->regs[reg1] >>= 1;
        }
        case 7:
        {
            this->regs[15] = this->regs[reg2] > this->regs[reg1];
            this->regs[reg1] = this->regs[reg2] - this->regs[reg1];
        }
        case 0xe:
        {
            this->regs[15] = this->regs[reg1] & 0x8000 >> 15;
            this->regs[reg1] <<= 1;
        }
        }
    }
    else if (instruction_type == 0x9000)
    {
        if (this->regs[reg1] != this->regs[reg2])
        {
            this->PC += 2;
        }
    }
    else if (instruction_type == 0xa000)
    {
        this->I = instruction & 0x0fff;
    }
    else if (instruction_type == 0xb000)
    {
        this->PC = instruction & 0x0fff + this->regs[0];
    }
    else if (instruction_type == 0xc000)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<uint8_t> dist(0, 255);
        uint8_t rnd_byte = dist(mt);
        this->regs[reg1] = rnd_byte & (instruction & 0xff);
    }
    else if (instruction_type == 0xd000)
    {

    }
    else if (instruction_type == 0xe000)
    {

    }
    else if (instruction_type == 0xf000)
    {
        if ((instruction & 0xff) == 0x07)
        {
            this->regs[reg1] = this->DT;
        }
        else if ((instruction & 0xff) == 0x0a)
        {

        }
        else if ((instruction & 0xff) == 0x15)
        {
            this->DT = this->regs[reg1];
        }
        else if ((instruction & 0xff) == 0x18)
        {
            this->ST = this->regs[reg1];
        }
        else if ((instruction & 0xff) == 0x1e)
        {
            this->I += this->regs[reg1];
        }
        else if ((instruction & 0xff) == 0x29)
        {
            this->I = (this->regs[reg1] & 0xf) * 5;
        }
        else if ((instruction & 0xff) == 0x33)
        {
            this->ram[this->I] = (this->regs[reg1] / 100) % 10;
            this->ram[this->I+1] = (this->regs[reg1] / 10) % 10;
            this->ram[this->I+2] = this->regs[reg1] % 10;
        }
        else if ((instruction & 0xff) == 0x55)
        {
            uint16_t old_I = this->I;
            for (int i = 0; i <= reg1; i++, I++)
            {
                this->ram[I] = this->regs[i];
            }
            this->I = old_I;
        }
        else if ((instruction & 0xff) == 0x65)
        {
            uint16_t old_I = this->I;
            for (int i = 0; i <= reg1; i++, I++)
            {
                this->regs[i] = this->ram[I];
            }
            this->I = old_I;
        }
    }

    this->PC += 2;
    this->DT--;
    this->ST--;
}