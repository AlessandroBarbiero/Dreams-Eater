//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <Box2D/Box2D.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "DreamGame.hpp"

PhysicsComponent::PhysicsComponent(GameObject *gameObject)
        : Component(gameObject){
    world = DreamGame::instance->world;
}

PhysicsComponent::~PhysicsComponent() {

    delete polygon;
    delete circle;
    if (paused) {
        fixture = nullptr;
        body = nullptr;
        return;
    }
    DreamGame::instance->deregisterPhysicsComponent(this);
    if (body != nullptr && fixture!= nullptr ) {
        body->DestroyFixture(fixture);
        fixture = nullptr;
    }
    if (world != nullptr && body != nullptr ) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

void PhysicsComponent::addImpulse(glm::vec2 force) {
    b2Vec2 iForceV{force.x,force.y};
    body->ApplyLinearImpulse(iForceV, body->GetWorldCenter(), true);
}

void PhysicsComponent::addForce(glm::vec2 force) {
    b2Vec2 forceV{force.x,force.y};
    body->ApplyForce(forceV,body->GetWorldCenter(),true);
}

glm::vec2 PhysicsComponent::getLinearVelocity() {
    b2Vec2 v = body->GetLinearVelocity();
    return {v.x,v.y};
}

void PhysicsComponent::setLinearVelocity(glm::vec2 velocity) {
    b2Vec2 v{velocity.x, velocity.y};
    if (velocity != glm::vec2(0,0)){
        body->SetAwake(true);
    }
    body->SetLinearVelocity(v);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, glm::vec2 center, float density) {
    assert(body == nullptr);
    autoUpdate = type != b2_staticBody;
    // do init
    shapeType = b2Shape::Type::e_circle;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    circle = new b2CircleShape();
    circle->m_radius = radius;
    b2FixtureDef fxD;
    fxD.userData = (void*)"Circle";
    fxD.shape = circle;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);
    
    lastShape = circle;
    lastBodyDef = bd;
    lastFixtureDef = fxD;

    DreamGame::instance->registerPhysicsComponent(this);
}

// Only works for circles
void PhysicsComponent::setScale(float scale) {
    if (fixture != nullptr) {
        auto shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_circle) {
            shape->m_radius *= scale;
        }else if (shape->GetType() == b2Shape::e_polygon) {
            auto p = dynamic_cast<b2PolygonShape*>(shape);
            for (int i = 0; i < p->m_count; i++) {
                auto temp = p->m_vertices[i];
                p->m_vertices[i].Set(temp.x*scale, temp.y*scale);
            }
        }
    }
}

void PhysicsComponent::setRadius(float radius) {
    if (fixture != nullptr) {
        fixture->GetShape()->m_radius = radius;
    }
}

float PhysicsComponent::getRadius() {
    if (fixture != nullptr) {
        return fixture->GetShape()->m_radius;
    }
    return 0;
}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density) {
    assert(body == nullptr);
    autoUpdate = type != b2_staticBody;
    // do init
    shapeType = b2Shape::Type::e_polygon;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    polygon = new b2PolygonShape();
    polygon->SetAsBox(size.x, size.y, {0,0}, 0);
    b2FixtureDef fxD;
    fxD.userData = (void*)"Box";
    fxD.shape = polygon;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    lastShape = polygon;
    lastBodyDef = bd;
    lastFixtureDef = fxD;

    DreamGame::instance->registerPhysicsComponent(this);
}

void PhysicsComponent::initChain(b2BodyType type, b2Vec2* points, int32 count, glm::vec2 center, float density) {
    assert(body == nullptr);
    autoUpdate = type != b2_staticBody;
    // do init
    shapeType = b2Shape::Type::e_chain;
    b2BodyDef bd;
    bd.type = type;
    rbType = type;
    bd.position = b2Vec2(center.x, center.y);
    body = world->CreateBody(&bd);
    
    b2ChainShape* chain = new b2ChainShape();
    chain->CreateLoop(points, count);
    
    b2FixtureDef fxD;
    fxD.userData = (void*)"Chain";
    fxD.shape = chain;
    fxD.density = density;
    fixture = body->CreateFixture(&fxD);

    lastBodyDef = bd;
    lastFixtureDef = fxD;

    DreamGame::instance->registerPhysicsComponent(this);
}

bool PhysicsComponent::isSensor() {
    return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
    fixture->SetSensor(enabled);
}

void PhysicsComponent::fixRotation() {
    fixture->GetBody()->SetFixedRotation(true);
}

void PhysicsComponent::moveTo(glm::vec2 pos) {
    glm::vec2 delta = pos - getPosition();

    setLinearVelocity(delta*(1/ DreamGame::timeStep));
}

glm::vec2 PhysicsComponent::getPosition() {
    return glm::vec2(body->GetPosition().x,body->GetPosition().y);
}

bool PhysicsComponent::isAutoUpdate() const {
    return autoUpdate;
}

void PhysicsComponent::setAutoUpdate(bool autoUpdate) {
    PhysicsComponent::autoUpdate = autoUpdate;
}

void PhysicsComponent::pause() {
    paused = true;
    DreamGame::instance->deregisterPhysicsComponent(this);

    // Save body data
    std::cout << "Copying body" << std::endl;
    lastBodyDef.position = {getPosition().x, getPosition().y};
    lastBodyDef.linearVelocity = body->GetLinearVelocity();
    lastBodyDef.linearDamping = body->GetLinearDamping();
    lastBodyDef.angle = body->GetAngle();
    lastBodyDef.angularVelocity = body->GetAngularVelocity();
    lastBodyDef.angularDamping = body->GetAngularDamping();
    lastBodyDef.fixedRotation = body->IsFixedRotation();
    lastBodyDef.awake = body->IsAwake();
    lastBodyDef.bullet = body->IsBullet();
    lastBodyDef.gravityScale = body->GetGravityScale();
    lastBodyDef.userData = body->GetUserData();
    lastBodyDef.type = body->GetType();
    lastBodyDef.active = body->IsActive();
    lastBodyDef.allowSleep = body->IsSleepingAllowed();

    // Save fixture data
    std::cout << "Copying fixture" << std::endl;
    lastFixtureDef.density = fixture->GetDensity();
    lastFixtureDef.filter = fixture->GetFilterData();
    lastFixtureDef.friction = fixture->GetFriction();
    lastFixtureDef.isSensor = fixture->IsSensor();
    lastFixtureDef.restitution = fixture->GetRestitution();
    lastFixtureDef.userData = fixture->GetUserData();

    // Save shape data
    auto currShape = fixture->GetShape();
    lastShape->m_type = currShape->GetType();
    lastShape->m_radius = currShape->m_radius;
    if (currShape->GetType() == b2Shape::e_polygon) {
        auto lastPolygon = dynamic_cast<b2PolygonShape*>(lastShape);
        auto currPolygon = dynamic_cast<b2PolygonShape*>(currShape);
        lastPolygon->m_count = currPolygon->m_count;
        for (int i = 0; i < lastPolygon->m_count; i++) {
            auto curr = currPolygon->m_vertices[i];
            lastPolygon->m_vertices[i].Set(curr.x, curr.y);
        }
    }
    lastFixtureDef.shape = lastShape;

    if (body != nullptr && fixture != nullptr) {
        body->DestroyFixture(fixture);
    }
    if (world != nullptr && body != nullptr) {
        world->DestroyBody(body);
    }
}

void PhysicsComponent::unpause() {
    paused = false;

    auto bd = lastBodyDef;
    auto fxD = lastFixtureDef;
    body = world->CreateBody(&bd);
    /*
    if (phys->shapeType == b2Shape::e_circle) {
        fxD.shape = phys->circle;
    }
    else if (phys->shapeType == b2Shape::e_polygon) {
        fxD.shape = phys->polygon;
    }
    */

    fixture = body->CreateFixture(&fxD);
    
    DreamGame::instance->registerPhysicsComponent(this);
}

b2Body *PhysicsComponent::getBody() {
    return body;
}

b2Fixture *PhysicsComponent::getFixture() {
    return fixture;
}
