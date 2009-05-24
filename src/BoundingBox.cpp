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
#include "BoundingBox.h"
#include "constants.h"

bool BoundingBox::contains(vec coords) const {
	for (int i=0; i<3; i++) {
		if (minExtent[i] < coords[i] || maxExtent[i] > coords[i])
			return false;
	}
	return true;
}

/*
 * Makes a new bounding box containing all the points in both this
 * and the other bounding box
 */
void BoundingBox::combine(const BoundingBox &other) {
	if (!other.isValid())
		return;
	
	if (!isValid()) {
		minExtent = other.minExtent;
		maxExtent = other.maxExtent;
		return;
	}
	
	for (int i=0; i<3; i++) {
		if (other.minExtent[i] < minExtent[i])
			minExtent[i] = other.minExtent[i];
		
		if (other.maxExtent[i] > maxExtent[i])
			maxExtent[i] = other.maxExtent[i];
	}
}

//I don't believe this code is ever used.
double BoundingBox::hit(const vec& origin, const vec& direction) {
	double tx_min, tx_max;
	double ty_min, ty_max;
	double tz_min, tz_max;
	
	double a = 1/direction[0];
	if (a >= 0) {
		tx_min = (minExtent[0] - origin[0]) * a;
		tx_max = (maxExtent[0] - origin[0]) * a;
	} else {
		tx_min = (maxExtent[0] - origin[0]) * a;
		tx_max = (minExtent[0] - origin[0]) * a;
	}
	
	double b = 1/direction[1];
	if (b >= 0) {
		ty_min = (minExtent[1] - origin[1]) * b;
		ty_max = (maxExtent[1] - origin[1]) * b;
	} else {
		ty_min = (maxExtent[1] - origin[1]) * b;
		ty_max = (minExtent[1] - origin[1]) * b;
	}
	
	double c = 1/direction[2];
	if (c >= 0) {
		tz_min = (minExtent[2] - origin[2]) * c;
		tz_max = (maxExtent[2] - origin[2]) * c;
	} else {
		tz_min = (maxExtent[2] - origin[2]) * c;
		tz_max = (minExtent[2] - origin[2]) * c;
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
	
	if (tmin < tmax && tmax > EPSILON) {
		//hit!
		if (tmin > EPSILON)
			return tmin;
		else
			return tmax;
	} else
		return -1;
}
