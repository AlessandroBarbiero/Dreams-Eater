#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"

// Enumeration used to Place the sprites in the right depth one over the other on the scene
// Use this like sprite.setOrderInBatch(Depth::something);
enum Depth : uint16_t {
    Floor,
    Wall,
    Object,
    Bullet,
    Enemy,
    Player,
    Effect
};

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(GameObject *gameObject);

    void renderSprite(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder) override;

    void setSprite(const sre::Sprite& sprite);
    sre::Sprite getSprite();

    void activate();
    void deactivate();
private:
    sre::Sprite sprite;
    bool noRender = false;
};

