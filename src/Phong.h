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

#include "Material.h"
#include "Color.h"

class Phong : public Material {
private:
	Color color;
	double exp;
public:
	Phong(const Color& col, double x=10.0): color(col), exp(x) { }
	
	virtual double pdf(const vec& incoming,
					   const vec& direction,
					   const vec& normal);
	
	virtual vec sample(const vec& incoming,
					   const vec& normal);
	
	virtual double sampledPdf(const vec& incoming,
							  vec direction,
							  vec normal);
	
	virtual Color getColor() { return color; }
	
	virtual Color combineColor(const Color &old) { return color * old; }
};

