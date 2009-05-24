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
#include "Triangle.h"
#include "constants.h"
#include "Random.h"

Triangle::Triangle(const vec& vec1, const vec& vec2, const vec& vec3) {
	v1 = vec1;
	v2 = vec2;
	v3 = vec3;
	
	vec temp1 = v1 - v2;
	vec temp2 = v1 - v3;
	
	normalv1 = temp1.cross(temp2);
	normalv1.normalize();
	
	normalv2 = normalv1;
	normalv3 = normalv1;
	material = 0;
}

Triangle::Triangle(const vec& vec1, const vec& vec2, const vec& vec3,
				   const vec& vecn1, const vec& vecn2, const vec& vecn3) {
	v1 = vec1;
	v2 = vec2;
	v3 = vec3;
	
	normalv1 = vecn1;
	normalv2 = vecn2;
	normalv3 = vecn3;
}

void Triangle::setMaterial(Material *mat) {
	material = mat;
}

BoundingBox Triangle::bounds() {
	BoundingBox bb;
	bb.minExtent = v1;
	bb.maxExtent = v1;
	for (int x=0; x<3; x++) {
		if (bb.minExtent.d[x] > v2.d[x])
			bb.minExtent.d[x] = v2.d[x];
		if (bb.minExtent.d[x] > v3.d[x])
			bb.minExtent.d[x] = v3.d[x];
		
		if (bb.maxExtent.d[x] < v2.d[x])
			bb.maxExtent.d[x] = v2.d[x];
		if (bb.maxExtent.d[x] < v3.d[x])
			bb.maxExtent.d[x] = v3.d[x];
	}
	return bb;
}

void Triangle::hit(HitInfo *info, /*const*/ vec /*&*/origin, /*const*/ vec /*&*/direction) {
	double a = v1.d[0] - v2.d[0], b = v1.d[0] - v3.d[0], c = direction.d[0], d = v1.d[0] - origin.d[0];
	double e = v1.d[1] - v2.d[1], f = v1.d[1] - v3.d[1], g = direction.d[1], h = v1.d[1] - origin.d[1];
	double i = v1.d[2] - v2.d[2], j = v1.d[2] - v3.d[2], k = direction.d[2], l = v1.d[2] - origin.d[2];
	
	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;
	
	double invDenom = 1.0 / (a * m + b * q + c * s);
	
	double e1 = d * m - b * n - c * p;
	double beta = e1 * invDenom;
	
	if (beta < 0.0)
		return;
	
	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * invDenom;
	
	if (gamma < 0.0)
		return;
	
	if (beta + gamma > 1.0)
		return;
	
	double e3 = a * p - b * r + d * s;
	double t = e3 * invDenom;
	
	if ((t < info->distance || !info->hit ) && t > EPSILON) {
		info->hit = true;
		info->distance = t;
		vec location = origin + direction * t;
		
		info->location = location;

		info->normal = normalv1 * (1 - beta - gamma) + normalv2 * beta + normalv3 * gamma;
		info->normal.normalize();
		info->incoming = direction;
		info->mat = material;
	}
}

vec Triangle::randomSurfacePoint(vec &newNormal) {
	double b = Random::randomUnit();
	double g = Random::randomUnit();
	double e = (1 - b - g);
	
	newNormal = normalv1 * b + normalv2 * g + normalv3 * e;
	newNormal.normalize();
	return v1 * b + v2 * g + v3 * e;
}

