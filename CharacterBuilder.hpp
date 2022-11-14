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
};

struct PlayerSettings{
	glm::vec2 position;
	float speed = 5.0f;
	Controls keybinds;
};

class CharacterBuilder {
public:
	static std::shared_ptr<GameObject> createPlayer(PlayerSettings settings);

private:
	static std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWraith() {
		static std::shared_ptr<sre::SpriteAtlas> WraithAtlas = sre::SpriteAtlas::create("Sprites/Wraith/Wraith_base_atlas.json", "Sprites/Wraith/Wraith_base_atlas.png");
		return WraithAtlas;
	};
};