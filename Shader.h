#ifndef SHADER_H // Evite d'inclure plusieurs fois ce fichier
#define SHADER_H

//#include <glad/glad.h> // inclure glad pour disposer de tout en-tête OpenGL

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL\glew.h>
#include "ConsoleColor.h";
#include "glm/glm.hpp";

class Shader 
{
	public:
		unsigned int ID; // shader program Id

		// constructeur
		Shader(const char* vertexFilename, const char* fragmentFilename) 
		{

			// récupère le code du fichier vertex/fragment shader
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;

			// s'assure que les objets ifstream peuvent envoyer des exceptions:
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				// ouverture des fichiers
				vShaderFile.open(vertexFilename);
				fShaderFile.open(fragmentFilename);

				// Lecture des fichiers et place le contenu dans des flux
				std::stringstream vShaderStream;
				std::stringstream fShaderStream;

				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// fermeture fichier
				vShaderFile.close();
				fShaderFile.close();

				// convert stream into string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << red << "ERREUR: Impossible de lire le fichier de shader" << white << std::endl;
			}

			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			unsigned int vertex;
			unsigned int fragment;
			int success;
			char infoLog[512];

			// vertex shader
			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vShaderCode, NULL);
			glCompileShader(vertex);

			// affiche les erreurs de compilation si besoin
			glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertex, 512, NULL, infoLog);
				std::cout << red << "Erreur pendant la compilation du Shader de Vertices (Vertex Shader)\n" << infoLog << white << std::endl;
			};

			// fragment shader
			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fShaderCode, NULL);
			glCompileShader(fragment);

			glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragment, 512, NULL, infoLog);
				std::cout << red << "Erreur pendant la compilation du Shader de Fragment (Fragment Shader)\n" << infoLog << white << std::endl;
			};

			// Shader Program
			ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);

			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cout << red << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << white << std::endl;
			}
			glDeleteShader(vertex);
			glDeleteShader(fragment);
		}

		void use()
		{
			glUseProgram(ID);
		}

		void deleteProgram()
		{
			glDeleteProgram(ID);
		}

		// fonctions utiles pour l'uniform
		void setBool(const std::string &name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}

		void setInt(const std::string &name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}

		//void setFloatV(const std::string &name, GLfloat *value[]) const
		//{
		//	glUniform4f(glGetUniformLocation(ID, name.c_str()), value);
		//}

		void setFloat(const std::string &name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}

		void setMat4(const std::string& name, const glm::mat4 &mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
		}

		// Récupère la valeur d'un uniform dans le shader
		//getUniform(const std::string &name) const {
		//	return glGetUniformLocation(ID, name.c_str());
		//}
};

#endif