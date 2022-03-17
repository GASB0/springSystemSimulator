#include "./headers/MY_DATATYPES.h"
#include "./headers/SHADER_H.h"
#include "./headers/TEXTURE_H.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <array>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <tuple>
#include <vector>

// Dimensiones de nuestra cuadrilla
uint width, height;

const int SCR_WIDTH = 600;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, std::vector<particle> &Ps) {
  (void)window;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    Ps[0].pos.y += 0.01;

  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    Ps[0].pos.y -= 0.01;

  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    Ps[0].pos.x += 0.01;

  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    Ps[0].pos.x -= 0.01;
}

int main() {
  //------------------------------------------------------//
  //------------------------------------------------------//
  // Sección de la configuración para mostrar en pantalla
  //------------------------------------------------------//
  //------------------------------------------------------//
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(
      SCR_WIDTH, SCR_HEIGHT, "Retarded spring system simulator", NULL, NULL);

  if (window == NULL) {
    std::cout << "Carnal, no se pudo crear la ventana" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Carnal, no se pudo inicializar GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  unsigned int VAO, VBO, EBO;

  //------------------------------------------------------//
  //------------------------------------------------------//
  // Sección de la configuración de las partículas.
  //------------------------------------------------------//
  //------------------------------------------------------//

  width = 10;
  height = 10;

  std::vector<particle> particleSystem;
  particleSystem.reserve(height * width);

  std::tuple<int, int> aMatPos;
  std::tuple<float, float, float> apos, avel, aforce;
  avel = std::make_tuple((float)0.0, (float)0.0, (float)0.0);
  aforce = std::make_tuple((float)0.0, (float)0.0, (float)0.0);
  float mass = 1.00;

  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      apos = std::make_tuple((((float)j / (float)width)) - 0.5,
                             (float)(height - (i + 1)) / (float)height,
                             0.0); // Esta cosa son las coordenadas en
                                   // el plano cartesiano R^3 x,y,z.

      aMatPos = std::make_tuple(i, j); // Esta cosa son la "posición" de las
                                       // partículas en una matrix para su
                                       // acceso.

      particleSystem.push_back(
          particle(apos, avel, aforce, mass, aMatPos)); // Inicialización de
                                                        // la partícula P_ij.
    }
  }

  //------------------------------------------------------//
  // Asignación de vecinos con respecto a la posición en matriz
  //------------------------------------------------------//

  // Lista de vecinos relativos a la posición en la matriz
  std::vector<std::tuple<int, int>> listaVecinos;
  listaVecinos.reserve(8);
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (not(j == 0 and i == 0)) {
        std::tuple<int, int> foo(i, j); // Up
        listaVecinos.push_back(foo);
      }
    }
  }

  // Asignación de vecinos a las partículas
  for (uint i = 0; i < height; i++) { // Cuello de botella
    for (uint j = 0; j < width; j++) {
      particleSystem.at(j + (i * width))
          .setRelativeNeighbours(width, height, listaVecinos, particleSystem);
    }
  }

  // Llenando el buffer de índices para el dibujado del sistema.
  particleSystem.shrink_to_fit();
  std::vector<int> EBOIndices;
  EBOIndices.clear();
  for (uint i = 0; i < height; i++) {
    for (uint j = 0; j < width; j++) {
      // Iterando sobre cada una de las partículas de mi sistema
      particle currentPart = particleSystem.at(j + i * width);

      for (uint neighInd = 0; neighInd < currentPart.neighbours.size();
           neighInd++) {
        int p = currentPart.neighbours.at(neighInd)->matPos.i;
        int q = currentPart.neighbours.at(neighInd)->matPos.j;

        EBOIndices.push_back(j + i * width);
        EBOIndices.push_back(q + p * width);
      }
    }
  }

  //----------------------------------------------------------//
  // Asignación de la data en los buffers //
  //----------------------------------------------------------//
  // TODO: Hazle un wrapper de esto de graficado
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // Contenedor del buffer de datos

  // Buffer data
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (int)particleSystem.size() * sizeof(particle),
               &particleSystem[0], GL_DYNAMIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle), (void *)0);
  glEnableVertexAttribArray(0);

  glad_glGenBuffers(1, &EBO);
  glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOIndices.size() * sizeof(int),
                    &EBOIndices[0], GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  Shader myShader("./shaders/gridShader.vs", "./shaders/gridShader.fs");

  //------------------------------------------------------//
  // Calculando las fuerzas sobre cada una de las partículas
  //------------------------------------------------------//
  vec3 springTerm;
  (void)springTerm;
  vec3 ParticleNetForce = {0.0, 0.0, 0.0};
  vec3 netNeighbourForce = {0.0, 0.0, 0.0};
  vec3 gForce = {0.0, 0.0, 0.0};
  vec3 extForces = {0.0, 0.0, 0.0};

  const float springsLength =
      1.3 * 2 / ((float)height + (float)width); // Longitud de los "resortes"
  const float K = 800;                          // Constante de los resortes
  float dt = 0.0001;
  particle currentParticle = particleSystem.at(0);
  Timer<std::chrono::microseconds, std::chrono::steady_clock> timer;

  while (!glfwWindowShouldClose(window)) {
    timer.tick();
    for (uint i = 1; i < height; i++) {
      for (uint j = 0; j < width; j++) {
        // Cambiandole el nombre a la partícula para no confundirme
        currentParticle = particleSystem[j + i * width];

        // Calculando la fuerza neta ejercida por los vecinos a mi partícula
        netNeighbourForce = {0.0, 0.0, 0.0};
        for (int neighInd = 0;
             neighInd < (int)currentParticle.neighbours.size(); neighInd++) {

          // Encontrando la dirección de la fuerza:
          springTerm = ((currentParticle.neighbours.at(neighInd)->pos -
                         currentParticle.pos)
                            .direction()) *
                       springsLength;

          netNeighbourForce += (currentParticle.neighbours.at(neighInd)->pos -
                                (currentParticle.pos + springTerm)) *
                               K;
        }

        // Añadiendo la fuerza de la gravedad
        gForce = {0.0, -9.8, 0.0};

        // Añadiendo fuerza externa
        extForces = {20.0f * (float)std::sin(3.0f * glfwGetTime()), 0.0, 0.0};

        // Fuerza neta en la partícula(ij)
        ParticleNetForce =
            gForce + netNeighbourForce + extForces; // Esta es la fuerza
                                                    // que tenemos en esta
                                                    // partícula en este
                                                    // instante.

        // Calculando la velcidad de la partícula
        currentParticle.vel =
            ((currentParticle.netFrc + ParticleNetForce) / (float)2) * dt /
            currentParticle.mass;

        // Calculando la posición de la partícula
        currentParticle.pos += currentParticle.vel * dt;

        // Actualizando la fuerza que sufre la patícula luego de haber calculado
        // su nueva velocidad.
        currentParticle.netFrc = ParticleNetForce;

        particleSystem[j + width * i] =
            currentParticle; // Actualizando el valor
                             // de la partícula i,j
      }
    }

    //----------------------------------------------------------//
    //----------------------------------------------------------//
    //  Sección del dibujado.
    //----------------------------------------------------------//
    //----------------------------------------------------------//
    myShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    particleSystem.size() * sizeof(particle),
                    &particleSystem[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glPointSize((float)10);
    glDrawElements(GL_POINTS, (uint)EBOIndices.size(), GL_UNSIGNED_INT, 0);
    glDrawElements(GL_LINES, (uint)EBOIndices.size(), GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    processInput(window, particleSystem);
    glfwPollEvents();

    //----------------------------------------------------------//
    // Recalculando el diferencial de tiempo
    //----------------------------------------------------------//
    timer.tock();
    dt = timer.duration().count() / (float)1000000; // Recalculando el
                                                    // diferencial de tiempo
  }

  return 0;
}
