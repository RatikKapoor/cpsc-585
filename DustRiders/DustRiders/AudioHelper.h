#pragma once

#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"

class AudioHelper {
public:
	static void Initialize();

	// static void PlayBackgroundNoise();
	static void PlayEngineNoise();
	static void SetEngineVolume(float v);
	static void PlayThunder();
	static void PlayLaser();
	static void PlayCollision();
	static void PlayPowerUp();
	static void PlayPowerDown();

protected:
	static SoundSource* engineSpeaker;
	static SoundSource* thunderSpeaker;
	static SoundSource* laserSpeaker;
	static SoundSource* collisionSpeaker;
	static SoundSource* powerUpSpeaker;
	static SoundSource* powerDownSpeaker;
	static SoundSource* stormSpeaker;
	static SoundSource* themeSpeaker;

	static uint32_t engine;
	static uint32_t thunder;
	static uint32_t laser;
	static uint32_t collision;
	static uint32_t powerUp;
	static uint32_t powerDown;
	static uint32_t storm;
	static uint32_t theme;

	static ALint themeState;
	static ALint stormState;
	static ALint engineSoundState;
	static ALint thunderSoundState;
	static ALint laserSoundState;
	static ALint collisionSoundState;
	static ALint powerUpSoundState;
	static ALint powerDownSoundState;

private:
	AudioHelper();
};
