#ifndef PHYSICS_H
#define PHYSICS_H

#include "btBulletDynamicsCommon.h"

class Physics
{
        private:


	public:

	btDefaultCollisionConfiguration* collisionConfiguration; 
        btCollisionDispatcher* dispatcher; 
        btBroadphaseInterface* overlappingPairCache; 
        btSequentialImpulseConstraintSolver* solver; 
        btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;




		Physics(){

			collisionConfiguration = new btDefaultCollisionConfiguration();

         dispatcher = new btCollisionDispatcher(collisionConfiguration);

        overlappingPairCache = new btDbvtBroadphase();

        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));
 
		}


		void cleanUp(){

 for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
        {
                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject(obj);
                delete obj;
        }

        //delete collision shapes
        for (int j = 0; j < collisionShapes.size(); j++)
        {
                btCollisionShape* shape = collisionShapes[j];
                collisionShapes[j] = 0;
                delete shape;
        }

        //delete dynamics world
        delete dynamicsWorld;

        //delete solver
        delete solver;

        //delete broadphase
        delete overlappingPairCache;

        //delete dispatcher
        delete dispatcher;

        delete collisionConfiguration;





		}

};

#endif
