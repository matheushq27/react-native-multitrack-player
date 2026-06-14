package com.reactnativemultitrackplayer

import android.media.AudioManager
import android.content.Context
import com.facebook.react.bridge.*
import com.facebook.react.module.annotations.ReactModule

@ReactModule(name = MultiTrackPlayerModule.NAME)
class MultiTrackPlayerModule(reactContext: ReactApplicationContext) :
    NativeMultiTrackPlayerSpec(reactContext) {

    companion object {
        const val NAME = "NativeMultiTrackPlayer"

        init {
            System.loadLibrary("MultiTrackPlayer")
        }
    }

    // JNI declarations
    private external fun nativeInit(licenseKey: String, tempFolder: String, samplerate: Int, bufferSize: Int)
    private external fun nativeSetTracks(paths: Array<String>)
    private external fun nativePlay()
    private external fun nativePause()
    private external fun nativeSetPosition(ms: Double)
    private external fun nativeGetPositionMs(): Double
    private external fun nativeSetTrackVolume(index: Int, left: Float, right: Float)
    private external fun nativeSetVolume(value: Float)
    private external fun nativeSetRate(rate: Float)
    private external fun nativeSetPitchShiftCents(cents: Int)
    private external fun nativeGetBufferedPercent(): Float
    private external fun nativeGetTrackBufferedPercent(index: Int): Float
    private external fun nativeGetDurationMs(path: String): Double
    private external fun nativeOnForeground()
    private external fun nativeOnBackground()

    override fun getName() = NAME

    override fun initPlayer(licenseKey: String, tempFolderPath: String) {
        val audioManager = reactApplicationContext.getSystemService(Context.AUDIO_SERVICE) as AudioManager
        val samplerate = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE)?.toIntOrNull() ?: 48000
        val bufferSize = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER)?.toIntOrNull() ?: 512
        nativeInit(licenseKey, tempFolderPath, samplerate, bufferSize)
    }

    override fun setTracks(paths: ReadableArray) {
        val arr = Array(paths.size()) { paths.getString(it) ?: "" }
        nativeSetTracks(arr)
    }

    override fun play() = nativePlay()
    override fun pause() = nativePause()

    override fun setPosition(ms: Double) = nativeSetPosition(ms)

    override fun getPositionMs(promise: Promise) {
        promise.resolve(nativeGetPositionMs())
    }

    override fun setTrackVolume(index: Double, left: Double, right: Double) =
        nativeSetTrackVolume(index.toInt(), left.toFloat(), right.toFloat())

    override fun setVolume(value: Double) = nativeSetVolume(value.toFloat())
    override fun setRate(rate: Double) = nativeSetRate(rate.toFloat())
    override fun setPitchShiftCents(cents: Double) = nativeSetPitchShiftCents(cents.toInt())

    override fun getBufferedPercent(promise: Promise) {
        promise.resolve(nativeGetBufferedPercent().toDouble())
    }

    override fun getTrackBufferedPercent(index: Double, promise: Promise) {
        promise.resolve(nativeGetTrackBufferedPercent(index.toInt()).toDouble())
    }

    override fun getDurationMs(path: String, promise: Promise) {
        promise.resolve(nativeGetDurationMs(path))
    }

    fun onHostResume() = nativeOnForeground()
    fun onHostPause() = nativeOnBackground()
}
