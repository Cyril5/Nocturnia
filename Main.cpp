#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Shader.h"
#include <cmath>
#include "stb_image.h"

int main()
{

	sf::ContextSettings settings;
	settings.depthBits = 24; // nombre de bits par pixel pour le depth buffer (16,24 ou 32)
	settings.stencilBits = 8; // nombre de bits par pixel pour le stencil buffer (0 pour ne pas en cr�er)
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	//settings.attributeFlags = sf::ContextSettings::Core;

	//float vertices[] = { // Sommets

	//// positions
	//0.5f, -0.5f, 0.0f,
	//-0.5f, -0.5f, 0.0f,
	//0.0f,  0.5f, 0.0f,
	//};

	float vertices[] = { // Sommets

		// positions			// colors			// texture coords
		0.5f,0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f,1.0f, // en haut � droite
		0.5f,-0.5f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f,0.0f, // en bas � droite
		-0.5f,-0.5f, 0.0f,		0.0f,0.0f,1.0f,		0.0f, 0.0f, // bottom left
		-0.5f,0.5f,0.0f,		1.0f,1.0f,0.0f,		0.0f, 1.0f, // top left
	};

	
	// EBO : Element Buffer Object
	unsigned int indices[] = {
		0,1,3, // Premier triangle
		1,2,3 // Deuxi�me triangle
	};


	sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	glewExperimental = GL_TRUE; // N�cessaire dans le profil de base
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Clock clock; // d�marre le chrono pour pouvoir obtenir le temps �coul� depuis le d�but de l'application


	/*
	*----------------------------------------------------------------------------------------------------
	*											DRAW OPENGL
	* --------------------------------------------------------------------------------------------------
	*/

	Shader myShaderProgram("myShader.vsd", "myShader.fsd");

	//VBO : Vertex Buffer Object
	//Ce VBO d�fini un vertex shader et un fragment shader et dit � OpenGL comment lier les donn�es des sommets aux attributs du vertex shader
	unsigned int VBO;
	glGenBuffers(1, &VBO);


	// VAO : Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	//EBO : Element Buffer Object
	unsigned int EBO;
	glGenBuffers(1, &EBO);


	// // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	// D�but du VAO qui contiendra des VBO ou EBO
	glBindVertexArray(VAO);
		// 2 .Copier les sommets du triangle dans un tampon pour qu�OpenGL les utilise
		// D�but du VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 3. copier le tableau d�indices dans un tampon d��l�ments
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Etablir les pointeurs d'attributs des sommets
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Etablir les pointeurs d'attributs de couleurs
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3*sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	
		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		// Fin du VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// Fin du VAO
	//glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // mode fil de fer

	// Load and create texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// d�finit les options de la texture actuellement li�e
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// charge et g�n�re la texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout <<red<< "Failed to load texture" << white << std::endl;
	}
	//lib�rer l�espace m�moire contenant l�image
	stbi_image_free(data);
	
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;


	// boucle principal
	bool running = true;
	while (running)
	{
		// gestion des �venements
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// on stoppe le programme
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// on ajuste le viewport lorsque la fen�tre est redimensionn�e
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// effacement tampons de couleur/profondeur
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind texture
		glBindTexture(GL_TEXTURE_2D, texture);

		//------------------------------------------------------------------------------------------------------------
		//												Rendu du triangle
		
		myShaderProgram.use();
		glBindVertexArray(VAO);
		
		//glDrawArrays(GL_TRIANGLES, 0, 3); // dessiner un triangle � 3 sommets
		// Dessiner les deux triangles gr�ce aux tableaux d'indices
		glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0);
		
		//------------------------------------------------------------------------------------------------------------

		//fin de la trame courante (en interne, �change les deux tampons de rendu)
		window.display();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	myShaderProgram.deleteProgram();


	//window.close();
	return 0;
}
