#include <array>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <tuple>
#include <vector>

// Recuerda de tus clases de física que con sólo la posición y
// la velocidad puedes determinar el estado de una partícula.

class particle {
public:
  // Propiedades físicas de nuestra partícula
  struct position {
    float x, y, z;
  } pos;

  struct velocity {
    float x, y, z;
  } vel;

  // Partículas aledañas a nuestra partícula
  struct neighbours {
    particle *upNeighbour;
    particle *downNeighbour;
    particle *leftNeighbour;
    particle *rightNeighbour;
    //
    particle *upleft;
    particle *upright;
    particle *downleft;
    particle *downright;
  } neigh;

  particle(std::tuple<float, float, float> position,
           std::tuple<float, float, float> velocity) {
    // Inicializando las posiciones
    pos.x = std::get<0>(position);
    pos.y = std::get<1>(position);
    pos.z = std::get<2>(position);

    // Inicializando las velocidades
    vel.x = std::get<0>(velocity);
    vel.x = std::get<1>(velocity);
    vel.x = std::get<2>(velocity);
  }
};

// Dimensiones de nuestra cuadrilla
int width, height;

int main() {
  std::vector<particle> elements;
  std::vector<std::vector<particle>> particleSystem;

  // Inicialización de las partículas en una cuadrilla
  // rectangular:
  width = 10;
  height = 10;

  auto apos = std::make_tuple(0.0, 0.0, 0.0);
  auto avel = std::make_tuple(0.0, 0.0, 0.0);

  for (int i = 0; i < height; i++) {
    elements.clear();
    for (int j = 0; j < width; j++) {
      apos = std::make_tuple((float)i, (float)j, 0.0);
      elements.push_back(particle(apos, avel));
    }
    particleSystem.push_back(elements);
  }

  // // Test del almacenamiento de estos valores
  // // Funciona, xdxd
  // for (int i = 0; i < height; i++) {
  //   for (int j = 0; j < width; j++) {
  //     std::cout << particleSystem[i][j].pos.x << ","
  //               << particleSystem[i][j].pos.y << "\t";
  //   }
  //   std::cout << std::endl;
  // }

  return 0;
}
