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

class HitInfo;

#include "dlas.h"

#include "World.h"
#include "Color.h"
#include "Material.h"

using namespace dlas;

class HitInfo {
public:
	double previousDistances;
	double distance;
	bool hit;
	bool distanceOnly;
	
	vec location;
	vec normal;
	vec incoming;
	Material *mat;
	
	HitInfo() {
		previousDistances = 0;
		distance = 0;
		mat = 0;
		hit = false;
		distanceOnly = false;
	}
	
	void follow(HitInfo &info) {
		distanceOnly = info.distanceOnly;
		previousDistances = info.distance + info.previousDistances;
	}
};
