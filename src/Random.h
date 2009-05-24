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
#pragma once

#include "dlas.h"

using namespace dlas;

const int NUM_SAMPLES = 86467;
// prime, to try to avoid obvious cycles

class Random {
public:
	
	static vec* samples;
	static vec* cosSamples;
	static double* units;
	static void init();
	static vec randomVec();
	static vec randomCosVec(double e=1.0);
	static double randomUnit();
	static double randomCenteredUnit();
	static double upto(int num);
};
