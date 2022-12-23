#pragma once 
#include <map>

#include <GameObject.hpp>
#include <string>
#include <functional>
#include <IEnemyController.hpp>
#include <Wizard.hpp>

using std::map;

enum class CharacterType {
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
	SamurayLight,
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
	{CharacterType::SamurayLight,	"SamurayLight"	},
	// Need to implement multiple attacks animations before add
	//	{CharacterType::BigTroll,		"BigTroll"		},
	//	{CharacterType::Witch,			"Witch"			}

};

#define DEFAULT_BEHAVIOUR Wizard

// Searching for the CharacterType it will return a function that links the right behaviour to the gameObject passed and return an EnemyController shared pointer
const std::map<CharacterType, std::function<std::shared_ptr<IEnemyController>(GameObject*)>> CharacterTypeToBehaviour{
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

