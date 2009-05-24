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
#include "Lambert.h"
#include "constants.h"
#include "Random.h"


double Lambert::pdf(const vec& incoming, const vec& direction, const vec& normal) {
	vec newDirection = direction;
	vec newNormal = normal;
	newDirection.normalize();
	newNormal.normalize();
	double ans = newNormal.dot(newDirection);
	if (ans < 0)
		return 0;
	else
		return ans * BOUNCE_MULT;
}

vec Lambert::sample(const vec& incoming, const vec& normal) {
	//No meaning to the following vector. Only purpose is that we usually won't be parallel
	// to it.
	vec u = normal.cross(vec(0.973483, 0.228749, 0.00225264,0));
	u.normalize();
	vec v = normal.cross(u);
	v.normalize();
	vec r = Random::randomCosVec();
	return u * r[0] + v * r[1] + normal * r[2];
}

double Lambert::sampledPdf(const vec& incoming, vec direction, vec normal) {
	return 1 * BOUNCE_MULT;
}
