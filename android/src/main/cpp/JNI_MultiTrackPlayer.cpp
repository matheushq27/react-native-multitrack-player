#include <jni.h>
#include <string>
// These headers are resolved via CMakeLists target_include_directories
#include "MultiTracksPlayer.h"
#include "SuperpoweredAndroidAudioIO.h"

// Audio I/O and player live as static singletons for the duration of the app.
static MultiTracksPlayer *player = nullptr;
static SuperpoweredAndroidAudioIO *audioIO = nullptr;

static bool audioCallback(
    void *clientdata,
    short int *audioBuffer,
    int numberOfFrames,
    int samplerate
) {
    auto *p = reinterpret_cast<MultiTracksPlayer *>(clientdata);

    // Allocate float buffer for mixing, then convert to 16-bit for Android output.
    auto *floatBuffer = new float[numberOfFrames * 2];
    bool hasAudio = p->process(floatBuffer, (unsigned int)numberOfFrames, (unsigned int)samplerate);

    if (hasAudio) {
        // Convert 32-bit float → 16-bit int
        for (int i = 0; i < numberOfFrames * 2; i++) {
            float s = floatBuffer[i];
            if (s > 1.0f) s = 1.0f;
            else if (s < -1.0f) s = -1.0f;
            audioBuffer[i] = (short)(s * 32767.0f);
        }
    }

    delete[] floatBuffer;
    return hasAudio;
}

extern "C" {

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeInit(
    JNIEnv *env,
    jobject /* thiz */,
    jstring licenseKey,
    jstring tempFolder,
    jint samplerate,
    jint bufferSize
) {
    const char *key = env->GetStringUTFChars(licenseKey, nullptr);
    const char *tmpPath = env->GetStringUTFChars(tempFolder, nullptr);

    MultiTracksPlayer::initSDK(key, tmpPath);

    env->ReleaseStringUTFChars(licenseKey, key);
    env->ReleaseStringUTFChars(tempFolder, tmpPath);

    Track::samplerate = samplerate;

    if (player) delete player;
    player = new MultiTracksPlayer();

    if (audioIO) delete audioIO;
    audioIO = new SuperpoweredAndroidAudioIO(
        samplerate, bufferSize,
        false, true,
        audioCallback, player,
        -1, -1
    );
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetTracks(
    JNIEnv *env,
    jobject /* thiz */,
    jobjectArray paths
) {
    if (!player) return;
    int count = env->GetArrayLength(paths);
    const char **cPaths = new const char *[count];
    jstring *jstrings = new jstring[count];

    for (int i = 0; i < count; i++) {
        jstrings[i] = (jstring)env->GetObjectArrayElement(paths, i);
        cPaths[i] = env->GetStringUTFChars(jstrings[i], nullptr);
    }

    player->setTracks(cPaths, count);

    for (int i = 0; i < count; i++) {
        env->ReleaseStringUTFChars(jstrings[i], cPaths[i]);
    }
    delete[] cPaths;
    delete[] jstrings;
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativePlay(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (player) player->play();
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativePause(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (player) player->pause();
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetPosition(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jdouble ms
) {
    if (player) player->setPosition(ms);
}

JNIEXPORT jdouble JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeGetPositionMs(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (!player) return 0.0;
    return player->getDisplayPositionMs();
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetTrackVolume(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jint index,
    jfloat left,
    jfloat right
) {
    if (player) player->setTrackVolume(index, left, right);
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetVolume(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jfloat value
) {
    if (player) player->setVolume(value);
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetRate(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jfloat rate
) {
    if (player) player->setRate(rate);
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeSetPitchShiftCents(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jint cents
) {
    if (player) player->setPitchShiftCents(cents);
}

JNIEXPORT jfloat JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeGetBufferedPercent(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (!player) return 0.0f;
    return player->getBufferedPercent();
}

JNIEXPORT jfloat JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeGetTrackBufferedPercent(
    JNIEnv * /* env */,
    jobject /* thiz */,
    jint index
) {
    if (!player) return 0.0f;
    return player->getTrackBufferedPercent(index);
}

JNIEXPORT jdouble JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeGetDurationMs(
    JNIEnv *env,
    jobject /* thiz */,
    jstring path
) {
    const char *cPath = env->GetStringUTFChars(path, nullptr);
    double duration = MultiTracksPlayer::getDurationMs(cPath);
    env->ReleaseStringUTFChars(path, cPath);
    return duration;
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeOnForeground(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (audioIO) audioIO->onForeground();
}

JNIEXPORT void JNICALL
Java_com_reactnativemultitrackplayer_MultiTrackPlayerModule_nativeOnBackground(
    JNIEnv * /* env */,
    jobject /* thiz */
) {
    if (audioIO) audioIO->onBackground();
}

} // extern "C"
