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

#include "GridContainer.h"
#include "constants.h"
#include "Random.h"

std::vector<Thing *>* GridContainer::cellAt(int x, int y, int z) {
	return grid + x + y * (gridSize+1) + z * (gridSize+1) * (gridSize+1);
}

void GridContainer::add(Thing *t) {
	things.push_back(t);
}

void GridContainer::buildGrid(double gridPerThing) {
	int numThings = things.size();
	
	extents.minExtent = vec(0,0,0,0);
	extents.maxExtent = vec(0,0,0,0);
	for (int i=0; i<numThings; i++) {
		extents.combine(things[i]->bounds());
	}
	for (int x=0; x<3; x++) {
		extents.minExtent[x] -= EPSILON;
		extents.maxExtent[x] += EPSILON;
	}
	gridSize = (int)ceil(std::sqrt(gridPerThing * numThings));
	std::cerr << gridSize << std::endl;
	
	if (grid)
		delete[] grid;
	
	unbounded.clear();
	
	grid = new std::vector<Thing *>[(gridSize+1) * (gridSize+1) * (gridSize+1)];
	cellSize = (extents.maxExtent - extents.minExtent) * (1.0/gridSize);
	
	for (int i=0; i<numThings; i++) {
		BoundingBox current = things[i]->bounds();
		if (!current.isValid()) {
			unbounded.push_back(things[i]);
		} else {
			int mapMinX, mapMinY, mapMinZ;
			int mapMaxX, mapMaxY, mapMaxZ;
			vec shiftedMin = current.minExtent - extents.minExtent;
			vec shiftedMax = current.maxExtent - extents.minExtent;
			mapMinX = (int)(shiftedMin[0]/cellSize[0]);
			mapMinY = (int)(shiftedMin[1]/cellSize[1]);
			mapMinZ = (int)(shiftedMin[2]/cellSize[2]);
			
			//TODO: Make more sure this is correct
			mapMaxX = (int)(shiftedMax[0]/cellSize[0] + 1);
			mapMaxY = (int)(shiftedMax[1]/cellSize[1] + 1);
			mapMaxZ = (int)(shiftedMax[2]/cellSize[2] + 1);
			
			if (mapMaxX >= gridSize) mapMaxX = gridSize;
			if (mapMaxY >= gridSize) mapMaxY = gridSize;
			if (mapMaxZ >= gridSize) mapMaxZ = gridSize;
			
			for (int x=mapMinX; x<=mapMaxX; x++)
				for (int y=mapMinY; y<=mapMaxY; y++)
					for (int z=mapMinZ; z<mapMaxZ; z++)
						cellAt(x,y,z)->push_back(things[i]);
		}
	}
	
}

void GridContainer::hit(HitInfo *info, vec origin, vec direction) {
	if (!grid) {
		for (int x=0; x<things.size(); x++) {
			things[x]->hit(info, origin, direction);
		}
	} else {
		double tx_min, tx_max;
		double ty_min, ty_max;
		double tz_min, tz_max;
		
		double a = 1/direction[0];
		if (a >= 0) {
			tx_min = (extents.minExtent.d[0] - origin.d[0]) * a;
			tx_max = (extents.maxExtent.d[0] - origin.d[0]) * a;
		} else {
			tx_min = (extents.maxExtent.d[0] - origin.d[0]) * a;
			tx_max = (extents.minExtent.d[0] - origin.d[0]) * a;
		}
		
		double b = 1/direction[1];
		if (b >= 0) {
			ty_min = (extents.minExtent.d[1] - origin.d[1]) * b;
			ty_max = (extents.maxExtent.d[1] - origin.d[1]) * b;
		} else {
			ty_min = (extents.maxExtent.d[1] - origin.d[1]) * b;
			ty_max = (extents.minExtent.d[1] - origin.d[1]) * b;
		}
		
		double c = 1/direction[2];
		if (c >= 0) {
			tz_min = (extents.minExtent.d[2] - origin.d[2]) * c;
			tz_max = (extents.maxExtent.d[2] - origin.d[2]) * c;
		} else {
			tz_min = (extents.maxExtent.d[2] - origin.d[2]) * c;
			tz_max = (extents.minExtent.d[2] - origin.d[2]) * c;
		}
		
		double tmin, tmax;
		if (tx_min < ty_min)
			tmin = tx_min;
		else
			tmin = ty_min;
		if (tz_min < tmin)
			tmin = tz_min;
		
		if (tx_max > tx_max)
			tmax = tx_max;
		else
			tmax = ty_max;
		if (tz_max > tmax)
			tmax = tz_max;
		
		if (tmin > tmax && tmax > EPSILON)
			return;
		
		int ix, iy, iz;
		
		if (tmin > EPSILON) {
			vec newOrigin = origin + direction * (tmin);
			ix = (int)((newOrigin.d[0] - extents.minExtent.d[0]) / cellSize.d[0]);
			iy = (int)((newOrigin.d[1] - extents.minExtent.d[1]) / cellSize.d[1]);
			iz = (int)((newOrigin.d[2] - extents.minExtent.d[2]) / cellSize.d[2]);
		} else {
			ix = (int)((origin.d[0] - extents.minExtent.d[0]) / cellSize.d[0]);
			iy = (int)((origin.d[1] - extents.minExtent.d[1]) / cellSize.d[1]);
			iz = (int)((origin.d[2] - extents.minExtent.d[2]) / cellSize.d[2]);
		}
		
		if (ix < 0) ix = 0;
		if (ix >= gridSize) ix = gridSize - 1;
		if (iy < 0) iy = 0;
		if (iy >= gridSize) iy = gridSize - 1;
		if (iz < 0) iz = 0;
		if (iz >= gridSize) iz = gridSize - 1;
		
		double dtx = (tx_max - tx_min) / gridSize;
		double dty = (ty_max - ty_min) / gridSize;
		double dtz = (tz_max - tz_min) / gridSize;
		
		double tx_next, ty_next, tz_next;
		int ix_step, iy_step, iz_step;
		int ix_stop, iy_stop, iz_stop;
		
		if (direction.d[0] > 0) {
			tx_next = tx_min + (ix + 1) * dtx;
			ix_step = 1;
			ix_stop = gridSize;
		} else {
			tx_next = tx_min + (gridSize - ix) * dtx;
			ix_step = -1;
			ix_stop = -1;
		}
		if (direction.d[0] == 0) {
			tx_next = HUGE_VALUE;
			ix_step = -1;
			ix_stop = -1;
		}
		
		if (direction.d[1] > 0) {
			ty_next = ty_min + (iy + 1) * dty;
			iy_step = 1;
			iy_stop = gridSize;
		} else {
			ty_next = ty_min + (gridSize - iy) * dty;
			iy_step = -1;
			iy_stop = -1;
		}
		if (direction.d[1] == 0) {
			ty_next = HUGE_VALUE;
			iy_step = -1;
			iy_stop = -1;
		}

		if (direction.d[2] > 0) {
			tz_next = tz_min + (iz + 1) * dtz;
			iz_step = 1;
			iz_stop = gridSize;
		} else {
			tz_next = tz_min + (gridSize - iz) * dtz;
			iz_step = -1;
			iz_stop = -1;
		}
		if (direction.d[2] == 0) {
			tz_next = HUGE_VALUE;
			iz_step = -1;
			iz_stop = -1;
		}
		
		for (int i=0; i<unbounded.size(); i++) {
			unbounded[i]->hit(info, origin, direction);
		}
		
		std::vector<Thing *>* cell;
		
		while (true) {
			cell = cellAt(ix, iy, iz);
			for (int i=0; i<cell->size(); i++)
				(*cell)[i]->hit(info, origin, direction);
			if (tx_next < ty_next && tx_next < tz_next) {
				if (info->hit && info->distance < tx_next) {
					return;
				}
				
				tx_next += dtx;
				ix += ix_step;
				
				if (ix == ix_stop)
					return;
			} 
			else { 	
				if (ty_next < tz_next) {
					if (info->hit && info->distance < ty_next) {
						return ;
					}
					
					ty_next += dty;
					iy += iy_step;
					
					if (iy == iy_stop)
						return;
				}
				else {		
					if (info->hit && info->distance < tz_next) {
						return ;
					}
					
					tz_next += dtz;
					iz += iz_step;
					
					if (iz == iz_stop)
						return;
				}
			}
				
		}
	}
}

void GridContainer::setMaterial(Material *mat) {
	for (int x=0; x<things.size(); x++)
		things[x]->setMaterial(mat);
}

vec GridContainer::randomSurfacePoint(vec &newNormal) {
	
	//FIXME: This is bad. How to fix it?
	if (things.size() == 0)
		return vec(0,0,0,0);
	
	int index = Random::upto(things.size());
	return things[index]->randomSurfacePoint(newNormal);
}


