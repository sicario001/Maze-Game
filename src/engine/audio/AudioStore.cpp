#include "engine/audio/AudioStore.hpp"

AudioStore::AudioStore(){
    for (int i = 0; i < 10; i++)
    {
        sound_buffer[i]=0;
    }
    
}

void AudioStore::init(){
    gEngine->audioMaster->loadWaveFile("media/audio/ui_click1.wav",sound_buffer[0]);
    gEngine->audioMaster->loadWaveFile("media/audio/ui_click2.wav",sound_buffer[1]);
    gEngine->audioMaster->loadWaveFile("media/audio/explosion.wav",sound_buffer[2]);
    gEngine->audioMaster->loadWaveFile("media/audio/beep.wav",sound_buffer[3]);
    gEngine->audioMaster->loadWaveFile("media/audio/walk.wav",sound_buffer[4]);
    gEngine->audioMaster->loadWaveFile("media/audio/gunshot.wav",sound_buffer[5]);
    gEngine->audioMaster->loadWaveFile("media/audio/reload.wav",sound_buffer[6]);
    gEngine->audioMaster->loadWaveFile("media/audio/slash.wav",sound_buffer[7]);
    gEngine->audioMaster->loadWaveFile("media/audio/bullethit.wav",sound_buffer[8]);
    gEngine->audioMaster->loadWaveFile("media/audio/bgm.wav",sound_buffer[9]);
}

void AudioStore::free(){
	alDeleteBuffers(10, sound_buffer);
}

AudioSource* AudioStore::getSourceFor(AsSoundType t){
    return new AudioSource(sound_buffer[t]);
}