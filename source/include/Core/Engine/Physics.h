#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btVector3.h"
#include "RigidBody.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <glm/ext/vector_float3.hpp>
#include <vector>

class Physics {
private:
  Physics();

private:
  static const btVector3 DEFAULT_GRAVITY;

public:
  std::vector<PrimitiveRigidBody> primitiveRigidBodies;

  btDefaultCollisionConfiguration *collisionConfig;
  btCollisionDispatcher *dispatcher;
  btBroadphaseInterface *broadphase;
  btSequentialImpulseConstraintSolver *solver;
  btDiscreteDynamicsWorld *dynamicsWorld;

public:
  Physics(const Physics &) = delete;
  Physics &operator=(const Physics &) = delete;
  Physics(Physics &&) = delete;
  Physics &operator=(Physics &&) = delete;

  static Physics *getInstance();

  bool init(const btVector3 &gravity = btVector3(0.0f, -9.8f, 0.0f));

  void setGravity(const btVector3 &gravity);

  void addPrimitiveRigidBody(PrimitiveRigidBody &rigidBody);
  void addConvexHullRigidBody(ConvexHullRigidBody &rigidBody,
                              const std::vector<glm::vec3> &vertices);

  btVector3 getGravity() const;
  void free();

private:
  void createRigidBody(PrimitiveRigidBody &rigidBody);
  btDefaultMotionState *createDefaultMotionState(btQuaternion &rotation,
                                                 btVector3 &position);
  btRigidBody::btRigidBodyConstructionInfo
  createRigidBodyInfo(PrimitiveRigidBody &rigidBody);
};
