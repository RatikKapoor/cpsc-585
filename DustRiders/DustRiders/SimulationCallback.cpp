#include <regex>

#include "SimulationCallback.h"

SimulationCallback::SimulationCallback(EntityComponentSystem& ecs) : PxSimulationEventCallback(), ecs(ecs) {
	noiseSpeaker = new SoundSource();
	speedUpSound = SoundBuffer::get()->addSoundEffect("sound/blessing.ogg");
	slowDownSound = SoundBuffer::get()->addSoundEffect("sound/spell.ogg");
}

void SimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
	auto triggerEntityName = pairs->triggerActor->getName();
	auto actorEntityName = pairs->otherActor->getName();

	if (regex_match(triggerEntityName, regex("(speedUpZone)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySpeedupEffect(5);
		alSourcePlay(noiseSpeaker->Play(speedUpSound));
	}
	else if (regex_match(triggerEntityName, regex("(slowDownZone)(.*)"))) {
		((Vehicle*)ecs[actorEntityName])->applySlowdownEffect(5);
		alSourcePlay(noiseSpeaker->Play(slowDownSound));
	}
}