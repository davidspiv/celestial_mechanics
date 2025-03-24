# Celestial Mechanics Demo

Calculates state vectors of the solar system's planets at an arbitrary moment in time. Outputs both a PNG of the system in the XY plane and formatted distances in the console.

## Get Started

### Dependencies
- **Make**
- **C++17** or higher

### Build and Run
```
make && ./build/main`
```
OR
```
make run
```

## Implementation ##
Uses two separate strategies.

A. **Terrestrial Planets** *(Mercury – Mars)*\
   The position of each planet is calculated using properties of a Keplerian Orbit:
   1. Normalize the Mean Anomaly to the specified date.
   2. Solve for the Eccentric Anomaly using Newton's Method.
   3. Convert the position along the orbital plane into heliocentric 3D Cartesian coordinates.

B. **Jovian Planets** *(Jupiter – Neptune)*\
   To account for perturbations in the orbits near Jupiter and Saturn, the position of each planet is approximated using an N-body model:
   1. Obtain initial state vectors by following the same Keplerian orbit steps used for Terrestrial planets (excluding normalization to the target date).
   2. Calculate an acceleration vector for each planet by summing the gravitational effects from the Sun and other planets based on their masses and relative positions.
   3. Use the 4th-order Runge-Kutta method to numerically integrate the acceleration vectors twice — once for velocity and again for position — over the specified time step.
   4. Iterate over time steps, repeating steps 2–3 until the target time is reached.

## Glossary ##
**Astronomical units (AU)**
: the mean distance between the Earth and the Sun

**Orbital Elements**
: a set of parameters designed to approximate the planet's orbit at a given epoch

**J2000 Epoch**
: Jan 1, 2000 — the reference point for orbital elements used in this program
