#import "MultiTrackPlayerModule.h"
#import <React/RCTBridgeModule.h>
#import <ReactCommon/RCTTurboModule.h>
#import <AVFoundation/AVFoundation.h>

// C++ includes — must come before the Objective-C++ class
#include "../cpp/MultiTracksPlayer.h"
#include "../Superpowered/OpenSource/SuperpoweredIOSAudioIO.h"

// iOS audio processing callback (C linkage)
static bool audioProcessingCallback(
    void *clientdata,
    float *inputBuffer,
    float *outputBuffer,
    unsigned int numberOfFrames,
    unsigned int samplerate,
    uint64_t /* hostTime */
) {
    auto *player = reinterpret_cast<MultiTracksPlayer *>(clientdata);
    return player->process(outputBuffer, numberOfFrames, samplerate);
}

@interface MultiTrackPlayerModule () <SuperpoweredIOSAudioIODelegate>
@end

@implementation MultiTrackPlayerModule {
    MultiTracksPlayer *_player;
    SuperpoweredIOSAudioIO *_audioIO;
}

RCT_EXPORT_MODULE(NativeMultiTrackPlayer)

- (void)dealloc {
    [_audioIO stop];
    delete _player;
}

// MARK: - SuperpoweredIOSAudioIODelegate (required protocol)
- (void)interruptionStarted {}
- (void)interruptionEnded {}

// MARK: - TurboModule methods

RCT_EXPORT_METHOD(initPlayer:(NSString *)licenseKey tempFolderPath:(NSString *)tempFolderPath) {
    MultiTracksPlayer::initSDK([licenseKey UTF8String], [tempFolderPath UTF8String]);

    delete _player;
    _player = new MultiTracksPlayer();

    [_audioIO stop];
    _audioIO = [[SuperpoweredIOSAudioIO alloc]
        initWithDelegate:self
        preferredBufferSize:12
        preferredSamplerate:48000
        audioSessionCategory:AVAudioSessionCategoryPlayback
        channels:2
        audioProcessingCallback:audioProcessingCallback
        clientdata:_player];
    [_audioIO start];
}

RCT_EXPORT_METHOD(setTracks:(NSArray<NSString *> *)paths) {
    if (!_player) return;
    int count = (int)paths.count;
    const char **cPaths = new const char *[count];
    for (int i = 0; i < count; i++) {
        cPaths[i] = [paths[i] UTF8String];
    }
    _player->setTracks(cPaths, count);
    delete[] cPaths;
}

RCT_EXPORT_METHOD(play) {
    if (_player) _player->play();
}

RCT_EXPORT_METHOD(pause) {
    if (_player) _player->pause();
}

RCT_EXPORT_METHOD(setPosition:(double)ms) {
    if (_player) _player->setPosition(ms);
}

RCT_EXPORT_METHOD(getPositionMs:(RCTPromiseResolveBlock)resolve reject:(RCTPromiseRejectBlock)reject) {
    resolve(@(_player ? _player->getDisplayPositionMs() : 0.0));
}

RCT_EXPORT_METHOD(setTrackVolume:(double)index left:(double)left right:(double)right) {
    if (_player) _player->setTrackVolume((int)index, (float)left, (float)right);
}

RCT_EXPORT_METHOD(setVolume:(double)value) {
    if (_player) _player->setVolume((float)value);
}

RCT_EXPORT_METHOD(setRate:(double)rate) {
    if (_player) _player->setRate((float)rate);
}

RCT_EXPORT_METHOD(setPitchShiftCents:(double)cents) {
    if (_player) _player->setPitchShiftCents((int)cents);
}

RCT_EXPORT_METHOD(getBufferedPercent:(RCTPromiseResolveBlock)resolve reject:(RCTPromiseRejectBlock)reject) {
    resolve(@(_player ? _player->getBufferedPercent() : 0.0f));
}

RCT_EXPORT_METHOD(getTrackBufferedPercent:(double)index resolve:(RCTPromiseResolveBlock)resolve reject:(RCTPromiseRejectBlock)reject) {
    resolve(@(_player ? _player->getTrackBufferedPercent((int)index) : 0.0f));
}

RCT_EXPORT_METHOD(getDurationMs:(NSString *)path resolve:(RCTPromiseResolveBlock)resolve reject:(RCTPromiseRejectBlock)reject) {
    resolve(@(MultiTracksPlayer::getDurationMs([path UTF8String])));
}

@end
