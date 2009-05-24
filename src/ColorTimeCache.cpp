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
#include "ColorTimeCache.h"

int ColorTimeCache::numFrames;
double ColorTimeCache::lightspeed;

int ColorTimeCache::offset(double time) {
	return (time) / lightspeed / numFrames * NUM_TIME_SAMPLES;
}

bool ColorTimeCache::isValidTime(double time) {
	return time < lightspeed * numFrames;
}

double ColorTimeCache::timeFrom(int offset) {
	return lightspeed * numFrames * offset / NUM_TIME_SAMPLES;
}

ColorTimeCache::ColorTimeCache(){
}

void ColorTimeCache::addSample(double t, Color col) {
	if (isValidTime(t)) {
		samples[offset(t)].color += col;
	}
}

void ColorTimeCache::operator*=(double a) {
	for (int x=0; x<NUM_TIME_SAMPLES; x++) {
		samples[x].color *= a;
	}
}
void ColorTimeCache::operator*=(Color c) {
	for (int x=0; x<NUM_TIME_SAMPLES; x++) {
		samples[x].color *= c;
	}
}

void ColorTimeCache::addWeighted(const double &timeOffset, const double &a, ColorTimeCache ctc) {
	int step = offset(timeOffset);
	ColorTimeSample temp;
	for (int x=0; x<NUM_TIME_SAMPLES; x++) {
		temp = ctc.samples[x];
		//addSample(temp.time, temp.color);
		if (x + step >= NUM_TIME_SAMPLES)
			continue;
		samples[x + step].color += ctc.samples[x].color * a;
	}
}

void ColorTimeCache::addWeighted(double timeOffset, Color c, ColorTimeCache ctc) {
	int step = offset(timeOffset);
	ColorTimeSample temp;
	for (int x=0; x<NUM_TIME_SAMPLES; x++) {
		temp = ctc.samples[x];
		//addSample(temp.time, temp.color);
		if (x + step >= NUM_TIME_SAMPLES)
			continue;
		samples[x + step].color += ctc.samples[x].color * c;
	}
}

Color ColorTimeCache::avg() {
	
	Color sum;
	for (int x=0; x<NUM_TIME_SAMPLES; x++) {
		sum += samples[x].color;
	}
	return sum;
}

Color ColorTimeCache::filter(double start, double length) {
	int startindex = offset(start);
	int endindex = offset(start + length);
	if (endindex == startindex)
		endindex++;
	Color sum;
	for (int x=startindex; x<NUM_TIME_SAMPLES && x<endindex; x++) {
		sum += samples[x].color;
	}
	return sum * (1.0 / (endindex - startindex));
}

ColorTimeCache ColorTimeCache::simplify() {
	return *this;
}
