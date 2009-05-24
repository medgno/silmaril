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

#include <vector>

#include "dlas.h"

#include "Container.h"
#include "HitInfo.h"

using namespace dlas;

class GridContainer : public Container {
private:
	std::vector<Thing *> things;
	
	std::vector<Thing *> unbounded;
	std::vector<Thing *>* grid;
	
	int gridSize;
	BoundingBox extents;
	vec cellSize;
	
	std::vector<Thing *>* cellAt(int x, int y, int z);
public:
	GridContainer() {
		grid = 0;
	}
	
	virtual void add(Thing *t);
	
	virtual void buildGrid(double gridPerThing=1.0);
	
	virtual void hit(HitInfo *info, /*const*/ vec /*&*/origin, /*const*/ vec /*&*/direction);
	
	virtual BoundingBox bounds() { return extents; }
	
	void setMaterial(Material *mat);
	
	virtual vec randomSurfacePoint(vec &newNormal);
};
