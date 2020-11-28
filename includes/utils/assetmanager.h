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
		cout << "what" << endl;
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
	unsigned char* data;
        
	cout << "ok" << endl;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

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
    cout << "done" << endl;
}

	// getters and setters
};

#endif
