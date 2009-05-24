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

#include <vector>

#include "Color.h"

const int NUM_TIME_SAMPLES = 1000;

struct ColorTimeSample {
	ColorTimeSample() { }
	ColorTimeSample(Color c) {
		color = c;
	}
	
	Color color;
};

class ColorTimeCache {
private:
	// not a vector due to runtime performance issues
	//std::vector<ColorTimeSample> samples;
	ColorTimeSample samples[NUM_TIME_SAMPLES];
	int offset(double time);
	bool isValidTime(double time);
	double timeFrom(int offset);
public:
	ColorTimeCache();
	static int numFrames;
	static double lightspeed;
	void addSample(double t, Color col);
	
	void operator*=(double a);
	void operator*=(Color c);
	
	void addWeighted(const double &timeOffset, const double &a, ColorTimeCache ctc);
	void addWeighted(double timeOffset, Color c, ColorTimeCache ctc);
	ColorTimeCache simplify();
	Color avg();
	Color filter(double start, double length);
};
