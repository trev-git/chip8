#include <array>
#include <raylib.h>

#include "keyboard.hpp"

void update_input(std::array<bool, 16> &keys)
{
    keys[0] = IsKeyDown(KEY_X);
    keys[1] = IsKeyDown(KEY_ONE);
    keys[2] = IsKeyDown(KEY_TWO);
    keys[3] = IsKeyDown(KEY_THREE);
    keys[4] = IsKeyDown(KEY_Q);
    keys[5] = IsKeyDown(KEY_W);
    keys[6] = IsKeyDown(KEY_E);
    keys[7] = IsKeyDown(KEY_A);
    keys[8] = IsKeyDown(KEY_S);
    keys[9] = IsKeyDown(KEY_D);
    keys[10] = IsKeyDown(KEY_Z);
    keys[11] = IsKeyDown(KEY_C);
    keys[12] = IsKeyDown(KEY_FOUR);
    keys[13] = IsKeyDown(KEY_R);
    keys[14] = IsKeyDown(KEY_F);
    keys[15] = IsKeyDown(KEY_V);
}
