#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL\glew.h>
#include "Shader.h"


namespace NocturniaEngine {
	namespace Render {

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 textureCoords;
			// tangent
			glm::vec3 tangent;
			glm::vec3 bitTangent;
		};

		struct Texture
		{
			unsigned short id;
			std::string type;
			std::string path;  // nous mémorisons le chemin d’accès pour comparaison avec d’autres textures
		};

		class Mesh
		{
			public:
				// Constructor
				Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

				void draw(Shader shader) const; 
				void setupMesh();



			private:
				std::vector<Vertex> m_Vertices;
				std::vector<unsigned int> m_Indices;
				std::vector <Texture> m_Textures;
				unsigned int m_VAO;
				unsigned int m_VBO;
				unsigned int m_EBO;
		};
	}
}

#endif