#pragma once

#include "WatchManager.h"
#include "delegate/MultiCastDelegate.h"

#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

using namespace SA;

/*
 * External Libraries:
 * https://github.com/earlephilhower/ESP8266Audio
 * https://github.com/Gianbacchio/ESP8266_Spiram
*/

class PlayingSound {
  private:
  AudioGenerator *audioGenerator;
  
  public:
  multicast_delegate<void()> onComplete;

  PlayingSound(AudioGenerator *audioGenerator);
  
  bool isPlaying();

  bool loop();
  
};

class SoundManager {
  private:
    WatchManager *manager = nullptr;
    AudioOutputI2S *defaultOutput = nullptr;

    PlayingSound *playingAudio [10];
  public:
  
  SoundManager(WatchManager *manager);

  void enableAudio();

  AudioOutputI2S* createOutput();

  PlayingSound* playWAVFromMem(const void *data,uint32_t len, AudioOutputI2S *output = nullptr);
  
  void loop();
};
