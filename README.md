## Purpose ##
Calculate the distance between the planets and earth given a specified date.

## Definitions ##
**Astronomical units (AU)**
: the mean distance between the Earth and the Sun

**Orbital Elements**
: parameters that specify the angle of orbit in three dimensions and the position of the planet along that orbit (planets.json)

**J2000 Epoch**
: Jan 1, 2000. The moment in time the orbital elements were measured

**Plane of the ecliptic**
: the imaginary plane containing Earth's path around the Sun

**Semi-major axis**
: [units: AU] half the length of the long axis of the ellipse. Worded another way, the largest radius that exists in the satellite's orbit

**Eccentricity**
: [units: N/A] ratio between the focal distance (which is relative to J2000) and the semi-major axis

**Orbital inclination**
: [units: degrees] The tilt of the planet's orbit around the sun relative to plane of the ecliptic

**Longitude of the ascending node**
: [units: degrees] angular position on plane of the ecliptic relative to J2000

**Longitude of perihelion**
: [units: degrees] the point in orbit closest to sun relative to the plane of the ecliptic. All three anomalies (calculated later on) are zero when the planet is at perihelion.

**Argument of the periapsis**
: The longitude of the ascending node subtracted from the longitude of perihelion

**Mean anomaly**
: [units: degrees] the fraction of the period that has elapsed since the planet passed through the perihelion relative to J2000

**Period**
: [units: days] the time it takes for the planet to travel around the sun

## Visualization in 2D ##
![Visualization in 2D](https://cdn.britannica.com/25/5725-050-E66883FF/Anomaly-A-aphelion-B-perihelion-anomaly-orbit-C-centre-E-eccentric.jpg)

V = trueAnomaly, E = eccentricAnomaly, B = perihelion, P = planet, and the distance between A and B represents the major axis. The orbital inclination exists in the dimension not shown. NOTE: These variable letters reference the image and do not align with the rest of the program.

## Visualization in 3D ##
![Visualization in 3D](https://upload.wikimedia.org/wikipedia/commons/thumb/e/eb/Orbit1.svg/640px-Orbit1.svg.png)

The plane of reference is the orbital plane of Earth around the sun. The reference direction is referred to as "the first point of Aries". Like an epoch, it is an arbitrary reference point. It was defined over 2000 years ago by Hipparchus, considered the founder of trigonometry.
