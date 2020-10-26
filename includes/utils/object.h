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
		Object() {};
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


		void getCollision(btAlignedObjectArray<btCollisionShape*> collisionShapes, btDiscreteDynamicsWorld* dynamicsWorld, float objectMass, glm::vec3 scale = glm::vec3(0,0,0)  ){
		if(scale == glm::vec3(0,0,0)){

		scale = makeCollider();
		}
                btCollisionShape* colShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
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

		glm::vec3 makeCollider(){
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


		 float minx = 10000.0f;
		 float miny = 10000.0f;
		 float minz = 10000.0f;
		 float maxx = -10000.0f;
		 float maxy = -10000.0f;
		 float maxz = -10000.0f;

		int len = sizeof(vertices)/sizeof(vertices[0]);

	        for( int i = 1; i <4; i++){

		 switch(i) {
			case 1:
				for (int j=i; j < len; j+=3){
					if(vertices[j] < minx){

						minx = vertices[j];

					}

					if(vertices[j] > maxx){

						maxx = vertices[j];

					}


				}
			case 2:
				for (int j=i; j < len; j+=3){
					if(vertices[j] < miny){

						miny = vertices[j];

					}

					if(vertices[j] > maxy){

						maxy = vertices[j];

					}


				}
			case 3:
				for (int j=i; j < len; j+=3){
					if(vertices[j] < minz){

						minz = vertices[j];

					}

					if(vertices[j] > maxz){

						maxz = vertices[j];

					}


				}



					}





		 }
		
	return glm::vec3(abs(maxx-minx)/2.0f,abs(maxy-miny)/2.0f,abs(maxz-minz)/2.0f);

		}


		

		void prepare(){
	       			float verts[] = {
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
    			glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
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
