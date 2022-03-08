#include "./headers/MY_DATATYPES.h"
#include <array>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>

// Dimensiones de nuestra cuadrilla
int width, height;

int main() {
  width = 3;
  height = 3;

  std::vector<particle> elements;
  std::vector<std::vector<particle>> particleSystem;
  elements.reserve(width);
  particleSystem.reserve(height);

  std::tuple<int, int> aMatPos;
  std::tuple<float, float, float> apos, avel, aforce;
  avel = std::make_tuple((float)0.0, (float)0.0, (float)0.0);
  aforce = std::make_tuple((float)0.0, (float)0.0, (float)0.0);
  float mass = 1;

  for (int i = 0; i < height; i++) {
    elements.clear();
    for (int j = 0; j < width; j++) {
      apos = std::make_tuple((float)j, (float)(height - (i + 1)),
                             0.0); // Esta cosa son las coordenadas en
                                   // el plano cartesiano R^3 x,y,z.

      aMatPos = std::make_tuple(i, j); // Esta cosa son la "posición" de las
                                       // partículas en una matrix para su
                                       // acceso.

      elements.push_back(
          particle(apos, avel, aforce, mass, aMatPos)); // Inicialización de
                                                        // la partícula P_ij.
    }
    particleSystem.push_back(elements);
  }

  //------------------------------------------------------//
  // Asignación de vecinos con respecto a la posición en matriz
  //------------------------------------------------------//
  std::tuple<int, int> foo1(-1, 0);  // Up
  std::tuple<int, int> foo2(1, 0);   // Down
  std::tuple<int, int> foo3(0, -1);  // Left
  std::tuple<int, int> foo4(0, 1);   // Right
  std::tuple<int, int> foo5(-1, -1); // UpLeft
  std::tuple<int, int> foo6(-1, 1);  // UpRight
  std::tuple<int, int> foo7(1, -1);  // DownLeft
  std::tuple<int, int> foo8(1, 1);   // DownRight

  std::vector<std::tuple<int, int>> listaVecinos;
  listaVecinos.reserve(8);
  listaVecinos.push_back(foo1);
  listaVecinos.push_back(foo2);
  listaVecinos.push_back(foo3);
  listaVecinos.push_back(foo4);
  listaVecinos.push_back(foo5);
  listaVecinos.push_back(foo6);
  listaVecinos.push_back(foo7);
  listaVecinos.push_back(foo8);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      particleSystem.at(i).at(j).setRelativeNeighbours(listaVecinos,
                                                       particleSystem);
    }
  }

  //------------------------------------------------------//
  // Calculando las fuerzas sobre cada una de las partículas
  //------------------------------------------------------//
  vec3 ParticleNetForce = {0.0, 0.0, 0.0};
  vec3 netNeighbourForce = {0.0, 0.0, 0.0};
  vec3 gForce = {0.0, -9.8, 0.0};
  vec3 extForces = {0.0, 0.0, 0.0};

  const float springsLength = 1.0; // Longitud de los "resortes"
  const float K = 1;               // Constante de los resortes
  float dt = 0.01;

  (void)springsLength;
  Timer<std::chrono::microseconds, std::chrono::steady_clock> timer;

  for (int m = 0; m < 100000; m++) {
    // TODO: Calcular el diferencial de tiempo para cada iteración del loop en
    // donde se estará ejecutando la simulación
    timer.tick();
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {

        // Calculando la fuerza neta ejercida por los vecinos a mi partícula
        netNeighbourForce = {0.0, 0.0, 0.0};
        for (int neighInd = 0;
             neighInd < (int)particleSystem.at(i).at(j).neighbours.size();
             neighInd++) {

          // Encontrando la dirección de la fuerza:
          // TODO: Revisar si esto realmente funciona
          vec3 springTerm =
              ((vec3){0.0, 0.0, 0.0} -
               (particleSystem.at(i).at(j).neighbours.at(neighInd).pos -
                particleSystem.at(i).at(j).pos)
                   .direction()) *
              springsLength;

          netNeighbourForce +=
              (particleSystem.at(i).at(j).neighbours.at(neighInd).pos -
               (particleSystem.at(i).at(j).pos) + springTerm) *
              K;
        }

        // Añadiendo la fuerza de la gravedad
        gForce = {0.0, -9.8, 0.0};

        // Añadiendo fuerza externa
        extForces = {0.0, 0.0, 0.0}; // No vamos a tener
                                     // ninguna fuerza
                                     // externa por estos momentos

        // Fuerza neta en la partícula(ij)
        ParticleNetForce =
            gForce + netNeighbourForce + extForces; // Esta es la fuerza
                                                    // que tenemos en esta
                                                    // partícula en este
                                                    // instante.

        // Calculando la velcidad de la partícula
        particleSystem.at(i).at(j).vel =
            ((particleSystem.at(i).at(j).netFrc + ParticleNetForce) /
             (float)2) *
            dt / particleSystem.at(i).at(j).mass;

        // Actualizando la fuerza que sufre la patícula luego de haber calculado
        // su nueva velocidad.
        particleSystem.at(i).at(j).netFrc = ParticleNetForce;

        // Calculando la posición de la partícula
        particleSystem.at(i).at(j).pos += particleSystem.at(i).at(j).vel * dt;
      }
      std::cout << "\n";
      std::cout << "\n";
    }
    timer.tock();
    dt = timer.duration().count() / (float)1000000; // Recalculando el
                                                    // diferencial de tiempo
  }

  return 0;
}
