#pragma once

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <SDL_events.h>

struct Controls {
	SDL_Keycode up = SDLK_UP;
	SDL_Keycode down = SDLK_DOWN;
	SDL_Keycode left = SDLK_LEFT;
	SDL_Keycode right = SDLK_RIGHT;
	SDL_Keycode shot = SDLK_SPACE;
};

struct PlayerSettings{
	std::string name = "player";
	glm::vec2 position;
	float speed = 5.0f;
	float rateOfFire = 5.0f;
	float shotSpeed = 10.0f;
	Controls keybinds;
};

struct EnemySettings {
	std::string name = "enemy";
	glm::vec2 position;
	float speed = 2.0f;
	float idealDistance = 6.0f; // meters 
	float range = 5.0f;
	float rateOfFire = 1.0f;
	float shotSpeed = 5.0f;
	std::shared_ptr<GameObject> player;

};

class CharacterBuilder {
public:
	static std::shared_ptr<GameObject> createPlayer(PlayerSettings settings);
	static std::shared_ptr<GameObject> createEnemy(EnemySettings settings);

private:
	static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWraith() {
		static std::shared_ptr<sre::SpriteAtlas> WraithAtlas = sre::SpriteAtlas::create("Sprites/Wraith/Wraith_base_atlas.json", "Sprites/Wraith/Wraith_base_atlas.png");
		return WraithAtlas;
	};
	static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWizard() {
		static std::shared_ptr<sre::SpriteAtlas> WizardAtlas = sre::SpriteAtlas::create("Sprites/Wizard/Wizard_base_atlas.json", "Sprites/Wizard/Wizard_base_atlas.png");
		return WizardAtlas;
	};
};