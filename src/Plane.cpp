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
#include "Plane.h"
#include "constants.h"
#include "Random.h"

Plane::Plane(const vec& n, const vec& p) {
	normal = n;
	normal.normalize();
	offset = normal.dot(p);
	material = 0;
}

Plane::Plane(const vec& n, double o) {
	normal = n;
	normal.normalize();
	offset = o;
	material = 0;
}

void Plane::setMaterial(Material *mat) {
	material = mat;
}

void Plane::hit(HitInfo *info, vec origin, vec direction) {
	double v_d = direction.dot(normal);
	if (v_d > 0)
		return;
	
	double t = -(normal.dot(origin) + offset) / v_d;
	
	if ((t < info->distance || !info->hit ) && t > EPSILON) {
		info->hit = true;
		info->distance = t;
		vec location = origin + direction * t;
	
		info->location = location;
		info->normal = normal;
		info->incoming = direction;
		info->mat = material;
	}
}

vec Plane::randomSurfacePoint(vec &newNormal) {
	// Slight problem here of trying to take a random point on an infinite surface.
	//TODO: There is a fudge factor here.
	
	newNormal = normal;
	
	//Here's what I'm going to do: Set up an orthonormal coordinate system around the normal vector
	//and have a square region
	
	vec u = normal.cross(vec(0.973483, 0.228749, 0.00225264,0));
	u.normalize();
	vec v = normal.cross(u);
	v.normalize();
	return u * Random::randomCenteredUnit() + v * Random::randomCenteredUnit() + normal * offset;
}
