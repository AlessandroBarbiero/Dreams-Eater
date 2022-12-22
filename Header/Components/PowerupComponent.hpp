#pragma once

#include "Component.hpp"
#include "Powerup.hpp"

// Component holding different powerups -> attach this to objects that can store powerups, powerups in this component are not always activated on the object holding the component
// Both the player and the powerup object can use this component
class PowerupComponent : public Component {
public:
    explicit PowerupComponent(GameObject* gameObject);

    void onCollisionStart(PhysicsComponent* comp) override;

    void addPowerup(std::shared_ptr<Powerup> powerup);
    void addPowerups(std::vector<std::shared_ptr<Powerup>> powerupsToAdd);

    void applyPowerups(GameObject* player);
    std::vector<std::shared_ptr<Powerup>> getPowerups();

private:
    std::vector<std::shared_ptr<Powerup>> powerups;
};