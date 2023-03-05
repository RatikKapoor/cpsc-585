#pragma once

#include <map>
#include <string>

#include "Entity.h"

class EntityComponentSystem {
protected:
	EntityComponentSystem() {}
	static EntityComponentSystem* instance;

private:
	std::map<std::string, Entity*> map;

public:
	Entity* get(std::string s) {
		return map[s];
	}

	void set(std::string s, Entity* e) {
		map[s] = e;
	}

	std::vector<Entity*> getAll();
	std::vector<std::string> getKeys();
	void erase(std::string s) { map.erase(s); }
	bool doesKeyExist(std::string s) { return map.find(s) != map.end(); }

	static EntityComponentSystem* getInstance();

	Entity* operator[] (const std::string& s) const { return this->map.at(s); }
	Entity*& operator[] (const std::string& s) { return this->map[s]; }
};