/**
 * Copyright (c) 2025 trev-git
 * This program is published under the MIT license.
 * Refer to the LICENSE file for more information.
 */

#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <array>
#include <cstdint>
#include <vector>

#define RAM_SIZE 0x1000

class Chip8
{
public:
    Chip8(std::vector<uint8_t> program);
    std::array<bool, 64*32> get_display();
    bool do_sound();
    void tick();

    std::array<bool, 16> keys = { 0 };

private:
    // registers
    std::array<uint8_t, 16> regs; // general purpose regs
    uint16_t I; // memory addr reg
    uint8_t dt; // delay timer
    uint8_t st; // sound timer
    uint16_t pc; // program counter
    uint8_t sp; // stack pointer

    // memory
    std::array<uint16_t, 16> stack;
    std::array<uint8_t, RAM_SIZE> ram;
    std::array<bool, 64*32> display;

    // instructions
    inline void cls();
    inline void ret();
    inline void sys(uint16_t addr);
    inline void jp(uint16_t addr);
    inline void call(uint16_t addr);
    inline void se_byte(uint8_t vx, uint8_t byte);
    inline void sne_byte(uint8_t vx, uint8_t byte);
    inline void se(uint8_t vx, uint8_t vy);
    inline void ld_byte(uint8_t vx, uint8_t byte);
    inline void add_byte(uint8_t vx, uint8_t byte);
    inline void ld(uint8_t vx, uint8_t vy);
    inline void or_(uint8_t vx, uint8_t vy);
    inline void and_(uint8_t vx, uint8_t vy);
    inline void xor_(uint8_t vx, uint8_t vy);
    inline void add(uint8_t vx, uint8_t vy);
    inline void sub(uint8_t vx, uint8_t vy);
    inline void shr(uint8_t vx, uint8_t vy);
    inline void subn(uint8_t vx, uint8_t vy);
    inline void shl(uint8_t vx, uint8_t vy);
    inline void sne(uint8_t vx, uint8_t vy);
    inline void ldi(uint16_t addr);
    inline void jpv0(uint16_t addr);
    inline void rnd(uint8_t vx, uint8_t byte);
    inline void drw(uint8_t vx, uint8_t vy, uint8_t nibble);
    inline void skp(uint8_t vx);
    inline void sknp(uint8_t vx);
    inline void ldregdt(uint8_t vx);
    inline void ldk(uint8_t vx);
    inline void lddt(uint8_t vx);
    inline void ldst(uint8_t vx);
    inline void addi(uint8_t vx);
    inline void ldf(uint8_t vx);
    inline void ldb(uint8_t vx);
    inline void ld_write_regs(uint8_t vx);
    inline void ld_read_regs(uint8_t vx);
};

#endif // CHIP8_HPP