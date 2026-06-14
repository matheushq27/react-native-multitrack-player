import { useCallback, useEffect, useRef, useState } from 'react';
import NativeMultiTrackPlayer from '../NativeMultiTrackPlayer';

export interface TrackVolume {
  left: number;
  right: number;
}

export interface MultiTrackPlayerOptions {
  licenseKey: string;
  tempFolderPath: string;
}

export function useMultiTrackPlayer(options: MultiTrackPlayerOptions) {
  const initialized = useRef(false);
  const [isPlaying, setIsPlaying] = useState(false);

  useEffect(() => {
    if (!initialized.current) {
      NativeMultiTrackPlayer.initPlayer(
        options.licenseKey,
        options.tempFolderPath
      );
      initialized.current = true;
    }
  }, [options.licenseKey, options.tempFolderPath]);

  const setTracks = useCallback((paths: string[]) => {
    NativeMultiTrackPlayer.setTracks(paths);
  }, []);

  const play = useCallback(() => {
    NativeMultiTrackPlayer.play();
    setIsPlaying(true);
  }, []);

  const pause = useCallback(() => {
    NativeMultiTrackPlayer.pause();
    setIsPlaying(false);
  }, []);

  const togglePlayback = useCallback(() => {
    if (isPlaying) pause();
    else play();
  }, [isPlaying, play, pause]);

  const setPosition = useCallback((ms: number) => {
    NativeMultiTrackPlayer.setPosition(ms);
  }, []);

  const getPositionMs = useCallback((): Promise<number> => {
    return NativeMultiTrackPlayer.getPositionMs();
  }, []);

  const setTrackVolume = useCallback(
    (index: number, volume: TrackVolume) => {
      NativeMultiTrackPlayer.setTrackVolume(index, volume.left, volume.right);
    },
    []
  );

  const muteTrack = useCallback((index: number) => {
    NativeMultiTrackPlayer.setTrackVolume(index, 0, 0);
  }, []);

  const panTrackLeft = useCallback((index: number) => {
    NativeMultiTrackPlayer.setTrackVolume(index, 1, 0);
  }, []);

  const panTrackRight = useCallback((index: number) => {
    NativeMultiTrackPlayer.setTrackVolume(index, 0, 1);
  }, []);

  const panTrackCenter = useCallback((index: number) => {
    NativeMultiTrackPlayer.setTrackVolume(index, 1, 1);
  }, []);

  const setVolume = useCallback((value: number) => {
    NativeMultiTrackPlayer.setVolume(value);
  }, []);

  const setRate = useCallback((rate: number) => {
    NativeMultiTrackPlayer.setRate(rate);
  }, []);

  const setPitchShiftCents = useCallback((cents: number) => {
    NativeMultiTrackPlayer.setPitchShiftCents(cents);
  }, []);

  const getBufferedPercent = useCallback((): Promise<number> => {
    return NativeMultiTrackPlayer.getBufferedPercent();
  }, []);

  const getTrackBufferedPercent = useCallback(
    (index: number): Promise<number> => {
      return NativeMultiTrackPlayer.getTrackBufferedPercent(index);
    },
    []
  );

  const getDurationMs = useCallback((path: string): Promise<number> => {
    return NativeMultiTrackPlayer.getDurationMs(path);
  }, []);

  return {
    isPlaying,
    setTracks,
    play,
    pause,
    togglePlayback,
    setPosition,
    getPositionMs,
    setTrackVolume,
    muteTrack,
    panTrackLeft,
    panTrackRight,
    panTrackCenter,
    setVolume,
    setRate,
    setPitchShiftCents,
    getBufferedPercent,
    getTrackBufferedPercent,
    getDurationMs,
  };
}
