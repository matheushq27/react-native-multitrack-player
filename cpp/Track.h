#pragma once

#include "../Superpowered/SuperpoweredAdvancedAudioPlayer.h"

class Track : public Superpowered::AdvancedAudioPlayer {
public:
    float leftVolume;
    float rightVolume;
    static int samplerate;

    static double getDurationMs(const char *path);

    explicit Track(const char *path);

    bool processLeft(float *buffer, bool mix, unsigned int numberOfFrames);
    bool processRight(float *buffer, bool mix, unsigned int numberOfFrames);
    bool processStereo(float *buffer, bool mix, unsigned int numberOfFrames);

    void setVolume(float left, float right);
};
