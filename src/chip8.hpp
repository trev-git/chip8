#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <array>
#include <cstdint>
#include <string>

class Chip8
{
public:
    Chip8(const std::string &filename);
    void cycle();
    ~Chip8();
private:
    std::array<uint8_t, 16> regs;
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint8_t DT;
    uint8_t ST;
    std::array<uint16_t, 16> stack;
    std::array<uint8_t, 4096> ram;
};

#endif // CHIP8_HPP
