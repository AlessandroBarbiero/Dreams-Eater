#pragma once

#include <unordered_map>
#include <glm/glm.hpp>


enum class State {
	Idle,
	IdleBlink,
	Run,
	Walk,
	Attack,
	Attack1,
	Attack2,
	Attack3,
	Hurt,
	Die,
	Hit,
	Victory,
	Item,
	Charge,
};

const std::unordered_map<State, const char*> StateToString{
	{State::Idle,			"Idle"		},
	{State::IdleBlink,		"IdleBlink"	},
	{State::Run,			"Run"		},
	{State::Walk,			"Walk"		},
	{State::Attack,			"Attack"	},
	{State::Attack1,		"Attack1"	},
	{State::Attack2,		"Attack2"	},
	{State::Attack3,		"Attack3"	},
	{State::Hurt,			"Hurt"		},
	{State::Die,			"Die"		},
	{State::Hit,			"Hit"		},
	{State::Victory,		"Victory"	},
	{State::Item,			"Item"		},
	{State::Charge,			"Charge"		},
};

const std::unordered_map<std::string, State> StringToState{
	{"Idle"		,		State::Idle				},
	{"IdleBlink",		State::IdleBlink		},
	{"Run"		,		State::Run				},
	{"Walk"		,		State::Walk				},
	{"Attack"	,		State::Attack			},
	{"Attack1"	,		State::Attack1			},
	{"Attack2"	,		State::Attack2			},
	{"Attack3"	,		State::Attack3			},
	{"Hurt"		,		State::Hurt				},
	{"Die"		,		State::Die				},
	{"Hit"		,		State::Hit				},
	{"Victory"	,		State::Victory			},
	{"Item"		,		State::Item				},
	{"Charge"	,		State::Charge			},
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
