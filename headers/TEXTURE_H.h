#ifndef TEXTURE_H
#define TEXTURE_H

#include "../glad/glad.h"
#include "./stb_image.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Objeto wrapper para cargar texturas en openGL

class Texture {
  // TODO: Implementar esta clase de tal forma que pueda cargar múltiples
  // texturas dando como argumentos una lista de rutas de las texturas
  // deseadas.
private:
  static size_t count;

public:
  short instanceID;
  unsigned int ID;

  // copy constructor:
  Texture(const Texture &) { ++count; }
  // Destructor
  inline ~Texture() { --count; }

  // Constructor
  Texture(const char *texturePath);

  // Esta función se encarga de poner en uso (bind) la textura correspondiente
  // al objeto y darle un número de texture Unit.
  void useTexture();
};

inline Texture::Texture(const char *texturePath) {
  /* ++objectInstances; */
  ++count;
  instanceID = (short)count;
  // ------------------------------------------------------ //
  // Configuración de las texturas
  // ------------------------------------------------------ //
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline void Texture::useTexture() {
  glActiveTexture(GL_TEXTURE0 + instanceID);
  glBindTexture(GL_TEXTURE_2D, ID);
}

#endif
