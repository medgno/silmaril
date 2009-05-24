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
#include "Thing.h"
#include "Material.h"

using namespace dlas;

class Sphere : public Thing {
private:
	double radius;
	vec center;
	Material *material;
public:
	Sphere();
	Sphere(vec c, double r = 1.0);
	
	virtual void setMaterial(Material *mat);
	
	virtual void hit(HitInfo *info, vec origin, vec direction);
	
	virtual BoundingBox bounds();
	
	virtual vec randomSurfacePoint(vec &newNormal);
	
	virtual Material* getMaterial() { return material; }
};

