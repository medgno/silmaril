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
#include <cmath>

#include "dlas.h"

namespace dlas {

	vec::vec() { }
	
	/*vec::vec(const vec &v) {
		for (int i=0; i<4; i++)
			d[i] = v.d[i];
	}*/
	
	vec::vec(double i, double j, double k, double l) {
		d[0] = i;
		d[1] = j;
		d[2] = k;
		d[3] = l;
	}
	
	bool vec::operator==(vec v) const {
		for (int i=0; i<4; i++)
			if (d[i] != v.d[i])
				return false;
		return true;
	}
	
	bool vec::operator!=(vec v) const {
		return !( *this == v);
	}
	
	vec vec::operator+(vec v) const {
		vec temp;
		for (int i=0; i<3; i++)
			temp.d[i] = d[i] + v.d[i];
		temp[3] = d[3];
		return temp;
	}
	
	vec& vec::operator+=(vec v) {
		for (int i=0; i<3; i++)
			d[i] += v.d[i];
		return *this;
	}
			

	vec vec::operator*(double n) const {
		vec temp;
		for (int i=0; i<3; i++)
			temp.d[i] = d[i] * n;
		temp[3] = d[3];
		return temp;
	}
	
	vec& vec::operator*=(double n) {
		for (int i=0; i<3; i++)
			d[i] *= n;
		return *this;
	}
	
	vec vec::operator-(vec v) const {
		vec temp;
		for (int i=0; i<3; i++)
			temp.d[i] = d[i] - v.d[i];
		temp[3] = 0;
		return temp;
	}
	
	vec vec::operator/(vec v) const {
		vec temp;
		for (int i=0; i<3; i++)
			temp.d[i] = d[i] / v.d[i];
		temp[3] = 0;
		return temp;
	}
	
	vec vec::operator/(double n) const {
		vec temp;
		for (int i=0; i<3; i++)
			temp.d[i] = d[i] / n;
		temp[3] = 0;
		return temp;
	}

	double vec::dot(const vec& v) const {
		return d[0]*v[0] + d[1]*v[1] + d[2]*v[2];
	}

	vec vec::cross(vec v) const {
		vec temp;
		temp.d[0] = d[1]*v.d[2] - d[2]*v.d[1];
		temp.d[1] = d[2]*v.d[0] - d[0]*v.d[2];
		temp.d[2] = d[0]*v.d[1] - d[1]*v.d[0];
		temp.d[3] = 0;
		return temp;
	}
	
	double vec::magnitudeSq() const {
		return d[0]*d[0] + d[1]*d[1] + d[2]*d[2];
	}
	
	double vec::magnitude() const {
		return std::sqrt(magnitudeSq());
	}

	void vec::normalize() {
		double m = magnitude();
		d[0] /= m;
		d[1] /= m;
		d[2] /= m;
	}

	void vec::fixW() { 
		if (d[3] != 0 && d[3] != 1) {
			d[0] /= d[3];
			d[1] /= d[3];
			d[2] /= d[3];
			d[3] = 1;
		}
	}

}
