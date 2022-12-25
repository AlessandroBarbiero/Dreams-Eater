//
// Created by Morten Nobel-Jørgensen on 10/10/2017.
//

#include "DoorComponent.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include <iostream>

DoorComponent::DoorComponent(GameObject *gameObject) : Component(gameObject) {}

void DoorComponent::update(float deltaTime) {
	if (load) {
		level->loadRoom(destinationRoomId, door.destinationPosition);
	}
}

void DoorComponent::onCollisionStart(PhysicsComponent* comp) {
	if (comp->getGameObject()->tag == Tag::Player) {
		if (!locked) {
			std::cout << "Entered door to room" << destinationRoomId << std::endl;
			load = true;
		}
		else {
			std::cout << "Touched locked door to room" << destinationRoomId << std::endl;
		}
	}
}
