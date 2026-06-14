#pragma once

#include <vector>
#include "Track.h"
#include "../Superpowered/SuperpoweredMixer.h"

// MultiTracksPlayer handles synchronized multi-track audio mixing.
// Platform-specific audio I/O is created by the native bridges (JNI on Android,
// Objective-C++ on iOS), which call process() from the audio callback.
class MultiTracksPlayer {
public:
    MultiTracksPlayer();
    ~MultiTracksPlayer();

    // Initialize Superpowered SDK. Call once before any other method.
    static void initSDK(const char *licenseKey, const char *tempFolderPath);

    // Load tracks from file paths. Replaces any existing tracks.
    void setTracks(const char **paths, int amount);

    // Process audio into a 32-bit float interleaved stereo buffer.
    // Returns false if output is silence (all tracks paused or muted).
    bool process(float *output, unsigned int numberOfFrames, unsigned int samplerate);

    void play();
    void pause();

    // Seek all tracks to position in milliseconds.
    void setPosition(double ms);

    double getDisplayPositionMs();

    // Per-track volume: left/right 0.0-1.0 (use 0/0 to mute, 1/0 for L-only panning, etc.)
    void setTrackVolume(int index, float left, float right);

    // Global output gain.
    void setVolume(float value);

    void setRate(float rate);
    void setPitchShiftCents(int cents);

    float getBufferedPercent();
    float getTrackBufferedPercent(int index);

    static double getDurationMs(const char *path);

private:
    std::vector<Track *> _tracks;
    Superpowered::StereoMixer *_stereoMixer;
};
