//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "PortalComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "DreamGame.hpp"
#include <iostream>

PortalComponent::PortalComponent(GameObject *gameObject) : Component(gameObject) {}


void PortalComponent::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->tag == Tag::Player) {
		std::shared_ptr<Level> nextLevel = LevelBuilder::createLevel(nextLevelSettings);
		level->nextLevel = nextLevel;
		level->finished = true;
		level = nullptr;
		getGameObject()->destroy();
	}
}
