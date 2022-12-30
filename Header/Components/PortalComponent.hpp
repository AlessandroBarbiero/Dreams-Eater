#pragma once

#include "Component.hpp"
#include "RoomBuilder.hpp"
#include "Level.hpp"
#include "LevelBuilder.hpp"


class PortalComponent : public Component {
public:
    explicit PortalComponent(GameObject *gameObject);
    
    void onCollisionStart(PhysicsComponent* comp) override;

    LevelSettings nextLevelSettings;
    Level* level;
private:
};

