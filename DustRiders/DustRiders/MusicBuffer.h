#pragma once
#include <AL\al.h>
#include <sndfile.h>
class MusicBuffer
{
public:
	void Play();
	//void Pause();
	//void Stop();

	void UpdateBufferStream();

	ALint getSource();
	void Reload();

	void enableLooping();
	void disableLooping();

	void changeMusicVolume(float newVolume);

	MusicBuffer(const char* filename);
	~MusicBuffer();
private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	MusicBuffer() = delete;
};

