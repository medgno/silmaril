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
#include <iostream>

#include "Sphere.h"
#include "constants.h"
#include "Random.h"

Sphere::Sphere(): center(0,0,0,0), radius(1), material(0) { }

Sphere::Sphere(vec c, double r): center(c), radius(r), material(0) { }

void Sphere::setMaterial(Material *mat) {
	material = mat;
}

void Sphere::hit(HitInfo *info, vec origin, vec direction) {
	double newDistance;
	vec originMinusCenter = origin - center;
	double b = 2 * originMinusCenter.dot(direction);
	double c = originMinusCenter.magnitudeSq() - radius * radius;
	double discrim = b * b - 4 * c;
	
	//discrim < 0 means no hit.
	if (discrim < 0)
		return;
	
	
	double sqrtDiscrim = std::sqrt(discrim);
	
	newDistance = (-b - sqrtDiscrim) / 2;
	if (newDistance < 0)
		newDistance = (-b + sqrtDiscrim) / 2;
	
	if ((newDistance < info->distance || !info->hit) && newDistance > EPSILON) {
		info->hit = true;
		info->distance = newDistance;
		if (info->distanceOnly)
			return;
		
		vec location = origin + direction * newDistance;
		vec normal = location - center;
		normal.normalize();
		
		info->location = location;
		info->normal = normal;
		info->incoming = direction;
		info->mat = material;
		
		return;
	} 
}

BoundingBox Sphere::bounds() {
	BoundingBox b;
	vec radiusVector(radius, radius, radius, 0);
	b.minExtent = center - radiusVector;
	b.maxExtent = center + radiusVector;
	return b;
}

vec Sphere::randomSurfacePoint(vec &newNormal) {
	newNormal = Random::randomVec();
	return center + newNormal * radius;
}

