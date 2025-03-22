#include <cmath>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/getInitialPlanetState.h"
#include "../include/io.h"
#include "../include/picture.h"
#include "../include/timer.h"
#include "../include/updatePlanetState.h"
#include "../include/util.h"


int main() {
  const double julianDay = getDate();
  //   const double julianDay = 0;
  Timer timer;
  const int dt = 604800; // 1-week intervals
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));

  const int picSideLength = 500;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);

  size_t oneBodyCutoff = 2;
  // parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  populatePlanets(elements, bodies);

  const size_t systemSize = approxSystemSize(elements);


  for (size_t i = 0; i < oneBodyCutoff; i++) {
    populateStateVectors(elements[i], bodies[i], julianDay);
  }

  for (size_t i = oneBodyCutoff; i < elements.size(); i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }

  std::vector<CelestialBody> for1BodyApprox(bodies.begin(),
                                            bodies.begin() + oneBodyCutoff);
  std::vector<CelestialBody> forNBodyApprox(bodies.begin() + oneBodyCutoff,
                                            bodies.end());

  for (int i = 0; i < steps; i++) {
    updateBodies(forNBodyApprox, dt);

    // end up rendering a lot of the same pixels, but checking is slower?
    // drawBodies(bodies, pic, systemSize);
  }

  bodies.clear();
  bodies.reserve(for1BodyApprox.size() + forNBodyApprox.size());
  bodies.insert(bodies.end(), for1BodyApprox.begin(), for1BodyApprox.end());
  bodies.insert(bodies.end(), forNBodyApprox.begin(), forNBodyApprox.end());

  drawBodies(bodies, pic, systemSize, true);

  pic.save("result.png");

  //   for (auto &body : bodies) {
  //     std::cout << body.name << '\n';
  //     std::cout << "POS: ";
  //     body.pos = body.pos / 1000;
  //     body.pos.print();
  //     std::cout << "VEL: ";
  //     body.vel = body.vel / 1000;
  //     body.vel.print();
  //   }
  printResults(bodies);
}

//   CelestialBody mercury = {"mercury",
//                            Coord(-2.105262107563719E+07,
//                            -6.640663812146252E+07,
//                                  -3.492445946196772E+06),
//                            Coord(3.665298704115296E+01,
//                            -1.228983807166655E+01,
//                                  -4.368173036362700E+00),
//                            3.3011e23};
//   CelestialBody venus = {"venus",
//                          Coord(-1.075055502720626E+08,
//                          -3.366520663456144E+06,
//                                6.159219789285429E+06),
//                          Coord(8.891597849735767E-01,
//                          -3.515920774141023E+01,
//                                -5.318594228074627E-01),
//                          4.8675e24};
//   CelestialBody earth = {"earth",
//                          Coord(-2.521092855639125E+07, 1.449279195842548E+08,
//                                -6.164165719002485E+02),
//                          Coord(-2.983983333686836E+01,
//                          -5.207633901882137E+00,
//                                6.168441159859483E-05),
//                          5.97237e24};
//   CelestialBody mars = {"mars",
//                         Coord(2.079950549835042E+08,
//                         -3.143009716235623E+06,
//                               -5.178781243534046E+06),
//                         Coord(1.295003553243561E+00, 2.629442066946597E+01,
//                               5.190097459167351E-01),
//                         6.4171e23};
//   CelestialBody jupiter = {"jupiter",
//                            Coord(5.989091645408236E+08, 4.391225866595105E+08,
//                                  -1.523251063026616E+07),
//                            Coord(-7.901937516118259E+00, 1.116317703174308E+01,
//                                  1.306732148711065E-01),
//                            1.8982e27};
//   CelestialBody saturn = {"saturn",
//                           Coord(9.587063371739676E+08, 9.825652104582239E+08,
//                                 -5.522065631227201E+07),
//                           Coord(-7.428885680405189E+00, 6.738814240737881E+00,
//                                 1.776643606865469E-01),
//                           5.6834e26};
//   CelestialBody uranus = {"uranus",
//                           Coord(2.158774481135283E+09,
//                           -2.054825439981381E+09,
//                                 -3.562364902696371E+07),
//                           Coord(4.637647623549945E+00, 4.627191832185963E+00,
//                                 -4.285552181289942E-02),
//                           8.6810e25};
//   CelestialBody neptune = {"neptune",
//                            Coord(2.514853560730616E+09,
//                            -3.738847414418951E+09,
//                                  1.903940653526902E+07),
//                            Coord(4.465802635366915E+00, 3.075682319815708E+00,
//                                  -1.665662221039170E-01),
//                            1.02413e26};
//   CelestialBody sun = {"sun", Coord(), Coord(), M_SUN};


//   std::vector<CelestialBody> bodies = {mercury, venus,  earth,   mars,
//   jupiter,
//                                        saturn,  uranus, neptune, sun};

//   for (auto &body : bodies) {
//     body.pos = body.pos * 1000;
//     body.vel = body.vel * 1000;
//   }

//   const size_t systemSize = 31;
