#include "engine/audio/AudioMaster.hpp"

AudioMaster::AudioMaster() :
	device(NULL),
	context(NULL)
{
}
AudioMaster::~AudioMaster()
{
}
void AudioMaster::free()
{
	if (context != NULL) {
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
	}
	
	if (device != NULL) {
		alcCloseDevice(device);
	}
}
AudioSource* AudioMaster::loadWaveFile(std::string path)
{
	AudioSource* entity = NULL;
	ALvoid *data;
	ALsizei size, freq;
	ALenum format;
	ALboolean loop = AL_FALSE;
	
	if (path.empty()) {
		throw "ALError: Invalid path";
	}
	
	std::ifstream file(path.c_str(), std::ifstream::binary);
	
	alutLoadWAVFile((ALbyte*)path.c_str(), &format, &data, &size, &freq, &loop);

	entity = new AudioSource(data, size, freq, format);
	
	return entity;
}
void AudioMaster::init()
{
	device = alcOpenDevice(NULL);
	
	if (device == NULL) {
		throw "ALError: Couldn't open device";
	}
	
	context = alcCreateContext(device, NULL);
	
	if (context == NULL) {
		throw "ALError: Couldn't create context";
	} else {
		alcMakeContextCurrent(context);
	}
	alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);
}

void AudioMaster::setListenerPosition(float x, float y, float z)
{
	alListener3f(AL_POSITION, x, y, z);
}

void AudioMaster::setListenerOrientation(float atX, float atY, float atZ, float upX, float upY, float upZ)
{
	float orientation[6] = {atX, atY, atZ, upX, upY, upZ};
	
	alListenerfv(AL_ORIENTATION, orientation);
}

void AudioMaster::setListenerVelocity(float x, float y, float z)
{
	alListener3f(AL_VELOCITY, x, y, z);
}