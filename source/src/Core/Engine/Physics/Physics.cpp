#include "Physics.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"
#include "BulletCollision/CollisionShapes/btConvexHullShape.h"
#include "BulletCollision/CollisionShapes/btCylinderShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btVector3.h"
#include "Logger.h"
#include "RigidBody.h"
#include <vector>

Physics::Physics()
    : collisionConfig(nullptr), dispatcher(nullptr), broadphase(nullptr),
      solver(nullptr), dynamicsWorld(nullptr) {}

const btVector3 Physics::DEFAULT_GRAVITY(0.0f, -9.8f, 0.0f);

Physics *Physics::getInstance() {
  static Physics instance;
  return &instance;
}

bool Physics::init(const btVector3 &gravity) {
  Logger::physics->info("Initializing Bullet Physics...");

  primitiveRigidBodies = std::vector<PrimitiveRigidBody>();

  collisionConfig = new btDefaultCollisionConfiguration();
  dispatcher = new btCollisionDispatcher(collisionConfig);
  broadphase = new btDbvtBroadphase();
  solver = new btSequentialImpulseConstraintSolver;
  dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver,
                                              collisionConfig);

  if (gravity == DEFAULT_GRAVITY) {
    Logger::physics->info(
        "No gravity value passed. Will use deafult earth gravity...");
    dynamicsWorld->setGravity(DEFAULT_GRAVITY);
  } else {
    Logger::physics->info("Gravity set to {}, {}, {}.", gravity.x(),
                          gravity.y(), gravity.z());
  }

  if (!collisionConfig || !dispatcher || !broadphase || !solver ||
      !dynamicsWorld) {
    Logger::physics->warn("Bullet Physics failed to inifialize.");
    return false;
  }

  Logger::physics->info("Bullet Physics initiated successfully.");
  return true;
}

void Physics::setGravity(const btVector3 &gravity) {
  dynamicsWorld->setGravity(gravity);
}

void Physics::addPrimitiveRigidBody(PrimitiveRigidBody &rigidBody) {
  createRigidBody(rigidBody);

  if (rigidBody.initialized) {
    Logger::physics->info("Rigid body initialized successfully.");
    Logger::physics->trace("Adding rigid body to rigidBodies vector...");
    primitiveRigidBodies.push_back(rigidBody);
  }
}

void Physics::addConvexHullRigidBody(ConvexHullRigidBody &rigidBody,
                                     const std::vector<glm::vec3> &vertices) {
  rigidBody.collisionShape = new btConvexHullShape();

  btConvexHullShape *hull =
      static_cast<btConvexHullShape *>(rigidBody.collisionShape);

  if (!hull) {
    Logger::physics->error(
        "Failed to cast from btCollisionShape to btConvexHullShape.");
    return;
  }

  for (const auto &vertex : vertices) {
    hull->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
  }
}

btVector3 Physics::getGravity() const { return dynamicsWorld->getGravity(); }

void Physics::free() {
  Logger::physics->info("Destroying Bullet physics resources...");
  delete dynamicsWorld;
  delete solver;
  delete broadphase;
  delete dispatcher;
  delete collisionConfig;
  Logger::physics->info("Bullet physics resources destroyed sucessfully.");
}

void Physics::createRigidBody(PrimitiveRigidBody &rigidBody) {
  Logger::physics->debug("Identifying rigid body's collision shape...");
  if (dynamic_cast<btBoxShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btBoxShape");
    rigidBody.collisionShape = new btBoxShape(rigidBody.dimension);
    rigidBody.calcLocalInertia();
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  } else if (dynamic_cast<btSphereShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btSphereShape");
    rigidBody.collisionShape = new btSphereShape(rigidBody.radius);
    rigidBody.calcLocalInertia();
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  } else if (dynamic_cast<btCapsuleShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btCapsuleShape");
    rigidBody.collisionShape =
        new btCapsuleShape(rigidBody.radius, rigidBody.height);
    rigidBody.calcLocalInertia();
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  } else if (dynamic_cast<btCylinderShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btCylinderShape");
    rigidBody.collisionShape = new btCylinderShape(rigidBody.dimension);
    rigidBody.calcLocalInertia();
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  } else if (dynamic_cast<btConeShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btConeShape");
    rigidBody.collisionShape =
        new btConeShape(rigidBody.radius, rigidBody.height);
    rigidBody.calcLocalInertia();
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  } else if (dynamic_cast<btStaticPlaneShape *>(rigidBody.collisionShape)) {
    Logger::physics->info("Rigid body's collision shape: btStaticPlaneShape");
    rigidBody.collisionShape =
        new btStaticPlaneShape(rigidBody.normal, rigidBody.height);
    rigidBody.rigidBody = new btRigidBody(createRigidBodyInfo(rigidBody));
    rigidBody.initialized = true;
  }
}

btDefaultMotionState *Physics::createDefaultMotionState(btQuaternion &rotation,
                                                        btVector3 &position) {
  Logger::physics->trace("Creating default motion state...");
  return new btDefaultMotionState(btTransform(rotation, position));
}

btRigidBody::btRigidBodyConstructionInfo
Physics::createRigidBodyInfo(PrimitiveRigidBody &rigidBody) {
  Logger::physics->trace("Creating rigid body info...");
  return btRigidBody::btRigidBodyConstructionInfo(
      rigidBody.mass,
      createDefaultMotionState(rigidBody.initialRotation,
                               rigidBody.initialPosition),
      rigidBody.collisionShape, rigidBody.inertia);
}
