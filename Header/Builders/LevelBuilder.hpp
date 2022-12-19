#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include "Level.hpp"

struct LevelSettings {
	std::string name = "room";
	int rooms;
	int difficulty;


};

class LevelBuilder {
public:
	static Level createLevel(LevelSettings settings);

private:
};