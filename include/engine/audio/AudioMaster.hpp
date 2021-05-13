#pragma once

#include <stdlib.h>
#include <iostream>
#include <fstream>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "engine/audio/AudioSource.hpp"

class AudioMaster
{
	public:
		AudioMaster();
		~AudioMaster();
		void free();
		void init() ;
		void setListenerPosition(float x, float y, float z);
		void setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ);
		void setListenerVelocity(float x, float y, float z);
		void loadWaveFile(std::string path, unsigned int& buffer);
		
	private:
		static AudioMaster* instance;
		ALCdevice* device = NULL;
		ALCcontext* context = NULL;
};


