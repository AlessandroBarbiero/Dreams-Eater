#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"

struct LevelSettings {
	std::string name = "room";
	int rooms;



};

class LevelBuilder {
public:
	static std::shared_ptr<GameObject> createLevel(LevelSettings settings);

private:
};