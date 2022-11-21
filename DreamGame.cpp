#include "DreamInspector.hpp"
#include <iostream>
#include "DreamGame.hpp"
#include "GameObject.hpp"
#include "sre/RenderPass.hpp"
#include "SpriteComponent.hpp"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "PhysicsComponent.hpp"
#include "RoomComponent.hpp"
#include "PlayerController.hpp"
#include "CharacterBuilder.hpp"

using namespace std;
using namespace sre;

const glm::vec2 DreamGame::windowSize(600, 600);

DreamGame* DreamGame::instance = nullptr;

DreamGame::DreamGame()
    :debugDraw(physicsScale)
{
    instance = this;
    r.setWindowSize(windowSize);
    r.init()
        .withSdlInitFlags(SDL_INIT_EVERYTHING)
        .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    init();

    // setup callback functions
    r.keyEvent = [&](SDL_Event& e) {
        onKey(e);
    };
    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };
    r.frameRender = [&]() {
        render();
    };
    // start game loop
    r.startEventLoop();
}

void DreamGame::init() {
    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }
    camera.reset();
    sceneObjects.clear();
    camera.reset();
    physicsComponentLookup.clear();
    initPhysics();

    spriteAtlas_inside = SpriteAtlas::create("Sprites/Room/Inside_atlas.json", "Sprites/Room/Inside_atlas.png");    
    
    auto camObj = createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);

    // Initialize

    //player
    PlayerSettings pSettings;
    pSettings.position = glm::vec2(3, 3);
    pSettings.speed = 8.0f;
    auto player = CharacterBuilder::createPlayer(pSettings);

    // Second player creation with different keybind
    //pSettings.name = "player2";
    //pSettings.position = glm::vec2(10,10);
    //pSettings.keybinds = { SDLK_w, SDLK_s, SDLK_a, SDLK_d };
    //auto player2 = CharacterBuilder::createPlayer(pSettings);

    EnemySettings eSettings;
    eSettings.position = glm::vec2(15, 15);
    eSettings.player = player;
    eSettings.speed = 2.0f;
    CharacterBuilder::createEnemy(eSettings);

    // Test room
    auto testRoom = createGameObject();
    testRoom->name = "testRoom";
    auto roomPhys = testRoom->addComponent<PhysicsComponent>();
    auto room = testRoom->addComponent<RoomComponent>();
    room->setRoomSize(glm::vec2(8, 8));
    room->buildFloor();
    room->buildWalls();
    camera->setFollowObject(testRoom, glm::vec2(0, 0));
    
}

void DreamGame::gameOver() {
    gameState = GameState::GameOver;
    std::cout << "Game Over!\nPress SPACE to restart" << std::endl;
}

void DreamGame::update(float time) {
    if (gameState == GameState::Running) {
        updatePhysics();

        for (int i = 0; i < sceneObjects.size(); i++) {
            sceneObjects[i]->update(time);
        }
        // Remove elements marked for deletion
        auto toErase = std::remove_if(sceneObjects.begin(), sceneObjects.end(), [](std::shared_ptr<GameObject> x) {return x->destroyed; });
        sceneObjects.erase(toErase, sceneObjects.end());
    }
}

void DreamGame::render() {

    auto rp = RenderPass::create()
        .withCamera(camera->getCamera())
        .build();

    auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto& go : sceneObjects) {
        go->renderSprite(spriteBatchBuilder);
    }

    // render gui
    for (auto& go : sceneObjects) {
        for (auto& comp : go->getComponents()) {
            comp->onGui();
        }
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if (doDebugDraw) {
        
        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();

        
        DreamInspector::instance->updateSceneObjectsSize(sceneObjects.size());
    }

    


    
}

void DreamGame::onKey(SDL_Event& event) {
    if (gameState == GameState::Running) {
        for (auto& gameObject : sceneObjects) {
            for (auto& c : gameObject->getComponents()) {
                bool consumed = c->onKey(event);
                if (consumed) {
                    return;
                }
            }
        }
    }

    // General Keys

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_SPACE:
            if (gameState == GameState::GameOver) {
                gameState = GameState::Running;
                init();
            }
            break;
        case SDLK_r:
            init();
            break;
        case SDLK_t:
            for each (std::shared_ptr<GameObject> obj in sceneObjects)
            {
                if (obj->name == "testRoom") {
                    std::cout << "Marking testRoom" << std::endl;
                    obj->destroy();
                    camera->setFollowObject(nullptr, glm::vec2(0,0));
                }
            }
            break;
        case SDLK_y:
            // press 'd' for physics debug
            doDebugDraw = !doDebugDraw;
            if (doDebugDraw) {
                world->SetDebugDraw(&debugDraw);
            }
            else {
                world->SetDebugDraw(nullptr);
            }
            break;
        }

    }
}

std::shared_ptr<GameObject> DreamGame::createGameObject() {
    auto obj = shared_ptr<GameObject>(new GameObject());
    sceneObjects.push_back(obj);
    return obj;
}

void DreamGame::updatePhysics() {

    const int positionIterations = 4;
    const int velocityIterations = 12;
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto phys : physicsComponentLookup) {
        PhysicsComponent* physicsComponent = phys.second;
        if (physicsComponent->isAutoUpdate() == false) continue;
        auto position = physicsComponent->getBody()->GetPosition();
        float angle = physicsComponent->getBody()->GetAngle();
        auto gameObject = physicsComponent->getGameObject();
        gameObject->setPosition(glm::vec2(position.x * physicsScale, position.y * physicsScale));
        gameObject->setRotation(angle);
    }
}

void DreamGame::initPhysics() {
    float gravity = 0; // Top-down
    delete world;
    world = new b2World(b2Vec2(0, gravity));
    world->SetContactListener(this);
}

void DreamGame::BeginContact(b2Contact* contact) {
    b2ContactListener::BeginContact(contact);
    handleContact(contact, true);
}

void DreamGame::EndContact(b2Contact* contact) {
    b2ContactListener::EndContact(contact);
    handleContact(contact, false);
}

void DreamGame::deregisterPhysicsComponent(PhysicsComponent* r) {
    auto iter = physicsComponentLookup.find(r->getFixture());
    if (iter != physicsComponentLookup.end()) {
        physicsComponentLookup.erase(iter);
    }
    else {
        assert(false); // cannot find physics object
    }
}

void DreamGame::registerPhysicsComponent(PhysicsComponent* r) {
    physicsComponentLookup[r->getFixture()] = r;
}

void DreamGame::handleContact(b2Contact* contact, bool begin) {
    auto fixA = contact->GetFixtureA();
    auto fixB = contact->GetFixtureB();
    auto physA = physicsComponentLookup.find(fixA);
    auto physB = physicsComponentLookup.find(fixB);
    if (physA != physicsComponentLookup.end() && physB != physicsComponentLookup.end()) {
        auto& aComponents = physA->second->getGameObject()->getComponents();
        auto& bComponents = physB->second->getGameObject()->getComponents();
        for (auto& c : aComponents) {
            if (begin) {
                c->onCollisionStart(physB->second);
            }
            else {
                c->onCollisionEnd(physB->second);
            }
        }
        for (auto& c : bComponents) {
            if (begin) {
                c->onCollisionStart(physA->second);
            }
            else {
                c->onCollisionEnd(physA->second);
            }
        }
    }
}