# Celestial Mechanics Demo

Calculates state vectors of the solar system's planets at an arbitrary moment in time. Outputs both a PNG of the system in the XY plane and formatted distances in the console.

## Get Started

### Dependencies
- **Make**
- **C++17** or higher

### Build and Run
```sh
make && ./build/main.o
# OR
make run
```

## Implementation ##
Uses two separate strategies.

1. For the Terrestrial planets (Mercury - Mars), the position of the planet is approximated using properties of a Kelperian Orbit.
	* normalize the Mean Anomaly (M) to the specified date
	* solve for Eccentric Anomaly (E) using Newton's Method
	* convert position along the orbital plane into heliocentric 3d cartesian coordinates.

2. For the Jovian planets (Jupiter - Neptune), the position of the planet is approximated by using an N-body model
	* follows the steps for Terrestrial planets (without normalizing to target date) in order to obtain initial state vectors
	* calculates the acceleration produced from other planets and the sun.
	* numerically integrates all relevant acceleration vectors for each planet per time step
	* iterates over time steps until target time is reached

## Glossary ##
**Astronomical units (AU)**
: the mean distance between the Earth and the Sun

**Orbital Elements**
: a set of parameters designed to approximate the planet's orbit at a given Epoch

**J2000 Epoch**
: Jan 1, 2000 — the reference point for orbital elements used in this program
