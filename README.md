# react-native-multitrack-player

Reprodução de múltiplas faixas de áudio com baixa latência para React Native, baseada no Superpowered Audio SDK.

Biblioteca recriada a partir do TCC _"Áudio De Baixa Latência Em Dispositivos Móveis"_ — Maykon Michel Palma, UNESP Bauru, 2022.

## Requisitos

- React Native 0.74+ (Nova Arquitetura / TurboModules)
- Android: NDK, minSdkVersion 21
- iOS: 13.0+
- **Licença Superpowered**: registre-se em [superpowered.com/dev](https://superpowered.com/dev) para obter sua chave gratuita

## Instalação

```sh
npm install react-native-multitrack-player
# ou
yarn add react-native-multitrack-player
```

### iOS (CocoaPods)

```sh
cd ios && pod install
```

### Android

O build system configura automaticamente o NDK via CMake. Certifique-se de que o NDK está instalado no Android Studio.

## Uso

```tsx
import { useMultiTrackPlayer } from 'react-native-multitrack-player';
import RNFS from 'react-native-fs'; // para obter o caminho temporário

export function MixerScreen() {
  const {
    isPlaying,
    setTracks,
    togglePlayback,
    setTrackVolume,
    muteTrack,
    panTrackLeft,
    panTrackRight,
    panTrackCenter,
    setRate,
    setPitchShiftCents,
  } = useMultiTrackPlayer({
    licenseKey: 'SUA_CHAVE_SUPERPOWERED',
    tempFolderPath: RNFS.TemporaryDirectoryPath,
  });

  useEffect(() => {
    setTracks([
      '/path/to/vocal.mp3',
      '/path/to/guitar.mp3',
      '/path/to/drums.mp3',
      '/path/to/keys.mp3',
      '/path/to/bass.mp3',
    ]);
  }, []);

  return (
    <>
      <Button title={isPlaying ? 'Pause' : 'Play'} onPress={togglePlayback} />

      {/* Panning da guitarra */}
      <Button title="Guitar L" onPress={() => panTrackLeft(1)} />
      <Button title="Guitar C" onPress={() => panTrackCenter(1)} />
      <Button title="Guitar R" onPress={() => panTrackRight(1)} />

      {/* Mute da bateria */}
      <Button title="Mute Drums" onPress={() => muteTrack(2)} />

      {/* Volume individual */}
      <Slider onValueChange={(v) => setTrackVolume(0, { left: v, right: v })} />

      {/* Pitch shift: +200 cents = 2 semitons acima */}
      <Button title="+2 semitones" onPress={() => setPitchShiftCents(200)} />
    </>
  );
}
```

## API

### `useMultiTrackPlayer(options)`

#### Opções

| Prop | Tipo | Descrição |
|------|------|-----------|
| `licenseKey` | `string` | Chave do Superpowered SDK |
| `tempFolderPath` | `string` | Caminho para arquivos temporários |

#### Retorno

| Método | Assinatura | Descrição |
|--------|-----------|-----------|
| `setTracks` | `(paths: string[]) => void` | Carrega as faixas |
| `play` | `() => void` | Inicia reprodução sincronizada |
| `pause` | `() => void` | Pausa todas as faixas |
| `togglePlayback` | `() => void` | Alterna play/pause |
| `setPosition` | `(ms: number) => void` | Seek em milissegundos |
| `getPositionMs` | `() => Promise<number>` | Posição atual |
| `setTrackVolume` | `(index, { left, right }) => void` | Volume L/R por faixa |
| `muteTrack` | `(index) => void` | Silencia uma faixa |
| `panTrackLeft` | `(index) => void` | Panning esquerdo |
| `panTrackRight` | `(index) => void` | Panning direito |
| `panTrackCenter` | `(index) => void` | Panning central (estéreo) |
| `setVolume` | `(value) => void` | Volume global (0–1) |
| `setRate` | `(rate) => void` | Velocidade (1.0 = normal) |
| `setPitchShiftCents` | `(cents) => void` | Pitch shift (−2400 a 2400) |
| `getBufferedPercent` | `() => Promise<number>` | % total carregado |
| `getTrackBufferedPercent` | `(index) => Promise<number>` | % de uma faixa |
| `getDurationMs` | `(path) => Promise<number>` | Duração de um arquivo |

## Arquitetura

```
TypeScript (useMultiTrackPlayer hook)
    ↓ TurboModule (Codegen)
Android: Kotlin → JNI → C++ | iOS: Objective-C++ → C++
    ↓
Track.cpp + MultiTracksPlayer.cpp
    ↓
Superpowered Audio SDK v2.8.0
    ↓
Android: OpenSL ES  |  iOS: CoreAudio
```

## Licença

MIT — veja [LICENSE](LICENSE).

O Superpowered Audio SDK tem licença própria. Consulte [superpowered.com](https://superpowered.com).
