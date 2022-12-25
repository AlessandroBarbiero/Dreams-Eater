#pragma once 
#include <unordered_map>

#include <GameObject.hpp>
#include <string>
#include <functional>
#include <IEnemyController.hpp>
#include <Wizard.hpp>
#include <BigTroll.hpp>
#include <EnemyBehaviours/Witch.hpp>

using std::map;

enum class CharacterType {
	Wizard = 0,
	FireWizard,
	IceWizard,
	Wraith,
	BrownWraith,
	PurpleWraith,
	Slime,
	Ninja,
	SamuraiHeavy,
	SamuraiLight,
	BigTroll,
	Witch
};

const std::unordered_map<CharacterType, char*> CharacterTypeToString{
	{CharacterType::Wizard,			"Wizard"		},
	{CharacterType::FireWizard,		"FireWizard"	},
	{CharacterType::IceWizard,		"IceWizard"		},
	{CharacterType::Wraith,			"Wraith"		},
	{CharacterType::BrownWraith,	"BrownWraith"	},
	{CharacterType::PurpleWraith,	"PurpleWraith"	},
	{CharacterType::Slime,			"Slime"			},
	{CharacterType::Ninja,			"Ninja"			},
	{CharacterType::SamuraiHeavy,	"SamuraiHeavy"	},
	{CharacterType::SamuraiLight,	"SamuraiLight"	},
	{CharacterType::BigTroll,		"BigTroll"		},
	{CharacterType::Witch,			"Witch"			}

};

#define DEFAULT_BEHAVIOUR Wizard

// Searching for the CharacterType it will return a function that links the right behaviour to the gameObject passed and return an EnemyController shared pointer
const std::unordered_map<CharacterType, std::function<std::shared_ptr<IEnemyController>(GameObject*)>> CharacterTypeToBehaviour{
	{CharacterType::Wizard,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::FireWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::IceWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Witch,			[](GameObject* obj) {	return obj->addComponent<Witch>();	}	},
	{CharacterType::BigTroll,		[](GameObject* obj) {	return obj->addComponent<BigTroll>();	}	},

};

