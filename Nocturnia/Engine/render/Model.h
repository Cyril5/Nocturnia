#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <assimp/Importer.hpp>


namespace NocturniaEngine {
	namespace Render {
		class Model
		{
			public:
				Model(const char* path);
				void draw(Shader shader);

			private:
				std::vector<Mesh> m_Meshes;
				std::string m_Directory;
				std::vector<Texture> m_TexturesLoaded;

				void loadModel(std::string path);
				void processNode(aiNode* node, const aiScene *scene);
				Mesh processMesh(aiMesh* mesh, const aiScene *scene);
				std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

				unsigned int TextureFromFile(const char* path, const std::string& directory);

		};
	}
}

#endif