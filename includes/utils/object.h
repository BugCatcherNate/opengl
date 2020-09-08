#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/shader.h>
#include <utils/camera.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>


class Object
{
	private:
		
	    	glm::vec3 position;
	public:	
 		unsigned int VBO, VAO, EBO;
		
	
		// Constructor
	    	Object(glm::vec3 pos)
	    	{
		position = pos;
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
		
		// Methods

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

		void draw(Camera cam, Shader s){
       	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model,getPosition());
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
