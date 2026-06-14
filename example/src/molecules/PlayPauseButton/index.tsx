import React, { useEffect, useState } from 'react';
import { TouchableOpacity, View } from 'react-native';
import { NativeMultiTrackPlayer } from 'react-native-multitrack-player';
import Icon from 'react-native-vector-icons/MaterialCommunityIcons';

import { useColors } from '../../style';
import styles from './styles';

const PlayPauseButton = () => {
  const [playing, setPlaying] = useState(false);

  const togglePlaying = () => setPlaying(!playing);

  useEffect(() => {
    if (playing) NativeMultiTrackPlayer.play();
    else NativeMultiTrackPlayer.pause();
  }, [playing]);

  const colors = useColors();
  const disabledColor = `${colors.secondary}55`;

  return (
    <View style={styles.container}>
      <Icon name="skip-previous" size={40} color={disabledColor} />
      <TouchableOpacity onPress={togglePlaying}>
        <Icon
          name={playing ? 'pause-circle' : 'play-circle'}
          size={80}
          color={colors.primary}
        />
      </TouchableOpacity>
      <Icon name="skip-next" size={40} color={disabledColor} />
    </View>
  );
};

export default PlayPauseButton;
