#ifndef CRATE_H
#define CRATE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/object.h>
#include <utils/assetmanager.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Crate : public Object
{
	private:

		public:
		Crate() {};
	// Constructor
		Crate(glm::vec3 pos, AssetManager* am):Object(pos, am){
		setTexturePath("/home/nathan/Desktop/opengl/resources/textures/crate.png");
		setModelPath("/home/nathan/Desktop/opengl/crate.obj");
		}

	// getters and setters
		};

#endif
