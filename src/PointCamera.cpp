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
#include "PointCamera.h"

PointCamera::PointCamera() {
}

PointCamera::PointCamera(vec eye, vec look, vec up, double dist) {
	resetLook(eye, look, up, dist);
}

void PointCamera::resetLook(vec eye, vec look, vec up, double dist) {
	distance = dist;
	here = eye;
	vec w = eye - look;
	w.normalize();
	
	vec u = up.cross(w);
	u.normalize();
	
	vec v = w.cross(u);
	
	mapping = matrix::identity();
	
	for (int i=0; i<3; i++) {
		mapping(i,0) = u[i];
		mapping(i,1) = v[i];
		mapping(i,2) = w[i];
	}
}

void PointCamera::map(double x, double y, vec &origin, vec &direction) {
	vec point(x, y, -distance, 1);
	origin = here;
	direction = mapping * point;
	direction.normalize();
}
