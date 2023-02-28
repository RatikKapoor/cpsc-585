#pragma once

#include <map>
#include <string>

#include "Entity.h"

class EntityComponentSystem {
private:
	static std::map<std::string, std::shared_ptr<Entity>> map;

public:
	static std::shared_ptr<Entity> get(std::string s) {
		return map[s];
	}

	static void set(std::string s, std::shared_ptr<Entity> e) {
		map[s] = e;
	}

	static std::vector<std::shared_ptr<Entity>> getAll() {
		std::vector<std::shared_ptr<Entity>> el;

		//std::transform(map.begin(), map.end(),
		//	std::back_inserter(el),
		//	[](const std::pair<std::string, std::shared_ptr<Entity>> p) {
		//		return p.second;
		//	});

		return el;
	}

	std::shared_ptr<Entity> operator[] (const std::string& s) const { return map[s]; }
	std::shared_ptr<Entity>& operator[] (const std::string& s) { return map[s]; }
};
