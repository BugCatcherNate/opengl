#ifndef CAMERA_H
#define CAMERA_H

#include "btBulletDynamicsCommon.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/perpendicular.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

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

			cameraPos += inc;
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

		void getCollision(btAlignedObjectArray<btCollisionShape*> collisionShapes, btDiscreteDynamicsWorld* dynamicsWorld, float objectMass, glm::vec3 scale = glm::vec3(0,0,0)){
			
		scale = glm::vec3(1.0f,1.0f,1.0f);
		
		
btCollisionShape* colShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));

                collisionShapes.push_back(colShape);

                //btCollisionShape* colShape = new btSphereShape(btScalar(2.));

                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar mass(2.0f);

                //rigidbody is dynamic if and only if mass is non zero, otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                        colShape->calculateLocalInertia(mass, localInertia);

                startTransform.setOrigin(btVector3(getPosition().x, getPosition().y, getPosition().z));


 btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
                btRigidBody* body = new btRigidBody(rbInfo);
		body->setFriction(0.8);
                dynamicsWorld->addRigidBody(body);
 physicsIndex = dynamicsWorld->getNumCollisionObjects() - 1;

		}

void applyForce(btDiscreteDynamicsWorld* dynamicsWorld, float magnitude, glm::vec3 dir){

btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[physicsIndex];
                        btRigidBody* body = btRigidBody::upcast(obj);

			glm::vec3 axis;
			if (dir == glm::vec3(1.0f, 0.0f, 0.0f)){
				
			axis = getFront();}else if (dir == glm::vec3(0.0f, 0.0f, 1.0f) ){
 axis = glm::normalize(glm::cross(getFront(), getUp()));
			}else{
	if(std::abs(body->getLinearVelocity().getY()) < 0.01f){

	 axis = glm::vec3(0.0f, 1.0f, 0.0f);
	}else{

	 axis = glm::vec3(0.0f, 0.0f, 0.0f);
	}
			}	
			body->activate(true);
			body->applyCentralImpulse(magnitude * btVector3(axis.x, axis.y, axis.z));
}
	        void runPhysics(btDiscreteDynamicsWorld* dynamicsWorld){

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
