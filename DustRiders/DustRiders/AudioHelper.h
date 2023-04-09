#pragma once

#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"

class AudioHelper {
public:
	AudioHelper(): storm("sound/storm_long.wav"), theme("sound/jumphigher.ogg")
	{
		engine = SoundBuffer::get()->addSoundEffect("sound/engine_2.wav");
		collision = SoundBuffer::get()->addSoundEffect("sound/collision.ogg");
		thunder = SoundBuffer::get()->addSoundEffect("sound/thunder.wav");
		storm = SoundBuffer::get()->addSoundEffect("sound/storm_long.wav");
		theme = SoundBuffer::get()->addSoundEffect("sound/jumphigher.ogg");

		engineSpeaker = SoundSource();
		thunderSpeaker = SoundSource();
		stormSpeaker = SoundSource();
		themeSpeaker = SoundSource();

		engineSpeaker.changeMusicVolume(0.05f);
		stormSpeaker.changeMusicVolume(0.4f);
		themeSpeaker.changeMusicVolume(0.3f);

		stormSpeaker.enableLooping();
		themeSpeaker.enableLooping();

		engineSoundState = AL_STOPPED;
		thunderSoundState = AL_STOPPED;

		alSourcePlay(stormSpeaker.Play(storm));
		alSourcePlay(themeSpeaker.Play(theme));
	}

	void PlayBackgroundNoise()
	{
		// The sound buffer should always update, not dependant on game state
		if (stormState == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			storm.UpdateBufferStream();
			alGetSourcei(storm.getSource(), AL_SOURCE_STATE, &stormState);
		}

		if (themeState == AL_PLAYING && alGetError() == AL_NO_ERROR)
		{
			theme.UpdateBufferStream();
			alGetSourcei(theme.getSource(), AL_SOURCE_STATE, &themeState);
		}
	}

	void PlayEngineNoise()
	{
		alGetSourcei(engineSpeaker.getSource(), AL_SOURCE_STATE, &engineSoundState);
		if (engineSoundState != AL_PLAYING) {
			alSourcePlay(engineSpeaker.Play(engine));
		}
	}

	void SetEngineVolume(float v)
	{
		engineSpeaker.changeMusicVolume(v);
	}

	void PlayThunder()
	{
		alGetSourcei(thunderSpeaker.getSource(), AL_SOURCE_STATE, &thunderSoundState);
		if (thunderSoundState != AL_PLAYING) {
			alSourcePlay(thunderSpeaker.Play(thunder));
		}
	}

protected:
	SoundSource engineSpeaker;
	SoundSource thunderSpeaker;
	SoundSource stormSpeaker;
	SoundSource themeSpeaker;

	uint32_t engine;
	uint32_t collision;
	uint32_t thunder;
	uint32_t storm;
	uint32_t theme;

	ALint engineSoundState;
	ALint stormSoundState;
	ALint themeSoundState;
	ALint thunderSoundState;
};
