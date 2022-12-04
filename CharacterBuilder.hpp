#pragma once

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <SDL_events.h>
#include "SpriteAnimationComponent.hpp"
#include <map>

enum class CharacterType{
	Wizard,
	FireWizard,
	IceWizard,
	Wraith,
	BrownWraith,
	PurpleWraith
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

	static std::map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> atlasMap;

	static void initMap();

	static std::shared_ptr<sre::SpriteAtlas> getAtlas(CharacterType);

	static void animationSetup(std::shared_ptr<SpriteAnimationComponent> animation, std::shared_ptr<sre::SpriteAtlas> spriteAtlas,
		std::map<State, int> AnimationSizes, float baseAnimTime, Depth visualDepth);
};

