#include "engine/audio/AudioSource.hpp"
#include <AL/alut.h>
AudioSource::AudioSource(unsigned int &pBuffer):
	buffer(pBuffer),
	source(0),
	state(STATE_STOP)
{	
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, pBuffer);
}

void AudioSource::play(bool loop)
{
	if (state != STATE_PLAY) {
		state = STATE_PLAY;
		
		alSourcei(source, AL_LOOPING, loop);
		alSourcePlay(source);
	}
}
void AudioSource::pause()
{
	if (state != STATE_PAUSE) {
		state = STATE_PAUSE;
		
		alSourcePause(source);
	}
}

void AudioSource::stop()
{
	if (state != STATE_STOP) {
		state = STATE_STOP;
		
		alSourceStop(source);
	}
}

void AudioSource::rewind()
{
	if (state != STATE_REWIND) {
		state = STATE_REWIND;
		
		alSourceStop(source);
	}
}

void AudioSource::setPosition(float x, float y, float z)
{
	alSource3f(source, AL_POSITION, x, y, z);
}

void AudioSource::setDirection(float x, float y, float z)
{
	float direction[3] = {x, y, z};
	
	alSourcefv(source, AL_DIRECTION, direction);
}

void AudioSource::setVelocity(float x, float y, float z)
{
	alSource3f(source, AL_VELOCITY, x, y, z);
}

void AudioSource::setPitch(float pitch)
{
	alSourcef(source, AL_PITCH, pitch);
}

void AudioSource::setGain(float gain)
{
	alSourcef(source, AL_GAIN, gain);
}

void AudioSource::free()
{
	if (state==STATE_EMPTY)
	{
		return;
	}
	
	alDeleteSources(1, &source);

	state = STATE_EMPTY;
}

int AudioSource::getState(){
	int st;
	alGetSourcei(source, AL_SOURCE_STATE, &st);
	return st;
}

void AudioSource::setRollOffFactor(float x){
	alSourcef(source,AL_ROLLOFF_FACTOR,x);
}

void AudioSource::setReferenceDistance(float x){
	alSourcef(source,AL_REFERENCE_DISTANCE,x);
}