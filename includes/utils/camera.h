#ifndef CAMERA_H
#define CAMERA_H

#include "btBulletDynamicsCommon.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/perpendicular.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/string_cast.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

btRigidBody* body;
btDiscreteDynamicsWorld* dynamicsWorld;
class Camera
{
	private:

		glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
		float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		float pitch = 0.0f;
		int physicsIndex;
		float fov = 45.0f;
		float radius = 10.0f;
		float SCR_WIDTH;
		float SCR_HEIGHT;
		float sensitivity = 0.1f;
		float pitchLimit = 89.0f; 
		bool firstMouse = true;
		float lastX;
		float lastY;
		float nearPlane = 0.1f;
		float farPlane = 100.0f; 
	
	public:
	// Constructor
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	bool jump = false;
	bool grounded = false;
		Camera(glm::vec3 position, float width, float height){

			cameraPos = position;
			SCR_WIDTH = width;
			SCR_HEIGHT = height;
			lastX = SCR_WIDTH / 2.0f;
			lastY = SCR_HEIGHT / 2.0f;	
		}

	// getters and setters
		void setPosition(glm::vec3 pos){

			cameraPos = pos;
		}
		
		void incPosition(glm::vec3 inc){
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[physicsIndex];
                        btRigidBody* body = btRigidBody::upcast(obj);
                        btTransform trans;
				
			body->activate(true);
                        if (body && body->getMotionState())
                        {
                                body->getMotionState()->getWorldTransform(trans);
                        }
                        else
                        {
                                trans = obj->getWorldTransform();
                        }

			//cameraPos += inc;
		        //trans.setOrigin(btVector3(cameraPos.x, cameraPos.y, cameraPos.z));	
                        body->setLinearVelocity(btVector3(inc.x, body->getLinearVelocity().getY(), inc.z));

		}

		glm::vec3 getPosition(){

			return cameraPos;
		}
		
		void setYaw(float yaw){

			yaw = yaw;
		}
		
		void incYaw(float inc){

			yaw += inc;
		}

		float getYaw(){

			return yaw;
		}

		void setPitch(float pitch){

			pitch = pitch;
			boundPitch();

		}
		
		void incPitch(float inc){

			pitch += inc;
			boundPitch();
		}

		float getPitch(){

			return pitch;
		}

		void setFront(glm::vec3 front){

			cameraFront = front;
		}

		glm::vec3 getFront(){

			return cameraFront;
		}

		void incFront(glm::vec3 inc){

			cameraFront += inc;
		}
		void setUp(glm::vec3 up){

			cameraUp = up;
		}

		glm::vec3 getUp(){

			return cameraUp;
		}

		void incUp(glm::vec3 inc){

			cameraUp += inc;
		}

		void setRadius(float rad){

			radius = rad;
		}

		int getRadius(){

			return radius;
		}

		// methods
		void boundPitch(){

			if (pitch > pitchLimit)
        			pitch = pitchLimit;

    			if (pitch < -pitchLimit)
        			pitch = -pitchLimit;
		}

		glm::mat4 calcProjection(){

		return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, nearPlane, farPlane);
		}

		glm::mat4 calcView(){

			return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		}
		//update directional vector 
		void updateCam(){
			
			 glm::vec3 front;

			 front.x = cos(glm::radians(getYaw())) * cos(glm::radians(getPitch()));

			 front.y = sin(glm::radians(getPitch()));
			 front.z = sin(glm::radians(getYaw())) * cos(glm::radians(getPitch()));

			 setFront(glm::normalize(front));

		}

		void castRay(){
		btVector3 start = btVector3(getPosition().x, getPosition().y, getPosition().z);
		glm::vec3 tempEnd = getPosition() + glm::normalize(getFront()) * 500000.0f;
		btVector3 end = btVector3(tempEnd.x, tempEnd.y, tempEnd.z);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

                // Perform raycast
                dynamicsWorld->rayTest(start, end, RayCallback);
                if(RayCallback.hasHit()) {

                        //End = RayCallback.m_hitPointWorld;
                        //Normal = RayCallback.m_hitNormalWorld;
			RayCallback.m_collisionObject->activate();
		btRigidBody* body = (btRigidBody*)RayCallback.m_collisionObject->getCollisionShape()->getUserPointer();
		
		glm::vec3 gunforce = getFront() * 1.0f;	
				body->applyCentralImpulse(btVector3(gunforce.x, gunforce.y, gunforce.z));
                }
        }

	bool isGrounded(){
		btVector3 start = btVector3(getPosition().x, getPosition().y, getPosition().z);
		glm::vec3 tempEnd = getPosition() + glm::normalize(-getUp()) * 1.2f;
		btVector3 end = btVector3(tempEnd.x, tempEnd.y, tempEnd.z);
		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

                // Perform raycast
                dynamicsWorld->rayTest(start, end, RayCallback);
                if(RayCallback.hasHit()) {

                        //End = RayCallback.m_hitPointWorld;
                        //Normal = RayCallback.m_hitNormalWorld;
			return true;	
                }else{
		return false;
		}
        }



		void getCollision(btAlignedObjectArray<btCollisionShape*> collisionShapes, btDiscreteDynamicsWorld* dw, float objectMass, glm::vec3 scale = glm::vec3(0,0,0)){
			
		scale = glm::vec3(1.0f,1.0f,1.0f);
		dynamicsWorld = dw;	
		
btCollisionShape* colShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));

                collisionShapes.push_back(colShape);


                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar mass(10.0f);

                //rigidbody is dynamic if and only if mass is non zero, otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);

                if (isDynamic)
                        colShape->calculateLocalInertia(mass, localInertia);

                startTransform.setOrigin(btVector3(getPosition().x, getPosition().y, getPosition().z));


 btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
                 body = new btRigidBody(rbInfo);
		body->setFriction(0.95f);
		body->setRestitution(0.0f);
                dynamicsWorld->addRigidBody(body);
 physicsIndex = dynamicsWorld->getNumCollisionObjects() - 1;

		}

void applyForce(float magnitude, bool press, glm::vec3 dir){
			if(isGrounded()){
			body->activate(true);
			jump = true;
				body->setLinearVelocity(btVector3(0.0f, magnitude, 0.0f));
			}
				jump = false;
}
	        void runPhysics(){

btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[physicsIndex];
                        btRigidBody* body = btRigidBody::upcast(obj);
                        btTransform trans;
                        if (body && body->getMotionState())
                        {
                                body->getMotionState()->getWorldTransform(trans);
                        }
                        else
                        {
                                trans = obj->getWorldTransform();
                        }




	setPosition(glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())));
		}	


		
		// rotate camera based on screen coordinate (x,y)
		void look(double xpos, double ypos)		   {

			if (firstMouse){

        	    	    lastX = xpos;
        	    	    lastY = ypos;
        	    	    firstMouse = false;
		    	}else if(xpos != lastX || ypos != lastY){

    			float xoffset = xpos - lastX;
    			float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    			lastX = xpos;
    			lastY = ypos;

    			xoffset *= sensitivity;
    			yoffset *= sensitivity;

    			incYaw(xoffset);
    			incPitch(yoffset);

		}else{
		
			return;	
		}
		}

};

#endif
