#ifndef TREE_H
#define TREE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <utils/object.h>
#include <utils/assetmanager.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Tree : public Object
{
	private:

		public:
		Tree() {};
	// Constructor
		Tree(glm::vec3 pos, AssetManager* am):Object(pos, am){
		setTexturePath("/home/nathan/Desktop/opengl/resources/textures/treetext.png");
		setModelPath("/home/nathan/Desktop/opengl/tree.obj");
		}

	// getters and setters
		};

#endif
