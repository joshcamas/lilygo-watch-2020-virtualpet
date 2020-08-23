
#include "SoundManager.h"
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

PlayingSound::PlayingSound(AudioGenerator *audioGenerator)
{
  this->audioGenerator = audioGenerator;
}

bool PlayingSound::isPlaying()
{
  return this->audioGenerator->isRunning();
}

bool PlayingSound::loop()
{
  if (!this->audioGenerator->loop())
  {
    //Trigger on complete loop
    this->onComplete();
    return false;
  }

  return true;
}

SoundManager::SoundManager(WatchManager *manager)
{
  this->manager = manager;

  for (int i = 0; i < 10; i++)
    this->playingAudio[i] = nullptr;
}

void SoundManager::enableAudio()
{
  this->manager->watch->enableLDO3();

  //Initiate default output
  if (this->defaultOutput == nullptr)
    this->defaultOutput = createOutput();
}

AudioOutputI2S *SoundManager::createOutput()
{
  AudioOutputI2S *out = new AudioOutputI2S();
  out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
  return out;
}

PlayingSound *SoundManager::playWAVFromMem(const void *data, uint32_t len, AudioOutputI2S *output)
{
  int listIndex = -1;

  for (int i = 0; i < 10; i++)
  {
    if (this->playingAudio[i] == nullptr)
    {
      listIndex = i;
      break;
    }
  }

  if (listIndex == -1)
  {
    Serial.printf("ERROR: Could not play sound due to max sounds being 10");
    return nullptr;
  }

  AudioFileSourcePROGMEM *file = new AudioFileSourcePROGMEM(data, len);
  AudioFileSourceID3 *id3 = new AudioFileSourceID3(file);
  AudioGeneratorWAV *wav = new AudioGeneratorWAV();

  PlayingSound *sound = new PlayingSound(wav);

  /*
    //Close file on completion
    auto onComplete = [=] () 
    { 
      Serial.printf("File closing\n");
      file->close(); 
    };
    
    sound->onComplete += onComplete;
    */

  this->playingAudio[listIndex] = sound;

  if (output == nullptr)
    wav->begin(id3, this->defaultOutput);
  else
    wav->begin(id3, output);

  return sound;
}

void SoundManager::update()
{
  for (int i = 0; i < 10; i++)
  {
    if (this->playingAudio[i] != nullptr)
    {
      if (!this->playingAudio[i]->loop())
        this->playingAudio[i] = nullptr;
    }
  }
}