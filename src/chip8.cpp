/**
 * Copyright (c) 2025 trev-git
 * This program is published under the MIT license.
 * Refer to the LICENSE file for more information.
 */

#include <algorithm>
#include <cstdio>
#include <ios>
#include <iostream>
#include <vector>
#include <cstdint>
#include <random>
#include <raylib.h>

#include "chip8.hpp"

Chip8::Chip8(std::vector<uint8_t> program)
{
    ram = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    this->pc = 0x200;
    this->sp = 0;
    this->stack = { 0 };
    this->display = { 0 };

    std::copy_n(program.begin(), program.size(), this->ram.begin() + 0x200);
}

std::array<bool, 64*32> Chip8::get_display()
{
    return this->display;
}

bool Chip8::do_sound()
{
    return st;
}

void Chip8::tick()
{
    uint16_t instruction = ((uint16_t)this->ram[this->pc] << 8) | (this->ram[this->pc+1]);
    uint16_t instruction_type = (instruction & 0xf000) >> 12;
    uint16_t addr = instruction & 0x0fff;
    uint8_t vx = (instruction & 0x0f00) >> 8;
    uint8_t vy = (instruction & 0x00f0) >> 4;
    uint8_t byte = instruction & 0x00ff;
    switch (instruction_type)
    {
    case 0x0:
        switch (instruction)
        {
        case 0x00e0:
            cls();
            this->pc += 2;
            break;
        case 0x00ee:
            ret();
            this->pc += 2;
            break;
        }
        break;
    case 0x1:
        jp(addr);
        break;
    case 0x2:
        call(addr);
        break;
    case 0x3:
        se_byte(vx, byte);
        this->pc += 2;
        break;
    case 0x4:
        sne_byte(vx, byte);
        this->pc += 2;
        break;
    case 0x5:
        se(vx, vy);
        this->pc += 2;
        break;
    case 0x6:
        ld_byte(vx, byte);
        this->pc += 2;
        break;
    case 0x7:
        add_byte(vx, byte);
        this->pc += 2;
        break;
    case 0x8:
        switch (instruction & 0xf)
        {
        case 0:
            ld(vx, vy);
            this->pc += 2;
            break;
        case 1:
            or_(vx, vy);
            this->pc += 2;
            break;
        case 2:
            and_(vx, vy);
            this->pc += 2;
            break;
        case 3:
            xor_(vx, vy);
            this->pc += 2;
            break;
        case 4:
            add(vx, vy);
            this->pc += 2;
            break;
        case 5:
            sub(vx, vy);
            this->pc += 2;
            break;
        case 6:
            shr(vx, vy);
            this->pc += 2;
            break;
        case 7:
            subn(vx, vy);
            this->pc += 2;
            break;
        case 0xe:
            shl(vx, vy);
            this->pc += 2;
        }
        break;
    case 9:
        sne(vx, vy);
        this->pc += 2;
        break;
    case 0xa:
        ldi(addr);
        this->pc += 2;
        break;
    case 0xb:
        jpv0(addr);
        break;
    case 0xc:
        rnd(vx, byte);
        this->pc += 2;
        break;
    case 0xd:
        drw(vx, vy, instruction & 0xf);
        this->pc += 2;
        break;
    case 0xe:
        switch (byte)
        {
        case 0x9e:
            skp(vx);
            pc += 2;
            break;
        case 0xa1:
            sknp(vx);
            pc += 2;
            break;
        }
        break;
    case 0xf:
        switch (byte)
        {
        case 0x07:
            ldregdt(vx);
            this->pc += 2;
            break;
        case 0x0a:
            ldk(vx);
            this->pc += 2;
            break;
        case 0x15:
            lddt(vx);
            this->pc += 2;
            break;
        case 0x18:
            ldf(vx);
            this->pc += 2;
            break;
        case 0x1e:
            addi(vx);
            this->pc += 2;
            break;
        case 0x33:
            ldb(vx);
            this->pc += 2;
            break;
        case 0x55:
            ld_write_regs(vx);
            this->pc += 2;
            break;
        case 0x65:
            ld_read_regs(vx);
            this->pc += 2;
            break;
        }
        break;
    default:
        std::cerr << "[WARNING] Unknown instruction: " << std::hex << instruction << '\n';
    }
    if (this->dt != 0)
        this->dt--;
    if (this->st != 0)
        this->st--;
}

void Chip8::cls()
{
    this->display = { 0 };
}

void Chip8::ret()
{
    this->pc = stack[this->sp--];
}

void Chip8::sys(uint16_t addr)
{
    return;
}

void Chip8::jp(uint16_t addr)
{
    this->pc = addr & 0xfff;
}

void Chip8::call(uint16_t addr)
{
    this->stack[++this->sp] = this->pc;
    this->pc = addr & 0xfff;
}

void Chip8::se_byte(uint8_t vx, uint8_t byte)
{
    if (this->regs[vx] == byte)
        this->pc += 2;
}

void Chip8::sne_byte(uint8_t vx, uint8_t byte)
{
    if (this->regs[vx] != byte)
        this->pc += 2;
}

void Chip8::se(uint8_t vx, uint8_t vy)
{
    if (this->regs[vx] == this->regs[vy]) 
        this->pc += 2;
}

void Chip8::ld_byte(uint8_t vx, uint8_t byte)
{
    this->regs[vx] = byte;
}

void Chip8::add_byte(uint8_t vx, uint8_t byte)
{
    this->regs[vx] += byte;
}

void Chip8::ld(uint8_t vx, uint8_t vy)
{
    this->regs[vx] = this->regs[vy];
}

void Chip8::or_(uint8_t vx, uint8_t vy)
{
    this->regs[vx] |= this->regs[vy];
    this->regs[0xf] = 0;
}

void Chip8::and_(uint8_t vx, uint8_t vy)
{
    this->regs[vx] &= this->regs[vy];
    this->regs[0xf] = 0;
}

void Chip8::xor_(uint8_t vx, uint8_t vy)
{
    this->regs[vx] ^= this->regs[vy];
    this->regs[0xf] = 0;
}

void Chip8::add(uint8_t vx, uint8_t vy)
{
    bool flag = (uint16_t)this->regs[vx] + (uint16_t)this->regs[vy] > 0xff;
    this->regs[vx] += this->regs[vy];
    this->regs[0xf] = flag;
}

void Chip8::sub(uint8_t vx, uint8_t vy)
{
    bool flag = this->regs[vx] >= this->regs[vy];
    this->regs[vx] -= this->regs[vy];
    this->regs[0xf] = flag;
}

void Chip8::shr(uint8_t vx, uint8_t vy)
{
    bool flag = this->regs[vx] & 0x1;
    this->regs[vx] >>= 1;
    this->regs[0xf] = flag;
}

void Chip8::subn(uint8_t vx, uint8_t vy)
{
    bool flag = this->regs[vy] >= this->regs[vx];
    this->regs[vx] = this->regs[vy] - this->regs[vx];
    this->regs[0xf] = flag;
}

void Chip8::shl(uint8_t vx, uint8_t vy)
{
    bool flag = this->regs[vx] & 0x80;
    this->regs[vx] <<= 1;
    this->regs[0xf] = flag;
}

void Chip8::sne(uint8_t vx, uint8_t vy)
{
    if (this->regs[vx] != this->regs[vy])
        this->pc += 2;
}

void Chip8::ldi(uint16_t addr)
{
    this->I = addr;
}

void Chip8::jpv0(uint16_t addr)
{
    this->pc = this->regs[0] + addr;
}

void Chip8::rnd(uint8_t vx, uint8_t byte)
{
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0,255);
    this->regs[vx] = dist(rd) & byte;
}

void Chip8::drw(uint8_t vx, uint8_t vy, uint8_t nibble)
{
    this->regs[0xf] = 0;
    for (int i = 0; i < nibble; i++)
    {
        uint8_t sprite_row = this->ram[this->I + i];
        int row = (this->regs[vy] + i) % 32;
        for(int n = 0; n <= 7; n++)
        {
            bool bit = (sprite_row & 0x80) >> 7;
            int col = (this->regs[vx] + n) % 64;
            int offset = row * 64 + col;
            if (this->display[offset] && bit)
                this->regs[0xf] = 1;
            this->display[offset] ^= bit;
            sprite_row <<= 1;
        }
    }
}

void Chip8::skp(uint8_t vx)
{
    vx &= 0xf;

    if (this->keys[this->regs[vx]])
    {
        pc += 2;
        return;
    }
}

void Chip8::sknp(uint8_t vx)
{
    vx &= 0xf;

    if (!this->keys[this->regs[vx]])
    {
        pc += 2;
    }
}

void Chip8::ldregdt(uint8_t vx)
{
    this->regs[vx] = this->dt;
}

void Chip8::ldk(uint8_t vx)
{
    if (!this->keys[this->regs[vx]])
        pc -= 2;
    else
        this->regs[vx] = vx;
}

void Chip8::lddt(uint8_t vx)
{
    this->dt = this->regs[vx];
}

void Chip8::ldst(uint8_t vx)
{
    this->st = this->regs[vx];
}

void Chip8::addi(uint8_t vx)
{
    this->I += this->regs[vx];
}

void Chip8::ldf(uint8_t vx)
{
    this->I = (this->regs[vx] & 0xf) * 5;
}

void Chip8::ldb(uint8_t vx)
{
    this->ram[this->I] = (this->regs[vx] / 100);
    this->ram[this->I+1] = (this->regs[vx] / 10 % 10);
    this->ram[this->I+2] = (this->regs[vx] % 10);
}

void Chip8::ld_write_regs(uint8_t vx)
{
    std::copy_n(this->regs.begin(), vx+1, this->ram.begin() + I);
    I = I + vx + 1;
}

void Chip8::ld_read_regs(uint8_t vx)
{
    std::copy_n(this->ram.begin() + I, vx+1, this->regs.begin());
    I += I + vx + 1;
}
