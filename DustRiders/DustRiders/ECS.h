#pragma once

#include <map>
#include <string>

#include "Entity.h"

class EntityComponentSystem {
private:
	std::map<std::string, Entity*> map;

public:
	Entity* get(std::string s) {
		return map[s];
	}

	void set(std::string s, Entity* e) {
		map[s] = e;
	}

	std::vector<Entity*> getAll() {
		std::vector<Entity*> el;

		std::transform(map.begin(), map.end(),
			std::back_inserter(el),
			[](const std::pair<std::string, Entity*> p) {
				return p.second;
			});

		return el;
	}

	Entity* operator[] (const std::string& s) const { return this->map.at(s); }
	Entity*& operator[] (const std::string& s) { return this->map[s]; }
};
