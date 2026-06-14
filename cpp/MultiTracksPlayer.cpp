#include "MultiTracksPlayer.h"
#include "../Superpowered/Superpowered.h"
#include "../Superpowered/SuperpoweredCPU.h"
#include <stdexcept>
#include <string.h>

MultiTracksPlayer::MultiTracksPlayer() {
    _stereoMixer = new Superpowered::StereoMixer();
    // input 0 (left-panned tracks) → only left output
    // input 1 (right-panned tracks) → only right output
    // input 2 (stereo tracks) → both channels
    _stereoMixer->inputGain[1] = 0.0f; // input 0 right = silent
    _stereoMixer->inputGain[2] = 0.0f; // input 1 left = silent
}

MultiTracksPlayer::~MultiTracksPlayer() {
    delete _stereoMixer;
    for (auto it = _tracks.rbegin(); it != _tracks.rend(); ++it) {
        delete *it;
    }
    _tracks.clear();
}

void MultiTracksPlayer::initSDK(const char *licenseKey, const char *tempFolderPath) {
    Superpowered::Initialize(licenseKey);
    if (tempFolderPath) {
        Superpowered::AdvancedAudioPlayer::setTempFolder(tempFolderPath);
    }
}

void MultiTracksPlayer::setTracks(const char **paths, int amount) {
    int i = 0;
    // Reuse existing track slots
    for (; i < amount && i < (int)_tracks.size(); i++) {
        _tracks[i]->open(paths[i]);
    }
    // Add new tracks
    for (; i < amount; i++) {
        _tracks.push_back(new Track(paths[i]));
    }
    // Remove extra tracks
    while ((int)_tracks.size() > amount) {
        delete _tracks.back();
        _tracks.pop_back();
    }
}

bool MultiTracksPlayer::process(float *output, unsigned int numberOfFrames, unsigned int samplerate) {
    for (auto track : _tracks) {
        track->outputSamplerate = samplerate;
        track->syncMode = Superpowered::AdvancedAudioPlayer::SyncMode_TempoAndBeat;
    }

    // Three mixing buses: [0]=left-panned, [1]=right-panned, [2]=stereo
    // Buffer size per Superpowered's recommendation: numberOfFrames * 8 + 64 floats
    unsigned int bufSize = numberOfFrames * 8 + 64;
    float *buf0 = new float[bufSize];
    float *buf1 = new float[bufSize];
    float *buf2 = new float[bufSize];
    memset(buf0, 0, bufSize * sizeof(float));
    memset(buf1, 0, bufSize * sizeof(float));
    memset(buf2, 0, bufSize * sizeof(float));

    bool silence[3] = {true, true, true};

    for (auto track : _tracks) {
        bool hasLeft = track->leftVolume > 0.0f;
        bool hasRight = track->rightVolume > 0.0f;

        if (hasLeft && hasRight) {
            if (track->processStereo(buf2, !silence[2], numberOfFrames))
                silence[2] = false;
        } else if (hasRight) {
            if (track->processRight(buf1, !silence[1], numberOfFrames))
                silence[1] = false;
        } else if (hasLeft) {
            if (track->processLeft(buf0, !silence[0], numberOfFrames))
                silence[0] = false;
        }
    }

    _stereoMixer->process(
        silence[0] ? nullptr : buf0,
        silence[1] ? nullptr : buf1,
        silence[2] ? nullptr : buf2,
        nullptr,
        output,
        numberOfFrames
    );

    bool hasSomething = !(silence[0] && silence[1] && silence[2]);

    delete[] buf0;
    delete[] buf1;
    delete[] buf2;

    return hasSomething;
}

void MultiTracksPlayer::play() {
    for (auto track : _tracks) {
        if (track == _tracks.front()) track->play();
        else track->playSynchronized();
    }
    Superpowered::CPU::setSustainedPerformanceMode(true);
}

void MultiTracksPlayer::pause() {
    for (auto track : _tracks) track->pause();
    Superpowered::CPU::setSustainedPerformanceMode(false);
}

void MultiTracksPlayer::setPosition(double ms) {
    for (auto track : _tracks)
        track->setPosition(ms, false, true, false, true);
}

double MultiTracksPlayer::getDisplayPositionMs() {
    if (_tracks.empty()) return 0.0;
    return _tracks.front()->getDisplayPositionMs();
}

void MultiTracksPlayer::setTrackVolume(int index, float left, float right) {
    if (index >= 0 && index < (int)_tracks.size())
        _tracks[index]->setVolume(left, right);
}

void MultiTracksPlayer::setVolume(float value) {
    _stereoMixer->outputGain[0] = value;
    _stereoMixer->outputGain[1] = value;
}

void MultiTracksPlayer::setRate(float rate) {
    for (auto track : _tracks) track->playbackRate = rate;
}

void MultiTracksPlayer::setPitchShiftCents(int cents) {
    for (auto track : _tracks) track->pitchShiftCents = cents;
}

float MultiTracksPlayer::getBufferedPercent() {
    if (_tracks.empty()) return 0.0f;
    float total = 0.0f;
    for (auto track : _tracks) total += track->getBufferedEndPercent();
    return (total / _tracks.size()) * 100.0f;
}

float MultiTracksPlayer::getTrackBufferedPercent(int index) {
    if (index < 0 || index >= (int)_tracks.size()) return 0.0f;
    return _tracks[index]->getBufferedEndPercent() * 100.0f;
}

double MultiTracksPlayer::getDurationMs(const char *path) {
    return Track::getDurationMs(path);
}
