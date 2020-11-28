#ifndef ASSETMANAGER_H 
#define ASSETMANAGER_H 

#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>
#include <glad/glad.h>
using namespace std;

class AssetManager
{
	private:

	public:
	// Constructor
        vector<string> modelPaths;
        vector<string> texturePaths;
	vector<FILE*> modelData;
	vector<unsigned char*> textureData;
	AssetManager(){	}
	
	FILE* modelPath(string path){
if(find(modelPaths.begin(), modelPaths.end(), path) != modelPaths.end()) {
	cout << "model already loaded" << endl;
	auto it = find(modelPaths.begin(), modelPaths.end(), path);
        int index = it - modelPaths.begin();
        
	return modelData[index]; 
} else {
	modelPaths.push_back(path);
	
	FILE * file = fopen(path.c_str(), "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		exit(false);
}

	modelData.push_back(file);
		return file;
	}


	}

	void texturePath(string path, int width, int height, int nrChannels){
	bool found = true;
	unsigned char* data;
	unsigned char* tempdata;
		if(find(texturePaths.begin(), texturePaths.end(), path) != texturePaths.end()) {
	cout << "texture already loaded" << endl;
	auto it = find(texturePaths.begin(), texturePaths.end(), path);
        int index = it - texturePaths.begin();
        
	 data = textureData[index]; 

} else {
	texturePaths.push_back(path);
	found = false;

	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	tempdata = data;
	textureData.push_back(tempdata);
}
 	if (tempdata)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempdata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
if(!found){
    stbi_image_free(data);
}

	}
	// getters and setters
};

#endif
