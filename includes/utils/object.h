#ifndef OBJECT_H
#define OBJECT_H

#include "btBulletDynamicsCommon.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/shader.h>
#include <utils/camera.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/gtx/quaternion.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
using namespace std;

class Object
{
	private:
		
	    	glm::vec3 position;
		glm::vec3 rotationAxis;
		float rotationRadians;
		int physicsIndex;
		float verticiessize;

		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> uves;

		glm::vec3 bounds;
	public:	
		Object() {};
 		unsigned int VBO, VAO, EBO;
		
		glm::mat4 rot;	

		string texturePath;
		string modelPath;
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

		void setModelPath(string path){
	
			modelPath = path;
		}

		void setTexturePath(string path){
	
			texturePath = path;
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
	
bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		exit(false);
		return false;
	}

	while( file ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
		//	if (matches != 9){
		//		printf("File can't be read by our simple parser :-( Try exporting with other options\n");
		//		fclose(file);
		//		return false;
		//	}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}
	fclose(file);
	return true;
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

		rot = glm::toMat4(glm::quat(trans.getRotation().getW(), trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));


		}	


		void getCollision(btAlignedObjectArray<btCollisionShape*> collisionShapes, btDiscreteDynamicsWorld* dynamicsWorld, float objectMass, glm::vec3 scale = glm::vec3(0,0,0)){
			
		if(scale == glm::vec3(0,0,0)){
		
			scale = bounds;	
                		}
		
btCollisionShape* colShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));

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
		colShape->setUserPointer(body);
		body->setFriction(0.95f);
                dynamicsWorld->addRigidBody(body);
 physicsIndex = dynamicsWorld->getNumCollisionObjects() - 1;

		}

		glm::vec3 makeCollider(){
	

		 float minx = 10000.0f;
		 float miny = 10000.0f;
		 float minz = 10000.0f;
		 float maxx = -10000.0f;
		 float maxy = -10000.0f;
		 float maxz = -10000.0f;
	

		for(auto& i: this->verts) { 

					if(i.x < minx){

						minx = i.x;

					}

					if(i.x > maxx){

						maxx = i.x;

					}


					if(i.y < miny){

						miny = i.y;

					}

					if(i.y > maxy){

						maxy = i.y;

					}


					if(i.z < minz){

						minz = i.z;

					}

					if(i.z > maxz){

						maxz = i.z;

					}


				}

	
	return glm::vec3(abs(maxx-minx)/2.0f,abs(maxy-miny)/2.0f,abs(maxz-minz)/2.0f);

		}
	

		void prepare(){
	std::vector<glm::vec3> vertes;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ(modelPath.c_str(), vertes, uvs, normals);	
	verts = vertes;
	bounds = makeCollider();
	verticiessize = verts.size();

	unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
     // set the texture wrapping parameters
 //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
glActiveTexture(GL_TEXTURE0);
    			glGenVertexArrays(1, &VAO);
    			glGenBuffers(1, &VBO);
    			glBindVertexArray(VAO);
    			glBindBuffer(GL_ARRAY_BUFFER, VBO);
    			glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
    			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    			glEnableVertexAttribArray(0);

				GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);


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
        	glDrawArrays(GL_TRIANGLES, 0, verticiessize);


		}
				
	
};
#endif
