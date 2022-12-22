#pragma once

#include "Component.hpp"
#include "RoomBuilder.hpp"
#include "Level.hpp"


class DoorComponent : public Component {
public:
    explicit DoorComponent(GameObject *gameObject);
    
    void update(float deltaTime) override;

    void onCollisionStart(PhysicsComponent* comp) override;
    

    bool load = false;
    bool locked;
    int destinationRoomId;
    Door door;
    Level* level;
private:
};

