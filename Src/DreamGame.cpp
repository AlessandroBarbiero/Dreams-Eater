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
#include <Builders/PowerupBuilder.hpp>
#include "StartMenuComponent.hpp"
#include "PauseMenuComponent.hpp"
#include "GuiHelper.hpp"
#include "LevelGuiComponent.hpp"


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
    spriteAtlas_baseWraith = SpriteAtlas::create("Sprites/Wraith/Wraith_purple_atlas.json", "Sprites/Wraith/Wraith_purple_atlas.png");

    time_t t;   // random seed based on time
    srand((unsigned)time(&t));

    init();
    buildStartMenu();


    deathEvent = SDL_RegisterEvents(1);
    playEvent = SDL_RegisterEvents(2);

    GuiHelper::getInstance()->setupImGuiStyle(GuiStyle::Dark);
    GuiHelper::getInstance()->setupFont();

    // setup callback functions
    r.keyEvent = [&](SDL_Event& e) {
        onKey(e);
    };

    r.otherEvent = [&](SDL_Event& e) {
        if (e.type == deathEvent) {
            init();
            currentScene = &startMenu;
        }
        else if (e.type == playEvent) {
            play();
        }
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

void DreamGame::buildStartMenu() {
    auto start = startMenu.createGameObject();
    start->addComponent<StartMenuComponent>();
    

    auto background = Texture::create()
        .withFile(GuiHelper::getInstance()->GUI_PATH + "BackgroundTitle.png")
        .withFilterSampling(false)
        .build();

    guiAtlas = SpriteAtlas::createSingleSprite(background, "background");

    auto backgroundSprite = guiAtlas->get("background");
    auto spriteComponent = start->addComponent<SpriteComponent>();
    spriteComponent->setSprite(backgroundSprite);
    

    /*auto end = endMenu.createGameObject();
    end->addComponent<EndMenuComponent>();*/
}

void DreamGame::init() {

    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }

    this->level.reset();
    camera.reset();
    game.cleanSceneObjects();
    camera.reset();
    physicsComponentLookup.clear();
    initPhysics();

    auto camObj = game.createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);
}

void DreamGame::play() {

    currentScene = &game;

    PlayerSettings pSettings;
    pSettings.position = glm::vec2(1,1);
    pSettings.speed = 15.0f;
    pSettings.knockback = 1.0f;
    pSettings.type = CharacterType::Wraith;
    pSettings.name = playerName;
    std::cout << "Creating player" << std::endl;
    auto player = CharacterBuilder::createPlayer(pSettings);

    // TODO: Remove me from here
    //PowerupBuilder* pBuilder = PowerupBuilder::getInstance();
    //pBuilder->createSinglePowerupObject(PowerupType::Fire,  { 5,5 });
    //pBuilder->createSinglePowerupObject(PowerupType::Water, { 0,5 });
    //pBuilder->createSinglePowerupObject(PowerupType::Earth, { 5,0 });
    //pBuilder->createSinglePowerupObject(PowerupType::Ice,   { 8,8 });

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

    std::shared_ptr<RoomSettings> rSettings = make_shared<RoomSettings>();
    rSettings->name = "TestRoom";
    rSettings->position = { 0,0 };
    rSettings->size = { 7,7 };
    */


    /*rSettings.tileSetFloor = BricksFloor;
    rSettings.tileSetWalls = BricksWalls;
    rSettings.tileSetFloor = ShogiFloor;
    rSettings.tileSetWalls = ShogiWalls;
    rSettings.tileSetFloor = StoneFloor;
    rSettings.tileSetWalls = StoneWalls;
    rSettings.tileSetFloor = LightWoodFloor;
    rSettings.tileSetWalls = LightWoodWalls;*/

    /*
    rSettings->tileSetFloor = WoodFloor;
    rSettings->tileSetWalls = WoodWalls;

    rSettings->roomType = EnemyRoom;

    rSettings->doors.push_back(Door{ false, Left, 1 });
    //auto testRoom = RoomBuilder::createRoom(rSettings);

    level = make_shared<Level>();

    level->name = "test";
    level->difficulty = 1;
    level->player = player;

    level->roomSettings.push_back(rSettings);
    level->roomEntered.push_back(false);
    level->roomObjects.push_back({});

    rSettings->name = "Room1";
    rSettings->roomType = BossRoom;
    rSettings->doors.clear();
    rSettings->doors.push_back(Door{ false, Right, 0 });
    level->roomSettings.push_back(rSettings);
    level->roomEntered.push_back(false);
    level->roomObjects.push_back({});

    level->rooms = 2;
    level->startRoom = 0;
    
    //level->loadRoom(0);
    //level->loadRoom(1);
    */

    auto levelGui = currentScene->createGameObject();
    auto guiComp = levelGui->addComponent<LevelGuiComponent>();
    

    LevelSettings testLevelSettings;
    testLevelSettings.difficulty = difficulty > 0 ? difficulty : 1;
    testLevelSettings.name = "TestLevel";
    testLevelSettings.rooms = 10;
    std::cout << "Creating level" << std::endl;
    level = LevelBuilder::createLevel(testLevelSettings);

    guiComp->setLevel(level);
    //guiComp->setPlayer(player);

    level->player = player;
    level->loadLevel();
    //level->loadRoom(0, level->roomSettings[0]->doors[0].position);

    camera->setFollowObject(player, glm::vec2(0, 0));
    // Fit room width to window

    camera->getCamera().setOrthographicProjection(level->currentRoom->getComponent<RoomComponent>()->getRoomSizeInPixels().x / 2.0f, -1, 1);

    gameState = GameState::Running;

    

}

void DreamGame::startGame() {
    SDL_Event event;
    event.user.type = DreamGame::instance->playEvent;
    SDL_PushEvent(&event);

}

void DreamGame::gameOver() {
    gameState = GameState::Ready;
    //init();

    SDL_Event event;
    event.user.type = deathEvent;
    SDL_PushEvent(&event);
    
}

void DreamGame::update(float time) {
    auto b = false;
    if (gameState == GameState::Running || gameState == GameState::Ready) {
        updatePhysics();

        auto sceneObjects = currentScene->getSceneObjects();

        for (int i = 0; i < sceneObjects->size(); i++) {
            sceneObjects->at(i)->update(time);
        }

        // Remove elements marked for deletion
        auto toErase = std::remove_if(sceneObjects->begin(), sceneObjects->end(), [](std::shared_ptr<GameObject> x) {return x->destroyed; });
        sceneObjects->erase(toErase, sceneObjects->end());

        // Also remove deleted objects from the current room
        if (level != nullptr && level->currentRoom != nullptr) {
            auto *roomObjects = &level->currentRoom->getComponent<RoomComponent>()->roomObjects;
            toErase = std::remove_if(roomObjects->begin(), roomObjects->end(), [](std::shared_ptr<GameObject> x) {return x->destroyed; });
            roomObjects->erase(toErase, roomObjects->end());
        }
    }
}

void DreamGame::render() {

    auto rp = RenderPass::create()
        .withCamera(camera->getCamera())
        .build();

    //auto pos = camera->getGameObject()->getPosition();

    auto spriteBatchBuilder = SpriteBatch::create();
    for (auto& go : *(currentScene->getSceneObjects())) {
        go->renderSprite(spriteBatchBuilder);
    }

    auto sb = spriteBatchBuilder.build();
    rp.draw(sb);

    if (doDebugDraw) {

        world->DrawDebugData();
        rp.drawLines(debugDraw.getLines());
        debugDraw.clear();

        GuiHelper::getInstance()->setupDebugGui();

        bool* open = nullptr;
        ImGui::Begin(GuiHelper::getInstance()->DEBUG_NAME, open);
        ImGui::Text("TOTAL GAME OBJECTS: %i", currentScene->getSceneObjects()->size());
        ImGui::End();

    }

    // render gui
    for (auto& go : *(currentScene->getSceneObjects())) {
        for (auto& comp : go->getComponents()) {
            comp->onGui();
        }
    }

}

void DreamGame::pause() {

    auto pause = pauseMenu.createGameObject();
    pause->addComponent<PauseMenuComponent>();

    gameState = GameState::Pause;
    pauseMenu.appendSceneObjects(*currentScene->getSceneObjects());
    currentScene = &pauseMenu;
    
}

void DreamGame::resume(){
    currentScene = &game;
    pauseMenu.cleanSceneObjects();
    gameState = GameState::Running;

    for (auto& go : *(currentScene->getSceneObjects())) {
        for (auto& comp : go->getComponents()) {
            comp->resetKeys();
        }
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
            // press 'y' for physics debug
            doDebugDraw = !doDebugDraw;
            if (doDebugDraw) {
                world->SetDebugDraw(&debugDraw);
                
            }
            else {
                world->SetDebugDraw(nullptr);
            }
            break;

        case SDLK_p:
            if (gameState != GameState::Pause)
                pause();
            else
                resume();
            break;
        }

    }
}


std::shared_ptr<GameObject> DreamGame::reactivateGameObject(std::shared_ptr<GameObject> obj) {
    obj->destroyed = false;
    currentScene->getSceneObjects()->push_back(obj);
    auto phys = obj->getComponent<PhysicsComponent>();
    if (phys != nullptr) {
        phys->unpause();
    }
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
        //std::cout << "PhysicsComponent does not exist in scene" << std::endl;
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

