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
#ifndef dlas_
#define dlas_

namespace dlas {
	
	class vec;
	class matrix;
	
	class vec {
	private:
		
	public:
		double d[4];
		vec();

		vec(double a, double b, double c, double d);
		
		bool operator==(vec v) const;
		bool operator!=(vec v) const;
		
		inline double& operator[](int index) {
			return d[index];
		}
		inline double operator[](int index) const {
			return d[index];
		}
		
		vec operator+(vec v) const;
		vec& operator+=(vec v);
		
		vec operator*(double n) const;
		vec& operator*=(double n);
		
		vec operator-(vec v) const;
		
		vec operator/(vec v) const;
		
		vec operator/(double n) const;
		
		double dot(const vec& v) const;
		
		vec cross(vec v) const;
		
		double magnitudeSq() const;
		
		double magnitude() const;
		
		void normalize();
		
		void fixW();
		
	};
	
	class matrix {
	private:
		double d[4][4];
	public:
		bool operator==(const matrix &m) const;
		bool operator!=(const matrix &m) const;
		static matrix identity();
		
		inline double& operator()(int row, int col) {
			return d[row][col];
		}
		
		inline double operator()(int row, int col) const {
			return d[row][col];
		}
		
		matrix operator*(const matrix &m) const;
		
		matrix operator*(double n) const;
		matrix& operator*=(double n);
		
		vec operator*(/*const*/ vec /*&*/v) const;
		
		matrix transpose() const;
		
	};
}

#endif
