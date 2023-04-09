#include "AudioHelper.h"


SoundSource* AudioHelper::engineSpeaker;
SoundSource* AudioHelper::thunderSpeaker;
SoundSource* AudioHelper::laserSpeaker;
SoundSource* AudioHelper::collisionSpeaker;
SoundSource* AudioHelper::powerUpSpeaker;
SoundSource* AudioHelper::powerDownSpeaker;
SoundSource* AudioHelper::stormSpeaker;
SoundSource* AudioHelper::themeSpeaker;

uint32_t AudioHelper::engine;
uint32_t AudioHelper::thunder;
uint32_t AudioHelper::laser;
uint32_t AudioHelper::collision;
uint32_t AudioHelper::powerUp;
uint32_t AudioHelper::powerDown;
uint32_t AudioHelper::storm;
uint32_t AudioHelper::theme;

ALint AudioHelper::themeState;
ALint AudioHelper::stormState;
ALint AudioHelper::engineSoundState;
ALint AudioHelper::thunderSoundState;
ALint AudioHelper::laserSoundState;
ALint AudioHelper::collisionSoundState;
ALint AudioHelper::powerUpSoundState;
ALint AudioHelper::powerDownSoundState;

void AudioHelper::Initialize()
{
	engine = SoundBuffer::get()->addSoundEffect("sound/engine_2.wav");
	thunder = SoundBuffer::get()->addSoundEffect("sound/thunder.wav");
	laser = SoundBuffer::get()->addSoundEffect("sound/laser-shoot.wav");
	collision = SoundBuffer::get()->addSoundEffect("sound/collision.ogg");
	powerUp = SoundBuffer::get()->addSoundEffect("sound/power-up.wav");
	powerDown = SoundBuffer::get()->addSoundEffect("sound/power-down.wav");
	storm = SoundBuffer::get()->addSoundEffect("sound/storm_long.wav");
	theme = SoundBuffer::get()->addSoundEffect("sound/jumphigher.ogg");

	engineSpeaker = new SoundSource();
	thunderSpeaker = new SoundSource();
	laserSpeaker = new SoundSource();
	collisionSpeaker = new SoundSource();
	powerUpSpeaker = new SoundSource();
	powerDownSpeaker = new SoundSource();
	stormSpeaker = new SoundSource();
	themeSpeaker = new SoundSource();

	engineSpeaker->changeMusicVolume(0.05f);
	stormSpeaker->changeMusicVolume(0.4f);
	themeSpeaker->changeMusicVolume(0.3f);

	stormSpeaker->enableLooping();
	themeSpeaker->enableLooping();

	engineSoundState = AL_STOPPED;
	thunderSoundState = AL_STOPPED;
	laserSoundState = AL_STOPPED;
	collisionSoundState = AL_STOPPED;
	powerUpSoundState = AL_STOPPED;
	powerDownSoundState = AL_STOPPED;

	alSourcePlay(stormSpeaker->Play(storm));
	alSourcePlay(themeSpeaker->Play(theme));
}

// void AudioHelper::PlayBackgroundNoise()
// {
// 	// The sound buffer should always update, not dependant on game state
// 	if (stormState == AL_PLAYING && alGetError() == AL_NO_ERROR)
// 	{
// 		storm->UpdateBufferStream();
// 		alGetSourcei(storm->getSource(), AL_SOURCE_STATE, &stormState);
// 	}

// 	if (themeState == AL_PLAYING && alGetError() == AL_NO_ERROR)
// 	{
// 		theme->UpdateBufferStream();
// 		alGetSourcei(theme->getSource(), AL_SOURCE_STATE, &themeState);
// 	}
// }

void AudioHelper::PlayEngineNoise()
{
	alGetSourcei(engineSpeaker->getSource(), AL_SOURCE_STATE, &engineSoundState);
	if (engineSoundState != AL_PLAYING) {
		alSourcePlay(engineSpeaker->Play(engine));
	}
}

void AudioHelper::SetEngineVolume(float v)
{
	engineSpeaker->changeMusicVolume(v);
}

void AudioHelper::PlayThunder()
{
	alGetSourcei(thunderSpeaker->getSource(), AL_SOURCE_STATE, &thunderSoundState);
	if (thunderSoundState != AL_PLAYING) {
		alSourcePlay(thunderSpeaker->Play(thunder));
	}
}

void AudioHelper::PlayLaser()
{
	alGetSourcei(laserSpeaker->getSource(), AL_SOURCE_STATE, &laserSoundState);
	if (laserSoundState != AL_PLAYING) {
		alSourcePlay(laserSpeaker->Play(laser));
	}
}

void AudioHelper::PlayCollision()
{
	alGetSourcei(collisionSpeaker->getSource(), AL_SOURCE_STATE, &collisionSoundState);
	if (collisionSoundState != AL_PLAYING) {
		alSourcePlay(collisionSpeaker->Play(collision));
	}
}

void AudioHelper::PlayPowerUp()
{
	alGetSourcei(powerUpSpeaker->getSource(), AL_SOURCE_STATE, &powerUpSoundState);
	if (powerUpSoundState != AL_PLAYING) {
		alSourcePlay(powerUpSpeaker->Play(powerUp));
	}
}

void AudioHelper::PlayPowerDown()
{
	alGetSourcei(powerDownSpeaker->getSource(), AL_SOURCE_STATE, &powerDownSoundState);
	if (powerDownSoundState != AL_PLAYING) {
		alSourcePlay(powerDownSpeaker->Play(powerDown));
	}
}
