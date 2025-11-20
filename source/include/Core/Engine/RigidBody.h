#pragma once

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btQuaternion.h"
#include "LinearMath/btScalar.h"
#include "LinearMath/btVector3.h"

struct RigidBody {
  btRigidBody *rigidBody;
  btCollisionShape *collisionShape;

  btScalar mass;
  btVector3 inertia;
  btVector3 initialPosition;
  btQuaternion initialRotation;

  short collisionGroup;
  short collisionMask;

  bool initialized;

  void calcLocalInertia();
};

struct PrimitiveRigidBody : public RigidBody {
  btVector3 dimension;
  btScalar radius;
  btScalar height;
  btVector3 normal;
};

struct ConvexHullRigidBody : public RigidBody {};
