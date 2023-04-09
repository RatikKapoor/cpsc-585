#include <regex>

#include "SimulationCallback.h"

SimulationCallback::SimulationCallback(EntityComponentSystem& ecs) : PxSimulationEventCallback(), ecs(ecs) {
	speedUpSpeaker = SoundSource();
	slowDownSpeaker = SoundSource();
	speedUpSound = SoundBuffer::get()->addSoundEffect("sound/power-up.wav");
	slowDownSound = SoundBuffer::get()->addSoundEffect("sound/power-down.wav");
	speedUpSoundState = AL_STOPPED;
	slowDownSoundState = AL_STOPPED;
}

void SimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	auto triggerEntityName = pairs->triggerActor->getName();
	auto actorEntityName = pairs->otherActor->getName();

	if (regex_match(triggerEntityName, regex("(speedUpZone)(.*)")) && regex_match(actorEntityName, regex("(car)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySpeedupEffect(5);
		alGetSourcei(speedUpSpeaker.getSource(), AL_SOURCE_STATE, &speedUpSoundState);
		if (speedUpSoundState != AL_PLAYING) {
			alSourcePlay(speedUpSpeaker.Play(speedUpSound));
		}
	}
	else if (regex_match(triggerEntityName, regex("(slowDownZone)(.*)")) && regex_match(actorEntityName, regex("(car)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySlowdownEffect(5);
		alGetSourcei(slowDownSpeaker.getSource(), AL_SOURCE_STATE, &slowDownSoundState);
		if (slowDownSoundState != AL_PLAYING) {
			alSourcePlay(slowDownSpeaker.Play(slowDownSound));
		}
	}
}
