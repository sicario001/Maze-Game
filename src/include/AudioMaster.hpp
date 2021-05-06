#pragma once

#include "AudioSource.hpp"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>


class AudioMaster
{
	public:
		AudioMaster();
		~AudioMaster();
		void release();
		void init() ;
		void setListenerPosition(float x, float y, float z);
		void setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ);
		void setListenerVelocity(float x, float y, float z);
		AudioSource* loadWaveFile(std::string path);
		
	private:
		static AudioMaster* instance;
		ALCdevice* device;
		ALCcontext* context;
};


