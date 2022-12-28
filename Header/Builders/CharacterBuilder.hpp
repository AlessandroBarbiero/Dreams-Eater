#pragma once

#include "SpriteComponent.hpp"
#include "GameObject.hpp"
#include "sre/SpriteAtlas.hpp"
#include <SDL_events.h>
#include "SpriteAnimationComponent.hpp"
#include <CharacterType.hpp>
#include <unordered_map>
using std::unordered_map;

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

	float hp = 5.0f;
	float armor = 0;
	float damage = 1.5f;
	float range = 6.0f;
	float rateOfFire = 1.0f;
	float shotSpeed = 5.0f;
	float knockback = 0;
	float scale = 0.9f;

	std::shared_ptr<GameObject> player;
};

class CharacterBuilder {
public:
	static std::shared_ptr<GameObject> createPlayer(PlayerSettings settings);
	static std::shared_ptr<GameObject> createEnemy(EnemySettings settings);
	static void transform(GameObject* character, CharacterType newType);

	static std::vector<sre::Sprite> getAnimationVector(std::string spriteName, int animationSize,
		std::shared_ptr<sre::SpriteAtlas> spriteAtlas, Depth visualDepth);

private:

	static unordered_map<CharacterType, std::shared_ptr<sre::SpriteAtlas>> atlasMap;
	static unordered_map<CharacterType, unordered_map<State, int>> animationSizesMap;
	static std::shared_ptr<sre::SpriteAtlas> attackAtlas;

	static void initAtlasMap();
	static void initAtlasMap(CharacterType type);
	static void initSizesMap(CharacterType type);

	static unordered_map<State, int> getAnimationSizes(CharacterType type);
	static std::shared_ptr<sre::SpriteAtlas> getAtlas(CharacterType type);
	static std::shared_ptr<sre::SpriteAtlas> getAttackAtlas();

	static void animationSetup(std::shared_ptr<SpriteAnimationComponent> animation, std::shared_ptr<sre::SpriteAtlas> spriteAtlas,
		std::unordered_map<State, int> AnimationSizes, float baseAnimTime, Depth visualDepth);
};

