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
#include <sre/Inspector.hpp>


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

    GuiHelper::getInstance()->setupImGuiStyle();
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
    start->setScale(1.1f);
    auto spriteComponent = start->addComponent<SpriteComponent>();
    spriteComponent->setSprite(backgroundSprite);
    
}

void DreamGame::init() {

    if (world != nullptr) { // deregister call backlistener to avoid getting callbacks when recreating the world
        world->SetContactListener(nullptr);
    }

    this->level.reset();
    camera.reset();
    player.reset();
    levelGuiComp.reset();
    game.cleanSceneObjects();
    camera.reset();
    physicsComponentLookup.clear();
    initPhysics();

    countEnemiesKilled = 0;

    auto camObj = game.createGameObject();
    camObj->name = "Camera";
    camera = camObj->addComponent<SideScrollingCamera>();
    camObj->setPosition(windowSize * 0.5f);
}

void DreamGame::play() {

    currentScene = &game;

    PlayerSettings pSettings;
    pSettings.position = glm::vec2(0,0);
    pSettings.speed = 15.0f;
    pSettings.knockback = 1.0f;
    pSettings.type = CharacterType::Wraith;
    pSettings.name = playerName;

    Controls controls;
    insertKeys(controls);

    pSettings.keybinds = controls;
    
    std::cout << "Creating player" << std::endl;
    player = CharacterBuilder::createPlayer(pSettings);

    auto levelGui = currentScene->createGameObject();
    levelGuiComp = levelGui->addComponent<LevelGuiComponent>();
    
    LevelSettings testLevelSettings;
    testLevelSettings.difficulty = (int)StringToDifficulty.at(difficulties[selectedDifficulty]);
    testLevelSettings.name = "TestLevel";
    testLevelSettings.rooms = 3;
    std::cout << "Creating level" << std::endl;
    level = LevelBuilder::createLevel(testLevelSettings);

    levelGuiComp->setLevel(level);
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

void DreamGame::insertKeys(Controls& c) {
    if (StringToKeys.at(possibleMoveKeys[selectedMoveKey]) == Keys::ASWD) {
        c.up = SDLK_w;
        c.down = SDLK_s;
        c.left = SDLK_a;
        c.right = SDLK_d;
    }
    else if (StringToKeys.at(possibleMoveKeys[selectedMoveKey]) == Keys::ARROWS) {
        c.up = SDLK_UP;
        c.down = SDLK_DOWN;
        c.left = SDLK_LEFT;
        c.right = SDLK_RIGHT;
    }
    else if (StringToKeys.at(possibleMoveKeys[selectedMoveKey]) == Keys::JKIL) {
        c.up =   SDLK_i;
        c.down = SDLK_k;
        c.left = SDLK_j;
        c.right = SDLK_l;
    }

    if (StringToKeys.at(possibleShootKeys[selectedShootKey]) == Keys::ASWD) {
        c.shootUp = SDLK_w;
        c.shootDown = SDLK_s;
        c.shootLeft = SDLK_a;
        c.shootRight = SDLK_d;
    }
    else if (StringToKeys.at(possibleShootKeys[selectedShootKey]) == Keys::ARROWS) {
        c.shootUp = SDLK_UP;
        c.shootDown = SDLK_DOWN;
        c.shootLeft = SDLK_LEFT;
        c.shootRight = SDLK_RIGHT;
    }
    else if (StringToKeys.at(possibleShootKeys[selectedShootKey]) == Keys::JKIL) {
        c.shootUp = SDLK_i;
        c.shootDown = SDLK_k;
        c.shootLeft  = SDLK_j;
        c.shootRight = SDLK_l;
    }

    if (StringToKeys.at(possibleSuperShootKeys[selectedSuperShootKey]) == Keys::SPACE) {
        c.shot = SDLK_SPACE;
    }
    else if (StringToKeys.at(possibleSuperShootKeys[selectedSuperShootKey]) == Keys::Q) {
        c.shot = SDLK_q;
    }

}

void DreamGame::gameOver() {
    gameState = GameState::Ready;

    SDL_Event event;
    event.user.type = deathEvent;
    SDL_PushEvent(&event);
    
}

void DreamGame::update(float time) {

    if (gameState == GameState::Running) {
        updatePhysics();

        auto sceneObjects = currentScene->getSceneObjects();

        for (int i = 0; i < sceneObjects->size(); i++) {
            sceneObjects->at(i)->update(time);
        }

        if (level != nullptr && level->finished) {
            level->nextLevel->player = player;

            level->unloadLevel();

            level = level->nextLevel;
            levelGuiComp->setLevel(level);
            level->loadLevel();
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

        static Inspector profiler;
        profiler.update();
        profiler.gui(false);

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

    //forget movements/shooting that were being executed before
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
            if(gameState==GameState::Running){
                init();
                play();
            }
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
            
            if (gameState == GameState::Running)
                pause();
            else if(gameState == GameState::Pause)
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

