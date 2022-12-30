#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"
#include <vector>
#include "Box2D/Dynamics/b2World.h"
#include "GameObject.hpp"
#include "SideScrollingCamera.hpp"
#include "Box2DDebugDraw.hpp"
#include "Scene.hpp"
#include "Level.hpp"
#include "LevelGuiComponent.hpp"
#include "CharacterBuilder.hpp"


class PhysicsComponent;

enum class GameState {
    Ready,
    Running,
    Pause,
    Settings
};

enum class Keys {
    ASWD,
    ARROWS,
    JKIL,
    SPACE,
    Q
};

const std::unordered_map<Keys, char*> KeysToString{
    {Keys::ASWD  ,	    "ASWD"    },
    {Keys::ARROWS,	    "ARROWS"  },
    {Keys::SPACE ,	    "SPACE"   },
    {Keys::Q     ,	    "Q"       },
    {Keys::JKIL  ,	    "JKIL"    },
};

const std::unordered_map<std::string, Keys> StringToKeys{
    {"ASWD"  ,    Keys::ASWD,	 },
    {"ARROWS",    Keys::ARROWS,	 },
    {"SPACE" ,    Keys::SPACE,	 },
    {"Q"     ,    Keys::Q,	     },
    {"JKIL"  ,    Keys::JKIL,	 },
};

class DreamGame : public b2ContactListener {
public:
    DreamGame();

    //std::shared_ptr<GameObject> createGameObject(); -> Objects are created inside the scene, not here
    std::shared_ptr<GameObject> reactivateGameObject(std::shared_ptr<GameObject> obj);

    static const glm::vec2 windowSize;

    void BeginContact(b2Contact* contact) override;

    void EndContact(b2Contact* contact) override;

    void gameOver();

    static DreamGame* instance;

    static constexpr float32 timeStep = 1.0f / 60.0f;
    const float physicsScale = 100;

    bool doDebugDraw = false;

    Scene* currentScene;
    std::shared_ptr<Level> level;

    Uint32 deathEvent;
    Uint32 playEvent;

private:
    GameState gameState = GameState::Ready;
    sre::SDLRenderer r;

    void init();

    void initPhysics();

    void update(float time);

    void render();

    void onKey(SDL_Event& event);

    void handleContact(b2Contact* contact, bool begin);

    void buildStartMenu();

    void play();

    void pause();

    void resume();

    void startGame();

    void insertKeys(Controls&);


    int difficulty = 1;
    char defaultName[64] = "DreamHunter";
    char playerName[64] = "DreamHunter";

    std::shared_ptr<GameObject> player;
    std::shared_ptr<LevelGuiComponent> levelGuiComp;
    std::shared_ptr<SideScrollingCamera> camera;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas_inside;
    std::shared_ptr<sre::SpriteAtlas> spriteAtlas_baseWraith;

    std::shared_ptr<sre::SpriteAtlas> guiAtlas;    
    
    Scene startMenu;
    Scene game;
    Scene pauseMenu;

    int lengthShootKeys = 3;

    char* possibleMoveKeys[3] = { KeysToString.at(Keys::ASWD),  KeysToString.at(Keys::ARROWS), KeysToString.at(Keys::JKIL) };
    char* possibleShootKeys[3] = { KeysToString.at(Keys::ASWD),  KeysToString.at(Keys::ARROWS), KeysToString.at(Keys::JKIL) };
    char* possibleSuperShootKeys[2] = { KeysToString.at(Keys::Q),  KeysToString.at(Keys::SPACE) };

    int selectedMoveKey = 0;
    int selectedShootKey = 1;
    int selectedSuperShootKey = 0;
    
    


    //std::vector<std::shared_ptr<GameObject>> sceneObjects;

    void updatePhysics();
    b2World* world = nullptr;

    void registerPhysicsComponent(PhysicsComponent* r);
    void deregisterPhysicsComponent(PhysicsComponent* r);
    std::map<b2Fixture*, PhysicsComponent*> physicsComponentLookup;


    Box2DDebugDraw debugDraw;

    friend class PhysicsComponent;
    friend class RoomComponent;
    friend class PlayerController;
    friend class CharacterBuilder;
    friend class Level;

    friend class StartMenuComponent;
    friend class PauseMenuComponent;
    friend class EndMenuComponent;

    friend class CharacterComponent;
    friend class LevelGuiComponent;
    

};