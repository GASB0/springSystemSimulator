#ifndef MY_DATATYPES
#include <array>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <tuple>
#include <vector>
template <class DT = std::chrono::milliseconds,
          class ClockT = std::chrono::steady_clock>
class Timer {
  using timep_t = decltype(ClockT::now());

  timep_t _start = ClockT::now();
  timep_t _end = {};

public:
  void tick() {
    _end = timep_t{};
    _start = ClockT::now();
  }

  void tock() { _end = ClockT::now(); }

  template <class duration_t = DT> auto duration() const {
    // Use gsl_Expects if your project supports it.
    assert(_end != timep_t{} && "Timer must toc before reading the time");
    return std::chrono::duration_cast<duration_t>(_end - _start);
  }
};

struct vec3 {
public:
  float x, y, z;
  template <typename T> T operator=(const T aTriple) {
    x = aTriple.x;
    y = aTriple.y;
    z = aTriple.z;
    return {x = aTriple.x, y = aTriple.y, z = aTriple.z};
  };

  template <typename T> T operator+(const T &aTriple) {
    return {aTriple.x + x, aTriple.y + y, aTriple.z + z};
  };

  template <typename T> T operator+=(const T &aTriple) {
    return {x += aTriple.x, y += aTriple.y, z += aTriple.z};
  };

  template <typename T> T operator-(const T &aTriple) {
    return {x - aTriple.x, y - aTriple.y, z - aTriple.z};
  };

  vec3 operator-() { return {-x, -y, -z}; };

  template <typename T> T operator-=(const T &aTriple) {
    return {x -= aTriple.x, y -= aTriple.y, z -= aTriple.z};
  };

  vec3 operator+(const float &aScalar) {
    return {aScalar + x, aScalar + y, aScalar + z};
  };

  vec3 operator*(const float &aScalar) {
    return {aScalar * x, aScalar * y, aScalar * z};
  };

  vec3 operator*=(const float &aScalar) {
    this->x *= aScalar;
    this->y *= aScalar;
    this->z *= aScalar;
    return {x *= aScalar, y *= aScalar, z *= aScalar};
  };

  vec3 operator/(const float &aScalar) {
    return {x / aScalar, y / aScalar, z / aScalar};
  };

  vec3 operator/=(const float &aScalar) {
    this->x /= aScalar;
    this->y /= aScalar;
    this->z /= aScalar;
    return {x /= aScalar, y /= aScalar, z /= aScalar};
  };

  float norm() {
    float norm = std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
    return norm;
  };

  vec3 direction() { return {x / norm(), y / norm(), z / norm()}; }
  friend std::ostream &operator<<(std::ostream &out, const vec3 &aTriplet);
};

inline std::ostream &operator<<(std::ostream &out, const vec3 &aTriplet) {
  out << "(" << aTriplet.x << "," << aTriplet.y << "," << aTriplet.z << ')';
  return out;
}

// Recuerda de tus clases de física que con sólo la posición y
// la velocidad puedes determinar el estado de una partícula.
class particle {
public:
  //------------------------------------------------------//
  // Propiedades físicas de nuestra partícula
  //------------------------------------------------------//
  struct position : vec3 {
    template <typename T> T operator=(const T &aTriplet) {
      x = aTriplet.x;
      y = aTriplet.y;
      z = aTriplet.z;
      return {x + aTriplet.x, y + aTriplet.y, z + aTriplet.z};
    };
  } pos;

  struct velocity : public vec3 {
    template <typename T> T operator=(const T &aTriplet) {
      x = aTriplet.x;
      y = aTriplet.y;
      z = aTriplet.z;
      return {x + aTriplet.x, y + aTriplet.y, z + aTriplet.z};
    };
  } vel;

  struct netForce : vec3 {
    template <typename T> T operator=(const T &aTriplet) {
      x = aTriplet.x;
      y = aTriplet.y;
      z = aTriplet.z;
      return {x + aTriplet.x, y + aTriplet.y, z + aTriplet.z};
    };
  } netFrc;

  struct matrixPos {
    int i, j;
  } matPos;

  float mass;
  // Partículas aledañas a nuestra partícula
  std::vector<particle *> neighbours;

  // Esta función recibe como argumento un vector de
  // tuplas con las "posiciones relativas" correspondientes
  // a los vecinos que se espera que tenga la partícula
  void setRelativeNeighbours(
      uint colNum, uint rowNum,
      std::vector<std::tuple<int, int>> &RelativeNeighboursList,
      std::vector<particle> &aParticleSystem) {

    int p, q;
    for (int i = 0; i < (int)RelativeNeighboursList.size(); i++) {
      p = std::get<0>(RelativeNeighboursList.at(i));
      q = std::get<1>(RelativeNeighboursList.at(i));

      if (matPos.i + p < 0 || matPos.j + q < 0 || matPos.i + p >= (int)rowNum ||
          matPos.j + q >= (int)colNum) {
      } else {
        neighbours.push_back(
            &aParticleSystem.at(colNum * (matPos.i + p) + (matPos.j + q)));
      }
    }

    neighbours.shrink_to_fit();
  }

  particle(std::tuple<float, float, float> position,
           std::tuple<float, float, float> velocity,
           std::tuple<float, float, float> force, float aMass,
           std::tuple<int, int> matPosition) {
    // Inicializando las posiciones
    pos.x = std::get<0>(position);
    pos.y = std::get<1>(position);
    pos.z = std::get<2>(position);

    // Inicializando las velocidades
    vel.x = std::get<0>(velocity);
    vel.y = std::get<1>(velocity);
    vel.z = std::get<2>(velocity);

    // Inicializando las fuerzas
    netFrc.x = std::get<0>(force);
    netFrc.y = std::get<1>(force);
    netFrc.z = std::get<2>(force);

    // Inicializando la masa
    mass = aMass;

    // Inicializando las posiciones en la "matriz"
    matPos.i = std::get<0>(matPosition);
    matPos.j = std::get<1>(matPosition);

    // Reservando la memoria que creo que usaré
    neighbours.reserve(8);
  }
};

#endif
