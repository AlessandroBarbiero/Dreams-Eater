#include "PowerupComponent.hpp"
#include "PhysicsComponent.hpp"
#include "CharacterComponent.hpp"

PowerupComponent::PowerupComponent(GameObject* gameObject) : Component(gameObject) {}

void PowerupComponent::onCollisionStart(PhysicsComponent* comp)
{
    if (comp->getGameObject()->tag == Tag::Player) {
        GameObject* player = comp->getGameObject();
        // activate the powerups
        applyPowerups(player);
        // Save the new powerups inside the player component
        player->getComponent<PowerupComponent>()->addPowerups(powerups);
        player->getComponent<CharacterComponent>()->showEffect(State::Item, 0.2f);
        gameObject->destroy();
    }
}


void PowerupComponent::addPowerup(std::shared_ptr<Powerup> powerup)
{
	powerups.push_back(powerup);
}

void PowerupComponent::addPowerups(std::vector<std::shared_ptr<Powerup>> powerupsToAdd)
{
    powerups.insert(powerups.end(), powerupsToAdd.begin(), powerupsToAdd.end());
}

void PowerupComponent::applyPowerups(GameObject* player) {
    for (const auto& powerup : powerups) {
        powerup->apply(player);
    }
}

std::vector<std::shared_ptr<Powerup>> PowerupComponent::getPowerups()
{
    return powerups;
}
