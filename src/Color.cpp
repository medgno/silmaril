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
#include "Color.h"

Color::Color() {
	r=0;
	g=0;
	b=0;
}

Color::Color(int ri, int gi, int bi) {
	r = ((double)ri / 255.0);
	g = ((double)gi / 255.0);
	b = ((double)bi / 255.0);
}

Color::Color(float* data) {
	r = (double)data[0];
	g = (double)data[1];
	b = (double)data[2];
}

Color::Color(double ri, double gi, double bi) {
	r = ri;
	g = gi;
	b = bi;
}

double Color::intensity() const {
	return r+g+b;
}


Color Color::operator+(const Color &other) const {
	Color temp(r + other.r, g + other.g, b + other.b);
	return temp;
}

Color Color::operator+=(const Color &other) {
	r += other.r;
	g += other.g;
	b += other.b;
	
	return *this;
}


Color Color::operator*(const Color &other) const {
	Color temp(r * other.r, g * other.g, b * other.b);
	return temp;
}

Color& Color::operator*=(const Color &other) {
	r *= other.r;
	g *= other.g;
	b *= other.b;
	return *this;
}


Color Color::operator*(double n) const {
	Color temp(r * n, g * n, b * n);
	return temp;
}

Color& Color::operator*=(double n) {
	r *= n;
	g *= n;
	b *= n;
	
	return *this;
}


void Color::write(float *data) const {
	data[0] = (float)r;
	data[1] = (float)g;
	data[2] = (float)b;
}

int Color::intR() const {
	if (r > 1)
		return 255;
	else
		return (int)(r * 255);
}

int Color::intG() const {
	if (g > 1)
		return 255;
	else
		return (int)(g * 255);
}

int Color::intB() const {
	if (b > 1)
		return 255;
	else
		return (int)(b * 255);
}

