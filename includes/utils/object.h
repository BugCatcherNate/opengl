#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Object
{
	public:
	
	    	glm::vec3 position;
	
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
	
};
#endif
