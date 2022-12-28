#pragma once

#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <unordered_map>
#include <Powerup.hpp>
using std::map;

// Each Powerup Type has his own implementation passed in the powerupMap
enum class PowerupType {
	Earth,
	Fire,
	Ice,
	Water
};


class PowerupBuilder {

public:

	static PowerupBuilder* getInstance();

	std::shared_ptr<GameObject> createSinglePowerupObject(PowerupType type, glm::vec2 position);

	std::string firePowerupMessage  = "Damage +3";
	std::string waterPowerupMessage = "Speed +3\nArmor + 0.5\nRateOfFire + 0.5";
	std::string icePowerupMessage   = "Health +10\nSpeed -0.5";
	std::string earthPowerupMessage = "Transform into Earth Wraith";
	//std::string loremIpsum = "ccccccccccccc\ncccccccccccccccccc\ncccccccccccccccc";
	

	const std::unordered_map<PowerupType, std::shared_ptr<Powerup>> PowerupMap {

		//Other version using lambda function, but I think it's cleaner to use it with static functions written somewhere else
		// {PowerupType::Fire,	 
		//	std::make_shared<Powerup>("Fire", [](GameObject* player) { player->getComponent<CharacterComponent>()->damage += 3; }, getPowerupAtlas()->get("badges/fire.png"))},

		{PowerupType::Fire,		std::make_shared<Powerup>("Fire", 	  firePowerupMessage,      &PowerupBuilder::firePowerup,		getPowerupAtlas()->get("badges/fire.png"))},
		{PowerupType::Water,	std::make_shared<Powerup>("Water",	  waterPowerupMessage,     &PowerupBuilder::waterPowerup,		getPowerupAtlas()->get("badges/water-drop.png"))},
		{PowerupType::Ice,		std::make_shared<Powerup>("Ice",	  icePowerupMessage,       &PowerupBuilder::icePowerup,		    getPowerupAtlas()->get("badges/ice.png"))},
		{PowerupType::Earth,	std::make_shared<Powerup>("Earth",	  earthPowerupMessage,     &PowerupBuilder::earthPowerup,		getPowerupAtlas()->get("badges/earth.png"))},
	};



private:
	PowerupBuilder();
	
	static void firePowerup(GameObject* player);
	static void waterPowerup(GameObject* player);
	static void icePowerup(GameObject* player);
	static void earthPowerup(GameObject* player);

	std::shared_ptr<sre::SpriteAtlas> powerupAtlas;
	std::shared_ptr<sre::SpriteAtlas> getPowerupAtlas();


	

};