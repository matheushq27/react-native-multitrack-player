import { useEffect } from 'react';
import RNFS from 'react-native-fs';
import { NativeMultiTrackPlayer, useMultiTrackPlayer } from 'react-native-multitrack-player';

const trackNames = ['Vocals', 'Guitars', 'Drums', 'Keyboards', 'Effects'];
const trackURIs = trackNames.map((name) => `https://www.mboxdrive.com/${name}.mp3`);

export default function useTracks() {
  useMultiTrackPlayer({
    licenseKey: 'ExampleLicenseKey-WillExpire-OnNextUpdate',
    tempFolderPath: RNFS.TemporaryDirectoryPath,
  });

  useEffect(() => {
    NativeMultiTrackPlayer.setTracks(trackURIs);
  }, []);

  return trackNames;
}
