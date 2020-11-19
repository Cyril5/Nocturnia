#include "Mesh.h";

using namespace NocturniaEngine::Render;

		Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		{
			m_Vertices = vertices;
			m_Indices = indices;
			m_Textures = textures;

            // now that we have all the required data, set the vertex buffers and its attribute pointers.
            setupMesh();

		}
		
        void Mesh::draw(Shader shader) const
		{
            unsigned short diffuseNr = 1;
            unsigned short specularNr = 1;
            unsigned short normalNr = 1;
            unsigned short heightNr = 1;

            for (unsigned short i = 0; i < m_Textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // Activation de l’unité de texture adéquate avant liaison
                // récupère le numéro de la texture (le N dans diffuse_textureN)
                std::string number;
                std::string name = m_Textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++);
                shader.setFloat(("material." + name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
            }
            glActiveTexture(GL_TEXTURE0);
            // affiche le mesh
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
		}

		void Mesh::setupMesh()
		{
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
            // position des sommets
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // normales
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            // coordonnées de texture
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
            glBindVertexArray(0);
		}
