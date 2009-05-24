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
#include "dlas.h"

namespace dlas {
	bool matrix::operator==(const matrix &m) const {
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++)
				if (d[r][c] != m.d[r][c])
					return false;
		return true;
	}
	
	bool matrix::operator!=(const matrix &m) const {
		return !(m == *this);
	}
	
	matrix matrix::identity() {
		matrix m;
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++) {
				if (c == r)
					m.d[r][c] = 1;
				else
					m.d[r][c] = 0;
			}
		return m;
	}
	
	matrix matrix::operator*(const matrix &m) const {
		matrix temp;
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++) {
				temp.d[r][c] = d[r][0]*m.d[0][c]
					+ d[r][1]*m.d[1][c]
					+ d[r][2]*m.d[2][c]
					+ d[r][3]*m.d[3][c];
			}
		return temp;
	}
	
	matrix matrix::operator*(double n) const {
		matrix temp;
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++)
				temp.d[r][c] = d[r][c] * n;
		return temp;
	}
	
	matrix& matrix::operator*=(double n) {
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++)
				d[r][c] *= n;
		return *this;
	}
	
	vec matrix::operator*(/*const*/ vec /*&*/v) const {
		vec temp;
		for (int i=0; i<4; i++) {
			temp[i] = d[i][0] * v[0]
				+ d[i][1] * v[1]
				+ d[i][2] * v[2]
				+ d[i][3] * v[3];
		}
		return temp;
	}
	
	matrix matrix::transpose() const {
		matrix temp;
		for (int r=0; r<4; r++)
			for (int c=0; c<4; c++)
				temp.d[r][c] = d[c][r];
		return temp;
	}
}
