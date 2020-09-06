#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera
{
	private:

		glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
		float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
		float pitch = 0.0f;
		float fov = 45.0f;
		float radius = 10.0f;
		float SCR_WIDTH;
		float SCR_HEIGHT;

	public:
	// Constructor
		Camera(glm::vec3 position, float width, float height){


			cameraPos = position;
			SCR_WIDTH = width;
			SCR_HEIGHT = height;
			

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

			if (pitch > 89.0f)
        			pitch = 89.0f;

    			if (pitch < -89.0f)
        			pitch = -89.0f;
		}

};

#endif
