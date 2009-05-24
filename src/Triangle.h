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

class Triangle : public Thing {
private:
	vec v1;
	vec v2;
	vec v3;
	vec normalv1;
	vec normalv2;
	vec normalv3;
	Material *material;
public:
	Triangle(const vec& vec1, const vec& vec2, const vec& vec3);
	
	Triangle(const vec& vec1, const vec& vec2, const vec& vec3,
			 const vec& vecn1, const vec& vecn2, const vec& vecn3);
	
	virtual void setMaterial(Material *mat);
	
	virtual BoundingBox bounds();
	
	virtual void hit(HitInfo *info, vec origin, vec direction);
	
	virtual vec randomSurfacePoint(vec &newNormal);
	
	virtual Material* getMaterial() { return material; }
};