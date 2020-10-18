#ifndef OBJECT_H
#define OBJECT_H

#include "btBulletDynamicsCommon.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/shader.h>
#include <utils/camera.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/quaternion.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Object
{
	private:
		
	    	glm::vec3 position;
		glm::vec3 rotationAxis;
		float rotationRadians;
		int physicsIndex;
	public:	
 		unsigned int VBO, VAO, EBO;
		
		glm::mat4 rot;	
		// Constructor
	    	Object(glm::vec3 pos)
	    	{
		position = pos;
		rotationRadians = 0.0f;
		rotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	
	    	}
	
	    	// getters and setters
		void setPosition(glm::vec3 pos){
	
			position = pos;
		}
			
		void incPosition(glm::vec3 inc){
	
			position += inc;
		}
	
		glm::vec3 getPosition(){
	
			return position;
		}

		void setRotationAxis(glm::vec3 axis){	
			rotationAxis = axis;
		}
			
		
		glm::vec3 getRotationAxis(){
	
			return rotationAxis;
		}
		void setRotationRadians(float rad){
	
			rotationRadians = rad;
		}
			
		void incRotationRadians(float rad){
	
			rotationRadians += rad;
		}
	
		float getRotationRadians(){
	
			return rotationRadians;
		}

	// Methods

	        void runPhysics(btDiscreteDynamicsWorld* dynamicsWorld){

btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[physicsIndex];
obj->setRestitution(0.9f);
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

		rot = glm::toMat4(glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));


		}	


		void getCollision(btAlignedObjectArray<btCollisionShape*> collisionShapes, btDiscreteDynamicsWorld* dynamicsWorld, float objectMass){
                btCollisionShape* colShape = new btBoxShape(btVector3(3,3,3));
                //btCollisionShape* colShape = new btSphereShape(btScalar(2.));
                collisionShapes.push_back(colShape);

                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar mass(objectMass);

                //rigidbody is dynamic if and only if mass is non zero, otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                        colShape->calculateLocalInertia(mass, localInertia);

                startTransform.setOrigin(btVector3(position.x, position.y, position.z));


 btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
                btRigidBody* body = new btRigidBody(rbInfo);

                dynamicsWorld->addRigidBody(body);
 physicsIndex = dynamicsWorld->getNumCollisionObjects() - 1;

		}

		void prepare(){
	        float vertices[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f 
    };

    			glGenVertexArrays(1, &VAO);
    			glGenBuffers(1, &VBO);
    			glBindVertexArray(VAO);
    			glBindBuffer(GL_ARRAY_BUFFER, VBO);
    			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    			glEnableVertexAttribArray(0);

		}

		void rotate(char axis, float rads){

			//double pi = 3.14159265359; 
    		//	float rads = (degs * (pi / 180));
		//	incRotationRadians(rads);
			setRotationRadians(rads);
			switch(axis) {
				case 'x'  :
				      setRotationAxis(glm::vec3(1.0f,0.0f,0.0f));
			    		  break;
				case 'y'  :
				      setRotationAxis(glm::vec3(0.0f,1.0f,0.0f));
			    		  break;
				case 'z'  :
				      setRotationAxis(glm::vec3(0.0f,0.0f,1.0f));
			    		  break;
			}

		}


		void draw(Camera cam, Shader s){

       		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model,getPosition());
//	model = glm::rotate(model,getRotationRadians(), getRotationAxis());
	model = model * rot;
	//model = glm::rotate(model,rot);
	glm::mat4 view = cam.calcView();
       	glm::mat4 projection = cam.calcProjection(); // make sure to initialize matrix to identity matrix first

	s.use();
        unsigned int modelLoc = glGetUniformLocation(s.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(s.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        s.setMat4("projection", projection);



        	glBindVertexArray(VAO); 
        	glDrawArrays(GL_TRIANGLES, 0, 36);


		}
				
	
};
#endif
