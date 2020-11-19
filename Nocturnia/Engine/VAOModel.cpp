#include "VAOModel.h"
#include <GL\glew.h>
#include <iostream>
#include <stb_image.h>
#include "../utils/ConsoleColor.h"

using namespace Render;

VAOModel::VAOModel(std::vector<float>& vertices)
{
	m_Vertices = vertices; // vertices du model


	// Générer le VAO
	glGenVertexArrays(1, &VAOID);
	// Générer le VBO
	glGenBuffers(1, &VBOID);
	// Générer le EBO
	glGenBuffers(1, &EBOID);

	// 1. Lier le Vertex Array Object (VAO
	glBindVertexArray(VAOID);
	// 2. Copier les vertices dans un Vertex Buffer Object pour qu’OpenGL les utilise
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

	// 3. Initialiser les pointeurs d’attributs (attribute pointer) de vertices à l'emplacement 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Même chose pour les Texture coords à l'emplacement 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Fin du VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

// Fin du VAO
void VAOModel::unbindVAO() {
	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

VAOModel::VAOModel(std::vector<float> &vertices, std::vector<int> &indices)
{
	m_Vertices = vertices; // vertices du model
	m_Indices = indices; // indique comment les faces triangulaires doivent être construit

	// Générer le VAO
	glGenVertexArrays(1, &VAOID);
	// Générer le VBO
	glGenBuffers(1, &VBOID);
	// Générer le EBO
	glGenBuffers(1, &EBOID);

	// 1. Lier le Vertex Array Object (VAO
	glBindVertexArray(VAOID);
	// 2. Copier les vertices dans un Vertex Buffer Object pour qu’OpenGL les utilise
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);
	// 3. Copier les indices dans un Element Buffer Object pour qu’OpenGL les utilise
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), m_Indices.data(), GL_STATIC_DRAW);

	// 3. Initialiser les pointeurs d’attributs (attribute pointer) de vertices à l'emplacement 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Même chose pour les Texture coords à l'emplacement 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

unsigned int VAOModel::loadTexture(const char* textureFilename, bool rgba) {

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// définit les options de la texture actuellement liée
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//charge et génère la texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(textureFilename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, (rgba ? GL_RGBA : GL_RGB), width, height, 0, (rgba ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout <<red<< "Failed to load texture" << white << std::endl;
	}
	//libérer l’espace mémoire contenant l’image
	stbi_image_free(data);

	return textureID;
}

void VAOModel::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, m_Vertices.size()-144, GL_UNSIGNED_INT, 0);
}

unsigned int VAOModel::getVAOID() const
{
	return VAOID;
}

//float* VAOModel::getVertices() const
//{
//	return vertices;
//}




