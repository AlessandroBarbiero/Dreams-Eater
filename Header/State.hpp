#pragma once

#include <map>
#include <glm/glm.hpp>


enum class State {
	Idle,
	Run,
	Walk,
	Attack,
	Die,
	Hit,
	Victory
};

const std::map<State, char*> StateToString{
	{State::Idle,			"Idle"		},
	{State::Run,			"Run"		},
	{State::Walk,			"Walk"		},
	{State::Attack,			"Attack"	},
	{State::Die,			"Die"		},
	{State::Hit,			"Hit"		},
	{State::Victory,		"Victory"	},
};

enum class Direction {
	RIGHT,
	LEFT
};

inline Direction vectorToDirection(glm::vec2 direction) {
	if (direction.x > 0)
		return Direction::RIGHT;
	return Direction::LEFT;
}
