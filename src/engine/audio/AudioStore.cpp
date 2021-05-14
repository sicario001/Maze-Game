#include "engine/audio/AudioStore.hpp"

AudioStore::AudioStore(){
    for (int i = 0; i < 11; i++)
    {
        sound_buffer[i]=0;
    }
    
}

void AudioStore::init(){
    gEngine->audioMaster->loadWaveFile("media/audio/ui_click1.wav",sound_buffer[AS_HOVER_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/ui_click2.wav",sound_buffer[AS_CLICK_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/explosion.wav",sound_buffer[AS_EXPLOSION_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/beep.wav",sound_buffer[AS_BOMB_BEEP_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/walk.wav",sound_buffer[AS_WALKING_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/gunshot.wav",sound_buffer[AS_SHOOTING_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/empty_mag.wav",sound_buffer[AS_EMPTY_MAG]);
    gEngine->audioMaster->loadWaveFile("media/audio/reload.wav",sound_buffer[AS_RELOADING_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/slash.wav",sound_buffer[AS_SLASHING_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/bullethit.wav",sound_buffer[AS_HIT_SOUND]);
    gEngine->audioMaster->loadWaveFile("media/audio/bgm.wav",sound_buffer[AS_BGM]);
}

void AudioStore::free(){
	alDeleteBuffers(11, sound_buffer);
}

AudioSource* AudioStore::getSourceFor(AsSoundType t){
    return new AudioSource(sound_buffer[t]);
}