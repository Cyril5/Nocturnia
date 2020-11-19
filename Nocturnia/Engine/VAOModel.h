#ifndef VAOMODEL_H
#define VAOMODEL_H

#include <vector>

namespace Render {

	class VAOModel {

		public:

			VAOModel(std::vector<float>& vertices); // Constructeur avec seulement des vertices
			VAOModel(std::vector<float> &vertices, std::vector<int> &indices); //Constructeur avec vertices + indices triangles

			unsigned int loadTexture(const char* textureFilename, bool rgba=false);
			void draw();
			void unbindVAO();

			unsigned int getVAOID() const;
			//float* getVertices() const;
			//float vertex[9] = {
			//			-0.5f, -0.5f, 0.0f,
			//			0.5f, -0.5f, 0.0f,
			//			0.0f,  0.5f, 0.0f,
			//};
		
		private:
			unsigned int VAOID;
			unsigned int VBOID;
			unsigned int EBOID;
			std::vector<float> m_Vertices;
			std::vector<int> m_Indices;
	};
}


#endif

