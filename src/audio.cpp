#include <cstdio>
#include <new>
#include <numbers>
#include <raylib.h>
#include <cmath>

#include "audio.hpp"

using std::numbers::pi;

static const unsigned int sample_rate = 48000;

static Sound beep_sound;

void generate_wave(unsigned int hz, unsigned int length)
{
    Wave sine = {
        .sampleRate = sample_rate,
        .sampleSize = 32,
        .channels = 1
    };
    unsigned int frames = length * (sample_rate / 1000);
    sine.data = new float[frames];
    sine.frameCount = frames;

    for (int i = 0; i < frames; i++)
    {
        ((float *)sine.data)[i] = std::sin((2*pi*hz) * ((float)i/sample_rate));
    }
    beep_sound = LoadSoundFromWave(sine);
    delete[] ((float *)sine.data);
}

void beep()
{
    if (!IsSoundPlaying(beep_sound))
        PlaySound(beep_sound);
}
