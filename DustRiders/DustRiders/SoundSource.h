#pragma once
#pragma comment(lib, "Shlwapi.lib")
#include <AL\al.h>
class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	ALuint Play(const ALuint buffer_to_play);

private:
	ALuint p_Source;
	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;
	ALuint p_Buffer = 0;
};

