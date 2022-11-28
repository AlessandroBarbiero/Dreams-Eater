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
#include "LevelBuilder.hpp"

using namespace std;
using namespace sre;

const glm::vec2 DreamGame::windowSize(1200, 700);

DreamGame* DreamGame::instance = nullptr;

DreamGame::DreamGame()
    :debugDraw(physicsScale),currentScene(&startMenu)
{
    instance = this;
    r.setWindowSize(windowSize);
    r.init()
        .withSdlInitFlags(SDL_INIT_EVERYTHING)
        .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    spriteAtlas_inside = SpriteAtlas::create("Sprites/Room/Inside_atlas.json", "Sprites/Room/Inside_atlas.png");

    time_t t;   // random seed based on time
    srand((unsigned)time(&t));

    init();
    buildMenus();

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

void DreamGame::buildMenus() {
    auto start = startMenu.createGameObject();
    start->addComponent<StartMenuComponent>();

    auto end = endMenu.createGameObject();
    end->addComponent<EndMenuComponent>();
}

void DreamGame::init() {

    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }

    camera.reset();
    game.cleanSceneObjects();
    camera.reset();
    physicsComponentLookup.clear();
    initPhysics();

    auto camObj = game.createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);

    gameState = GameState::Running;
}

void DreamGame::play() {

    currentScene = &game;

    PlayerSettings pSettings;
    pSettings.position = glm::vec2(3, 3);
    pSettings.speed = 8.0f;
    pSettings.knockback = 1.0f;
    auto player = CharacterBuilder::createPlayer(pSettings);

    /*
    EnemySettings eSettings;
    eSettings.position = glm::vec2(10, 10);
    eSettings.player = player;
    eSettings.speed = 2.0f;
    eSettings.knockback = 1.0f;
    CharacterBuilder::createEnemy(eSettings);
    */

    // Test room
    /*
    auto testRoom = createGameObject();
    testRoom->name = "testRoom";
    auto roomPhys = testRoom->addComponent<PhysicsComponent>();
    auto room = testRoom->addComponent<RoomComponent>();
    room->setRoomSize(glm::vec2(8, 8));
    room->buildFloor();
    room->buildWalls();
    */
    RoomSettings rSettings;
    rSettings.name = "TestRoom";
    rSettings.position = { 0,0 };
    rSettings.size = { 7,7 };


    /*rSettings.tileSetFloor = BricksFloor;
    rSettings.tileSetWalls = BricksWalls;
    rSettings.tileSetFloor = ShogiFloor;
    rSettings.tileSetWalls = ShogiWalls;
    rSettings.tileSetFloor = StoneFloor;
    rSettings.tileSetWalls = StoneWalls;
    rSettings.tileSetFloor = LightWoodFloor;
    rSettings.tileSetWalls = LightWoodWalls;*/


    rSettings.tileSetFloor = WoodFloor;
    rSettings.tileSetWalls = WoodWalls;

    rSettings.roomType = BossRoom;

    rSettings.doors.push_back(Door{ false, Top });
    rSettings.doors.push_back(Door{ false, Bottom });
    rSettings.doors.push_back(Door{ false, Left });
    rSettings.doors.push_back(Door{ false, Right });
    //auto testRoom = RoomBuilder::createRoom(rSettings);

    auto testLevel = Level();

    testLevel.name = "test";
    testLevel.difficulty = 1;
    testLevel.player = player;
    testLevel.roomSettings.push_back(rSettings);
    testLevel.roomEntered.push_back(false);
    testLevel.rooms = 1;
    testLevel.startRoom = 0;

    testLevel.loadRoom(0);

    camera->setFollowObject(testLevel.currentRoom, glm::vec2(0, 0));
    // Fit room width to window

    camera->getCamera().setOrthographicProjection(testLevel.currentRoom->getComponent<RoomComponent>()->getRoomSizeInPixels().x / 2.0f, -1, 1);

}



void DreamGame::gameOver() {
    gameState = GameState::GameOver;
    //std::cout << "Game Over!\nPress SPACE to restart" << std::endl;
    currentScene = &endMenu;
}

void DreamGame::update(float time) {
    if (gameState == GameState::Running) {
        updatePhysics();

        auto sceneObjects = currentScene->getSceneObjects();

        for (int i = 0; i < sceneObjects->size(); i++) {
            sceneObjects->at(i)->update(time);
        }

        // Remove elements marked for deletion
        auto toErase = std::remove_if(sceneObjects->begin(), sceneObjects->end(), [](std::shared_ptr<GameObject> x) {return x->destroyed; });
        sceneObjects->erase(toErase, sceneObjects->end());

        //currentScene->setSceneObjects(sceneObjects); //there must be a better way
    }
}

void DreamGame::render() {

    auto rp = RenderPass::create()
        .withCamera(camera->getCamera())
        .build();

    auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto& go : *(currentScene->getSceneObjects())) {
        go->renderSprite(spriteBatchBuilder);
    }

    // render gui
    for (auto& go : *(currentScene->getSceneObjects())) {
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

        
        DreamInspector::instance->updateSceneObjectsSize(currentScene->getSceneObjects()->size());
    }
    
}

void DreamGame::onKey(SDL_Event& event) {
    if (gameState == GameState::Running) {
        for (auto& gameObject : *(currentScene->getSceneObjects())) {
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
        /*case SDLK_SPACE:
            if (gameState == GameState::GameOver) {
                gameState = GameState::Running;
                init();
            }
            break;*/
        case SDLK_r:
            init();
            play();
            break;
        case SDLK_t:
            for each (std::shared_ptr<GameObject> obj in *(currentScene->getSceneObjects()))
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


std::shared_ptr<GameObject> DreamGame::reactivateGameObject(std::shared_ptr<GameObject> obj) {
    obj->destroyed = false;
    currentScene->getSceneObjects()->push_back(obj);
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