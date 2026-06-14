import type { TurboModule } from 'react-native';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
  /**
   * Initialize the Superpowered engine. Must be called before any other method.
   * @param licenseKey License key from superpowered.com/dev
   * @param tempFolderPath Path for temp files (use RNFS.TemporaryDirectoryPath or similar)
   */
  initPlayer(licenseKey: string, tempFolderPath: string): void;

  /**
   * Load an array of audio file paths as synchronized tracks.
   * All tracks will play in lockstep.
   */
  setTracks(paths: string[]): void;

  /** Start synchronized playback of all tracks. */
  play(): void;

  /** Pause all tracks simultaneously. */
  pause(): void;

  /**
   * Seek all tracks to a position.
   * @param ms Position in milliseconds.
   */
  setPosition(ms: number): void;

  /** @returns Current playback position in milliseconds. */
  getPositionMs(): Promise<number>;

  /**
   * Set the volume of a specific track with independent left/right channels.
   * Use 0/0 to mute, 1/1 for full volume, 1/0 for left-only, 0/1 for right-only.
   * @param index Track index (0-based).
   * @param left Left channel gain (0.0 to 1.0).
   * @param right Right channel gain (0.0 to 1.0).
   */
  setTrackVolume(index: number, left: number, right: number): void;

  /**
   * Set the global output volume.
   * @param value Gain (0.0 to 1.0).
   */
  setVolume(value: number): void;

  /**
   * Change playback rate (speed) without pitch shifting.
   * @param rate 1.0 = normal, 0.5 = half speed, 2.0 = double speed.
   */
  setRate(rate: number): void;

  /**
   * Shift pitch in cents (100 cents = 1 semitone).
   * @param cents Range: -2400 to 2400.
   */
  setPitchShiftCents(cents: number): void;

  /** @returns Total buffered percentage of all tracks (0.0 to 100.0). */
  getBufferedPercent(): Promise<number>;

  /**
   * @param index Track index (0-based).
   * @returns Buffered percentage for a specific track (0.0 to 100.0).
   */
  getTrackBufferedPercent(index: number): Promise<number>;

  /**
   * Get the duration of a file without loading it as a track.
   * @param path Full filesystem path to the audio file.
   * @returns Duration in milliseconds.
   */
  getDurationMs(path: string): Promise<number>;
}

export default TurboModuleRegistry.getEnforcing<Spec>('NativeMultiTrackPlayer');
