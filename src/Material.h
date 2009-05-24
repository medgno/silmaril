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

class Material;

#include "dlas.h"
#include "World.h"
#include "Color.h"

using namespace dlas;

class Material {
public:
	
	virtual double pdf(const vec& incoming,
					   const vec& direction,
					   const vec& normal) = 0;
	
	virtual vec sample(const vec& incoming,
					   const vec& normal) = 0;
	
	virtual double sampledPdf(const vec& incoming,
							  vec direction,
							  vec normal) = 0;
	
	virtual Color getColor() { return Color(0,0,0); }
	
	virtual Color combineColor(const Color &old) { return Color(0,0,0); }
};

