#pragma once

#include "Component.hpp"
#include "BulletComponent.hpp"
#include <queue>
#include "SDL.h"
#include <CharacterType.hpp>
#include <State.hpp>

constexpr auto KNOCKBACK_TIME = 0.2;

// Forward declaration
class SpriteAnimationComponent;

class CharacterComponent : public Component {
public:
	explicit CharacterComponent(GameObject* gameObject);

	void onCollisionStart(PhysicsComponent* comp) override;
	// void onCollisionEnd(PhysicsComponent* comp) override;

	// bool onKey(SDL_Event& event) override;

	void onGui() override;
	
	void update(float deltaTime) override;
	void resetKeys() override;

	void shoot(glm::vec2 direction, const sre::Sprite& bulletSprite);

	void stunned(float stunTimeout);
	void stunned(bool stun);

	State getState();
	void changeState(State newState);
	Direction getDirection();
	void setDirection(Direction newFacingDirection);

	void showEffect(State effect, float animTime = -1);

	float getHp()			{ return hp; };
	float getArmor()		{ return armor ; };
	float getSpeed()		{ return speed ; };
	float getDamage()		{ return damage; };
	float getRange()		{ return range ; };
	float getRateOfFire()	{ return rateOfFire; };
	float getShotSpeed()	{ return shotSpeed ; };
	float getKnockback()	{ return knockback ; };


private:

	static std::shared_ptr<sre::SpriteAtlas> effectAtlas;

	void updateStunTimeout(float deltaTime);
	void checkRateOfFire(float deltaTime);
	void updateFlyingProj();

	void initSpecialEffectObject();
	std::shared_ptr<SpriteAnimationComponent> specialEffects;

	void inflictDamage(float damage);
	void applyKnockback(std::shared_ptr<BulletComponent> bullet);

	CharacterType type;
	State state = State::Idle;
	Direction facingDirection = Direction::RIGHT;


	// If set to true the character cannot move
	bool stun = false;
	float stunTimeout = 0;

	float hp		= 5.0f;
	float armor		= 0;
	float speed		= 5.0f;
	float damage	= 1.5f;
	float range		= 10.0f;
	// shots per second
	float rateOfFire = 3.0f;    
	float shotSpeed = 10.0f;
	float knockback = 0;

	// The radius of the physic component -> it is also the distance from which the bullet is spawned
	float radius; 

	float shotCooldownTimer = 0;
	bool readyToShoot = true;

	std::queue<std::weak_ptr<BulletComponent>> flyingProj;

	void startShotCooldown();
	void die();

	// If this variable is set to true you can control the shooting of this character using passed keybind
	bool useShootingKeys = false;   
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	SDL_Keycode keyShootUp;
	SDL_Keycode keyShootDown;
	SDL_Keycode keyShootLeft;
	SDL_Keycode keyShootRight;



	//==============GUI==============

	std::shared_ptr<sre::Texture> heartFullTexture;
	std::shared_ptr<sre::Texture> heartEmptyTexture;

	void setPlayerGui();
	//void setEnemyGui();

	//ImVec2 guiSize = { 180, 50 };
	ImVec2 guiPivot = { 0,0 };
	ImFont* font;

	int flags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar;


	//==============================

	friend class CharacterBuilder;
	friend class PlayerController;
	friend class IEnemyController;
	friend class Powerup;
	friend class PowerupBuilder;
	friend class Level;
};

// Cannot include it at the beginning of the file because they are mutually referenciate each other
#include <SpriteAnimationComponent.hpp>