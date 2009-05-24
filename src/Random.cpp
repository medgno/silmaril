/*
    Copyright 2008, 2009 David Schroeder

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <cstdlib>
#include <cmath>

#include "Random.h"
#include "constants.h"

vec* Random::samples = 0;
double* Random::units = 0;
vec* Random::cosSamples = 0;

void Random::init() {
	samples = new vec[NUM_SAMPLES];
	cosSamples = new vec[NUM_SAMPLES];
	units = new double[NUM_SAMPLES];
	vec temp;
	for (int x=0; x<NUM_SAMPLES; x++) {
		do {
			temp = vec((double(rand()) / RAND_MAX -.5)*2,
					  (double(rand()) / RAND_MAX -.5)*2,
					  (double(rand()) / RAND_MAX -.5)*2, 0);
		} while (temp.magnitudeSq() > 1);
		temp.normalize();
		samples[x] = temp;
		units[x] = double(rand()) / RAND_MAX;
		
		double rx = double(rand()) / RAND_MAX;
		double ry = double(rand()) / RAND_MAX;
		
		double cosp = std::cos(2 * PI * rx);
		double sinp = std::sin(2 * PI * rx);
		double e = 1.0;
		double cost = std::pow((1 - ry), 1.0/(e + 1));
		double sint = std::sqrt(1 - cost * cost);
		cosSamples[x] = vec(sint * cosp, sint * sinp, cost, 0);
	}
}

vec Random::randomVec() {
	return samples[rand() % NUM_SAMPLES];
}

double Random::randomUnit() {
	return units[rand() % NUM_SAMPLES];
}

double Random::randomCenteredUnit() {
	return units[rand() % NUM_SAMPLES] - .5;
}

vec Random::randomCosVec(double e) {
	double rx = double(rand()) / RAND_MAX;
	double ry = double(rand()) / RAND_MAX;
	
	double cosp = std::cos(2 * PI * rx);
	double sinp = std::sin(2 * PI * rx);
	double cost = std::pow((1 - ry), 1.0/(e + 1));
	double sint = std::sqrt(1 - cost * cost);
	return vec(sint * cosp, sint * sinp, cost, 0);
}

double Random::upto(int num) {
	return rand() % num;
}
