#pragma once

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <SDL_events.h>

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
	static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWraith() {
		static std::shared_ptr<sre::SpriteAtlas> WraithAtlas = sre::SpriteAtlas::create("Sprites/Wraith/Wraith_base_atlas.json", "Sprites/Wraith/Wraith_base_atlas.png");
		return WraithAtlas;
	};
	static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWizard() {
		static std::shared_ptr<sre::SpriteAtlas> WizardAtlas = sre::SpriteAtlas::create("Sprites/Wizard/Wizard_base_atlas.json", "Sprites/Wizard/Wizard_base_atlas.png");
		return WizardAtlas;
	};
};