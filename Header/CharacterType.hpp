#pragma once 
#include <unordered_map>

#include <GameObject.hpp>
#include <string>
#include <functional>
#include <IEnemyController.hpp>
#include <Wizard.hpp>
#include <BigTroll.hpp>
#include <EnemyBehaviours/Witch.hpp>
#include <SimpleMelee.hpp>

using std::map;

enum class CharacterType {
	Wizard = 0,
	FireWizard,
	IceWizard,
	Wraith,
	BrownWraith,
	PurpleWraith,
	Slime,
	Witch

	//These Characters are ready to be added but not currently used in the game
	/*Ninja,
	SamuraiHeavy,
	SamuraiLight,
	BigTroll,*/

};

const std::unordered_map<CharacterType, char*> CharacterTypeToString{
	{CharacterType::Wizard,			"Wizard"		},
	{CharacterType::FireWizard,		"FireWizard"	},
	{CharacterType::IceWizard,		"IceWizard"		},
	{CharacterType::Wraith,			"Wraith"		},
	{CharacterType::BrownWraith,	"BrownWraith"	},
	{CharacterType::PurpleWraith,	"PurpleWraith"	},
	{CharacterType::Slime,			"Slime"			},
	{CharacterType::Witch,			"Witch"			}

	/*{CharacterType::Ninja,			"Ninja"			},
	{CharacterType::SamuraiHeavy,	"SamuraiHeavy"	},
	{CharacterType::SamuraiLight,	"SamuraiLight"	},
	{CharacterType::BigTroll,		"BigTroll"		},*/


};

const std::unordered_map <std::string, CharacterType> CharacterStringToType{
	{"Wizard",			CharacterType::Wizard		},
	{"FireWizard",		CharacterType::FireWizard	},
	{"IceWizard",		CharacterType::IceWizard		},
	{"Wraith",			CharacterType::Wraith		},
	{"BrownWraith",		CharacterType::BrownWraith	},
	{"PurpleWraith",	CharacterType::PurpleWraith	},
	{"Slime",			CharacterType::Slime			},
	{"Witch",			CharacterType::Witch			}
	/*{"Ninja",			CharacterType::Ninja			},
	{"SamuraiHeavy",	CharacterType::SamuraiHeavy	},
	{"SamuraiLight",	CharacterType::SamuraiLight	},
	{"BigTroll",		CharacterType::BigTroll		},*/

};

#define DEFAULT_BEHAVIOUR SimpleMelee

// Searching for the CharacterType it will return a function that links the right behaviour to the gameObject passed and return an EnemyController shared pointer
const std::unordered_map<CharacterType, std::function<std::shared_ptr<IEnemyController>(GameObject*)>> CharacterTypeToBehaviour{
	{CharacterType::Wizard,			[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::FireWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::IceWizard,		[](GameObject* obj) {	return obj->addComponent<Wizard>();	}	},
	{CharacterType::Witch,			[](GameObject* obj) {	return obj->addComponent<Witch>();	}	},
	/*{CharacterType::BigTroll,		[](GameObject* obj) {	return obj->addComponent<BigTroll>();	}	},*/

};

