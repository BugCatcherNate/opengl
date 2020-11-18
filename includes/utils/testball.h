#ifndef TESTBALL_H
#define TESTBALL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/object.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class TestBall : public Object
{
	private:

		public:
		TestBall() {};
	// Constructor
		TestBall(glm::vec3 pos):Object(pos){
		setTexturePath("/home/nathan/Desktop/opengl/resources/textures/green.png");
		setModelPath("/home/nathan/Desktop/opengl/pyramid.obj");
		}

	// getters and setters
		};

#endif
