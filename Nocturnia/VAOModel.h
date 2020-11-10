#ifndef VAOMODEL_H
#define VAOMODEL_H

#include <vector>

namespace Render {

	class VAOModel {

		public:
			VAOModel(std::vector<float> &vertices); //Constructeur

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
			std::vector<float> m_Vertices;
	};
}


#endif

