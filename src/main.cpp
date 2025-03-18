#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "../include/coord.h"
#include "../include/picture.h"

struct CelestialBody {
  string name;
  Coord pos;
  Coord vel;
  double mass;
};

const double M_PER_AU = 1.496e11;
const double M_PER_KM = 1000;

const double G = 6.67430e-11;      // Gravitational constant
const double mMercury = 3.3011e23; // [kg]
const double mVenus = 4.8670e24;
const double mEarth = 5.9722e24;
const double mMars = 6.3900e23;
//   const double mJupiter = 1.8982e27;
//   const double mSaturn = 5.6834e26;
//   const double mUranus = 8.6810e25;
//   const double mNeptune = 1.0241e26;
//   const double mPluto = 1.3030e22;

// ----------------------------------------------------------
//         Name: mercury
//  Acc [m/s/s]: X: 0.02089       Y: 0.07184       Z: 0.00395
//    Vel [m/s]: X: -37177.59990  Y: -10576.72537  Z: -4276.33216
//     Pos [AU]: X: -0.13008      Y: -0.44729      Z: -0.02460
// ----------------------------------------------------------
//         Name: venus
//  Acc [m/s/s]: X: 0.04460       Y: 0.00203       Z: -0.00255
//    Vel [m/s]: X: -2220.13867   Y: -49691.54133  Z: -807.21777
//     Pos [AU]: X: -0.71832      Y: -0.03276      Z: 0.04102
// ----------------------------------------------------------
//         Name: earth
//  Acc [m/s/s]: X: 0.00467       Y: -0.02551      Z: -0.00000
//    Vel [m/s]: X: 51607.28254   Y: -9452.74623   Z: -0.01688
//     Pos [AU]: X: -0.17716      Y: 0.96721       Z: 0.00000
// ----------------------------------------------------------
//         Name: mars
//  Acc [m/s/s]: X: -0.01388      Y: 0.00014       Z: 0.00034
//    Vel [m/s]: X: -551.00146    Y: 52652.44794   Z: 1089.52988
//     Pos [AU]: X: 1.39059       Y: -0.01384      Z: -0.03450

// [m], [m/s]
// CelestialBody mercury = {
//     "Mercury", {5.7910e10, 0, 0}, {0, 4.7870e4, 0}, mMercury};
// CelestialBody venus = {"Venus", {1.0770e11, 0, 0}, {0, 3.5000e4, 0}, mVenus};
// CelestialBody earth = {"Earth", {1.4959e11, 0, 0}, {0, 2.9780e4, 0}, mEarth};

CelestialBody earth = {
    "Earth",
    {-1.756637922977122E-01 * M_PER_AU, 9.659912850526895E-01 * M_PER_AU,
     2.020629118443605E-04 * M_PER_AU},
    {-2.979426007051617E+01 * M_PER_KM, -5.469294939330306E+00 * M_PER_KM,
     1.817836783024607E-04 * M_PER_KM},
    mEarth};

// CelestialBody mars = {"Mars", {2.2794e11, 0, 0}, {0, 2.4100e4, 0}, mMars};
//   CelestialBody  jupiter = {{7.7857e11, 0, 0}, {0, 1.3070e4, 0}, mJupiter};
//   CelestialBody  saturn = {{1.4335e12, 0, 0}, {0, 9.6900e3, 0}, mSaturn};
//   CelestialBody  uranus = {{2.8725e12, 0, 0}, {0, 6.8100e3, 0}, mUranus};
//   CelestialBody  neptune = {{4.4951e12, 0, 0}, {0, 5.4300e3, 0}, mNeptune};
//   CelestialBody  pluto = {{5.9064e12, 0, 0}, {0, 4.7400e3, 0}, mPluto};

std::vector<CelestialBody> planets{earth};

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = -G * (p1.mass + p2.mass);
  const double rSquared = p2.pos.distSquared(p1.pos); //[meters] distance

  return p1.pos * gravitationalFactor / (sqrt(rSquared) * rSquared);
}

Coord getTotalAcc(const CelestialBody &p) {
  static double mSun = 1.9891e30; // [kg]
  static CelestialBody sun = {"Sun", {0, 0, 0}, {0, 0, 0}, mSun};
  Coord acc{0, 0, 0};
  acc = std::accumulate(planets.begin(), planets.end(), acc,
                        [&](const Coord &totalAcc, const CelestialBody &other) {
                          return p.mass != other.mass
                                     ? totalAcc + calcAcc(p, other)
                                     : totalAcc;
                        });

  acc += calcAcc(p, sun);
  return acc;
};

CelestialBody rungeKuttaStep(const CelestialBody &p, int dt) {

  // Calculate Runge-Kutta terms
  Coord k1v = getTotalAcc(p) * dt;
  Coord k1r = p.vel * dt;
  CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                p.mass};

  Coord k2v = getTotalAcc(k1CelestialBody) * dt;
  Coord k2r = (p.vel + k1v * 0.5) * dt;
  CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                p.mass};

  Coord k3v = getTotalAcc(k2CelestialBody) * dt;
  Coord k3r = (p.vel + k2v * 0.5) * dt;
  CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  Coord k4v = getTotalAcc(k3CelestialBody) * dt;
  Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return updatedBody;
}

int main() {
  const int picWidth = 300;
  const int picHeight = 300;
  const int picCenter = picWidth / 2;
  Picture pic(picWidth, picHeight, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 0, 0);

  double secondsPerYear = 31536000;
  int dt = 600; // 10-minute intervals
  const int steps = secondsPerYear / dt;

  for (int i = 0; i < steps; i++) {
    std::vector<CelestialBody> updatedBody;
    for (const auto &p : planets) {
      updatedBody.push_back(rungeKuttaStep(p, dt));
    }
    planets = updatedBody;

    for (CelestialBody p : planets) {
      Coord pos = p.pos / M_PER_AU;
      int x = scaleValue(pos.x, 2, 150) + picWidth / 2;
      int y = scaleValue(-pos.y, 2, 150) + picHeight / 2;
      pic.set(x, y, 0, 255, 0);
    }
  }

  //   for (CelestialBody p : planets) {
  //     Coord pos = p.pos / 1.496e+11;
  //     int x = scaleValue(pos.x, 2, 150) + picWidth / 2;
  //     int y = scaleValue(-pos.y, 2, 150) + picHeight / 2;
  //     pic.set(x, y, 0, 255, 0);
  //   }

  for (CelestialBody body : planets) {
    std::cout << "----------------------------------------------------------\n";
    std::cout << setw(14) << "Name: " << body.name << "\n";
    Coord acc = getTotalAcc(body);
    std::cout << setw(14) << "Acc [m/s/s]: ";
    acc.print();
    Coord vel = body.vel;
    std::cout << setw(14) << "Vel [m/s]: ";
    vel.print();
    Coord pos = body.pos / 1.496e+11;
    std::cout << setw(14) << "Pos [AU]: ";
    pos.print();
  }

  pic.save("result.png");
  return 0;
}
