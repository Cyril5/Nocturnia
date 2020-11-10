#include "VAOModel.h";
#include <GL\glew.h>
#include <iostream>

using namespace Render;

VAOModel::VAOModel(std::vector<float> &vertices)
{
	m_Vertices = vertices;

	// Générer le VAO
	glGenVertexArrays(1, &VAOID);
	// Générer le VBO
	glGenBuffers(1, &VBOID);

	// 1. Lier le Vertex Array Object (VAO
	glBindVertexArray(VAOID);
	// 2. Copier les vertices dans un Vertex Buffer Object pour qu’OpenGL les utilise
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
	// 3. Initialiser les pointeurs d’attributs (attribute pointer) de vertices à l'emplacement 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//// Même chose pour les Texture coords à l'emplacement 1
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Fin du VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

// TODO : Ajouter le destructeur ici
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
//glDeleteVertexArrays(1, &VAO);
//glDeleteBuffers(1, &VBO);

unsigned int VAOModel::getVAOID() const
{
	return VAOID;
}

//float* VAOModel::getVertices() const
//{
//	return vertices;
//}




