#include "RigidBody.h"

void RigidBody::calcLocalInertia() {
  collisionShape->calculateLocalInertia(mass, inertia);
}
