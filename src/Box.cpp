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
#include "Box.h"
#include "constants.h"
#include "Random.h"

Box::Box(const vec& v1, const vec& v2) {
	for (int x=0; x<3; x++) {
		if (v1[x] < v2[x]) {
			minExtent[x] = v1[x];
			maxExtent[x] = v2[x];
		} else {
			minExtent[x] = v2[x];
			maxExtent[x] = v1[x];
		}
	}
}

void Box::hit(HitInfo *info, vec origin, vec direction) {
	double tx_min, tx_max;
	double ty_min, ty_max;
	double tz_min, tz_max;
	
	double a = 1/direction.d[0];
	if (a >= 0) {
		tx_min = (minExtent.d[0] - origin.d[0]) * a;
		tx_max = (maxExtent.d[0] - origin.d[0]) * a;
	} else {
		tx_min = (maxExtent.d[0] - origin.d[0]) * a;
		tx_max = (minExtent.d[0] - origin.d[0]) * a;
	}
	
	double b = 1/direction.d[1];
	if (b >= 0) {
		ty_min = (minExtent.d[1] - origin.d[1]) * b;
		ty_max = (maxExtent.d[1] - origin.d[1]) * b;
	} else {
		ty_min = (maxExtent.d[1] - origin.d[1]) * b;
		ty_max = (minExtent.d[1] - origin.d[1]) * b;
	}
	
	double c = 1/direction.d[2];
	if (c >= 0) {
		tz_min = (minExtent.d[2] - origin.d[2]) * c;
		tz_max = (maxExtent.d[2] - origin.d[2]) * c;
	} else {
		tz_min = (maxExtent.d[2] - origin.d[2]) * c;
		tz_max = (minExtent.d[2] - origin.d[2]) * c;
	}
	
	double tmin, tmax;
	if (tx_min > ty_min)
		tmin = tx_min;
	else
		tmin = ty_min;
	if (tz_min > tmin)
		tmin = tz_min;
	
	if (tx_max < ty_max)
		tmax = tx_max;
	else
		tmax = ty_max;
	if (tz_max < tmax)
		tmax = tz_max;
	
	double t;
	if (tmin > tmax || tmax < EPSILON)
		return;
	if (tmin > EPSILON)
		t = tmin;
	else
		t = tmax;
	if ((t < info->distance || !info->hit) && t > EPSILON) {
		
		info->hit = true;
		info->distance = t;
		if (info->distanceOnly)
			return;
		vec location = origin + direction * t;
		
		info->location = location;
		vec temp;
		if (t == tx_min || t == tx_max) {
			temp = vec(1,0,0,0);
			if (direction.dot(temp) < 0)
				info->normal = temp;
			else
				info->normal = temp * -1;
		}
		else if (t == ty_min || t == ty_max) {
			temp = vec(0,1,0,0);
			if (direction.dot(temp) < 0)
				info->normal = temp;
			else
				info->normal = temp * -1;
		}
		else if (t == tz_min || t == tz_max) {
			temp = vec(0,0,1,0);
			if (direction.dot(temp) < 0)
				info->normal = temp;
			else
				info->normal = temp * -1;
		}
		info->incoming = direction;
		info->mat = material;
	}
}

vec Box::randomSurfacePoint(vec &newNormal) {
	int face = rand() % 6;
	vec diff = maxExtent - minExtent;
	if (face == 0) {
		//top
		newNormal = vec(0,0,1,0);
		return vec(minExtent[0] + diff[0] * Random::randomUnit(),
				   minExtent[1] + diff[1] * Random::randomUnit(),
				   maxExtent[2],
				   0);
	}
	else if (face == 1) {
		//bottom
		newNormal = vec(0,0,-1,0);
		return vec(minExtent[0] + diff[0] * Random::randomUnit(),
				   minExtent[1] + diff[1] * Random::randomUnit(),
				   minExtent[2],
				   0);
	}
	else if (face == 2) {
		//right
		newNormal = vec(0,1,0,0);
		return vec(minExtent[0] + diff[0] * Random::randomUnit(),
				   maxExtent[1],
				   minExtent[2] + diff[2] * Random::randomUnit(),
				   0);
	}
	else if (face == 3) {
		//left
		newNormal = vec(0,-1,0,0);
		return vec(minExtent[0] + diff[0] * Random::randomUnit(),
				   minExtent[1],
				   minExtent[2] + diff[2] * Random::randomUnit(),
				   0);
	}
	else if (face == 4) {
		//front
		newNormal = vec(1,0,0,0);
		return vec(maxExtent[0],
				   minExtent[1] + diff[1] * Random::randomUnit(),
				   minExtent[2] + diff[2] * Random::randomUnit(),
				   0);
	}
	else {
		//back
		newNormal = vec(-1,0,0,0);
		return vec(minExtent[0],
				   minExtent[1] + diff[1] * Random::randomUnit(),
				   minExtent[2] + diff[2] * Random::randomUnit(),
				   0);
	}
}
