#pragma once
#include "engine/audio/AudioMaster.hpp"
#include "engine/GameEngine.hpp"
extern GameEngine* gEngine;

enum AsSoundType{
    AS_HOVER_SOUND,
    AS_CLICK_SOUND,
    AS_EXPLOSION_SOUND,
    AS_BOMB_BEEP_SOUND,
    AS_WALKING_SOUND,
    AS_SHOOTING_SOUND,
    AS_EMPTY_MAG,
    AS_RELOADING_SOUND,
    AS_SLASHING_SOUND,
    AS_HIT_SOUND,
    AS_BGM
};

class AudioStore{
    unsigned int sound_buffer[11];
    public:
        AudioStore();
        void init();
        void free();
        AudioSource* getSourceFor(AsSoundType t);
};