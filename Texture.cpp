#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h";
#include <GL\glew.h>

int width;
int height;
int nrChannels; // nb de cannaux de couleurs
unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
