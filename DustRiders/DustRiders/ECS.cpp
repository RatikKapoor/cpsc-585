#include "ECS.h"

EntityComponentSystem* EntityComponentSystem::instance = nullptr;

EntityComponentSystem* EntityComponentSystem::getInstance() {
	if (instance == nullptr) {
		instance = new EntityComponentSystem();
	}

	return instance;
}

std::vector<Entity*> EntityComponentSystem::getAll() {
	std::vector<Entity*> el;

	for (auto& m : map) {
		el.push_back(m.second);
	}

	return el;
}

std::vector<std::string> EntityComponentSystem::getKeys() {
	std::vector<std::string> kl;

	for (auto& k : map) {
		std::cout << k.first;
		kl.push_back(k.first);
	}

	return kl;
}