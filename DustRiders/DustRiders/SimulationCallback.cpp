#include <regex>

#include "SimulationCallback.h"

SimulationCallback::SimulationCallback(EntityComponentSystem& ecs) : PxSimulationEventCallback(), ecs(ecs) {
	speedUpSpeaker = new SoundSource();
	slowDownSpeaker = new SoundSource();
	speedUpSound = SoundBuffer::get()->addSoundEffect("sound/power-up.wav");
	slowDownSound = SoundBuffer::get()->addSoundEffect("sound/power-down.wav");
}

void SimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	auto triggerEntityName = pairs->triggerActor->getName();
	auto actorEntityName = pairs->otherActor->getName();

	if (regex_match(triggerEntityName, regex("(speedUpZone)(.*)")) && regex_match(actorEntityName, regex("(car)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySpeedupEffect(5);
		alSourcePlay(speedUpSpeaker->Play(speedUpSound));
	}
	else if (regex_match(triggerEntityName, regex("(slowDownZone)(.*)")) && regex_match(actorEntityName, regex("(car)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySlowdownEffect(5);
		alSourcePlay(slowDownSpeaker->Play(slowDownSound));
	}
}
