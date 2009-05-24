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
#include "DumbContainer.h"
#include "Random.h"

void DumbContainer::add(Thing *t) {
	things.push_back(t);
}

void DumbContainer::hit(HitInfo *info, vec origin, vec direction) {
	for (int x=0; x<things.size(); x++) {
		things[x]->hit(info, origin, direction);
	}
}

BoundingBox DumbContainer::bounds() {
	BoundingBox b;
	b.minExtent = vec(0,0,0,0);
	b.maxExtent = b.minExtent;
	for (int i=0; i<things.size(); i++) {
		b.combine(things[i]->bounds());
	}
	return b;
}
	
vec DumbContainer::randomSurfacePoint(vec &newNormal) {
	
	//FIXME: This is bad. How to fix it?
	if (things.size() == 0)
		return vec(0,0,0,0);
	
	int index = Random::upto(things.size());
	return things[index]->randomSurfacePoint(newNormal);
}
