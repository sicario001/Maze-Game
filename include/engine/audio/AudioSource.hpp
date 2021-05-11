
#pragma once

#include <string>

#include <AL/al.h>

enum EAudioSourceState
{
	STATE_EMPTY,
	STATE_STOP,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_REWIND
};

class AudioSource
{
	public:
		AudioSource(void *data, unsigned int size, unsigned int frequency, int format);
		AudioSource(AudioSource &src);
		void play(bool loop=false);
		void pause();
		void stop();
		void rewind();
		void free();
		void setPosition(float x, float y, float z);
		void setDirection(float x, float y, float z);
		void setVelocity(float x, float y, float z);
		void setPitch(float pitch);
		void setGain(float gain);
		void setRollOffFactor(float x);
		void setReferenceDistance(float x);
		int getState();
	private:
	
		unsigned int buffer;
		unsigned int source;
		EAudioSourceState state;
};

