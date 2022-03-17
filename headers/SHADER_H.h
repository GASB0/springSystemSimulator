#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  // Program ID
  unsigned int ID;

  // Lee el source del shader y lo compila.
  // Este constructor toma como argumento las direcciones de los
  // sources de los shaders para vértices y para fragmentos y
  // los procesa.
  Shader(const char *vertexPath, const char *fragmentPath);

  // Activa(pone en contexto) el shader
  void use();

  // Método para setear algún valor de uniforme booleano a mi shader.
  void setBool(const std::string &name, bool value) const;

  // Método para setear algún valor de uniforme entero a mi shader.
  void setInt(const std::string &name, int value) const;

  // Método para setear algún valor de uniforme float a mi shader.
  void setFloat(const std::string &name, float value) const;
};

inline Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  //------------------------------------------------------------//
  // Lectura del sourcecode de los shaders a utilizar
  //------------------------------------------------------------//
  // Retriving vertex/fragments from a file
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Asegurando de que estos objetos puedan tirar excepciones
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // Abrir archivos
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // Leer el contenido del buffer del archivo a stream
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // cerrando handlers de archivos
    vShaderFile.close();
    fShaderFile.close();
    // convirtiendo streams a strings
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure const &e) {
    (void)e;
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ"
              << "\n";
  }
  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();

  //------------------------------------------------------------//
  // Compilación de los shaders
  //------------------------------------------------------------//
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  //------------------------------------------------------------//
  // Linking de los shaders
  //------------------------------------------------------------//

  ID = glCreateProgram();

  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

inline void Shader::use() { glUseProgram(ID); }

inline void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

inline void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

inline void Shader::setFloat(const std::string &name, float value) const {
  glad_glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

// inline void Shader::setMatrix(const std::string &name, glm::mat4) {
//   glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), value);
// }

#endif
