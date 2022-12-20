#pragma once

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <SDL_events.h>
#include "SpriteAnimationComponent.hpp"
#include <map>
#include <Wizard.hpp>
#include <IEnemyController.hpp>
using std::map;

enum class CharacterType{
	Wizard = 0,
	FireWizard,
	IceWizard,
	Wraith,
	BrownWraith,
	PurpleWraith,
	Guy,
	Robot,
	Zombie,
	Slime,
	Ninja,
	SamuraiHeavy,
//	SamurayLight,		fix atlas, it is not pot2
// TODO: Need to implement multiple attacks animations before add
//	BigTroll
//  Witch
};

const std::map<CharacterType, char*> CharacterTypeToString{
	{CharacterType::Wizard,			"Wizard"		},
	{CharacterType::FireWizard,		"FireWizard"	},
	{CharacterType::IceWizard,		"IceWizard"		},
	{CharacterType::Wraith,			"Wraith"		},
	{CharacterType::BrownWraith,	"BrownWraith"	},
	{CharacterType::PurpleWraith,	"PurpleWraith"	},
	{CharacterType::Guy,			"Guy"			},
	{CharacterType::Robot,			"Robot"			},
	{CharacterType::Zombie,			"Zombie"		},
	{CharacterType::Slime,			"Slime"			},
	{CharacterType::Ninja,			"Ninja"			},
	{CharacterType::SamuraiHeavy,	"SamuraiHeavy"	},
//	{CharacterType::SamurayLight,	"SamurayLight"	},
// Need to implement multiple attacks animations before add
//	{CharacterType::BigTroll,		"BigTroll"		},
//	{CharacterType::Witch,			"Witch"			}

};

#define DEFAULT_BEHAVIOUR Wizard

// Searching for the CharacterType it will return a function that links the right behaviour to the gameObject passed and return an EnemyController shared pointer
const std::map<CharacterType, std::function<std::shared_ptr<IEnemyController>(GameObject*)>> CharacterTypeToBehaviour {
	{CharacterType::Wizard,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::FireWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::IceWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Wraith,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::BrownWraith,	[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::PurpleWraith,	[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Guy,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Robot,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Zombie,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Slime,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	//	{CharacterType::BigTroll,		"BigTroll"		}

};



struct Controls {
	SDL_Keycode up = SDLK_w;
	SDL_Keycode down = SDLK_s;
	SDL_Keycode left = SDLK_a;
	SDL_Keycode right = SDLK_d;

	SDL_Keycode shot = SDLK_SPACE;

	SDL_Keycode shootUp = SDLK_UP;
	SDL_Keycode shootDown = SDLK_DOWN;
	SDL_Keycode shootLeft = SDLK_LEFT;
	SDL_Keycode shootRight = SDLK_RIGHT;
};

struct PlayerSettings{
	CharacterType type = CharacterType::Wraith;
	std::string name = "player";
	glm::vec2 position;
	float speed = 5.0f;
	Controls keybinds;

	float hp = 5.0f;
	float armor = 0;
	float damage = 1.5f;
	float range = 10.0f;
	float rateOfFire = 5.0f;
	float shotSpeed = 10.0f;
	float knockback = 0;

};

struct EnemySettings {
	CharacterType type = CharacterType::Wizard;
	std::string name = "enemy";
	glm::vec2 position;
	float speed = 2.0f;

	float idealDistance = 6.0f; // meters 
	float hp = 5.0f;
	float armor = 0;
	float damage = 1.5f;
	float range = 6.0f;
	float rateOfFire = 1.0f;
	float shotSpeed = 5.0f;
	float knockback = 0;

	std::shared_ptr<GameObject> player;
};

class CharacterBuilder {
public:
	static std::shared_ptr<GameObject> createPlayer(PlayerSettings settings);
	static std::shared_ptr<GameObject> createEnemy(EnemySettings settings);

private:

	static map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> atlasMap;
	static map<CharacterType, map<State, int>> animationSizesMap;

	static void initAtlasMap();
	static void initSizesMap(CharacterType type);

	static map<State, int> getAnimationSizes(CharacterType type);
	static std::shared_ptr<sre::SpriteAtlas> getAtlas(CharacterType type);

	static void animationSetup(std::shared_ptr<SpriteAnimationComponent> animation, std::shared_ptr<sre::SpriteAtlas> spriteAtlas,
		std::map<State, int> AnimationSizes, float baseAnimTime, Depth visualDepth);
};

