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
#include "dlas.h"
#include "Thing.h"
#include "constants.h"

using namespace dlas;

class Box: public Thing {
private:
	vec minExtent;
	vec maxExtent;
public:
	Material *material;
public:
	Box(const vec& v1, const vec& v2);
	
	virtual void setMaterial(Material *mat) { material = mat; }
	
	virtual BoundingBox bounds() {
		BoundingBox b;
		vec e(EPSILON, EPSILON, EPSILON, 0);
		b.minExtent = minExtent - e;
		b.maxExtent = maxExtent + e;
		return b;
	}
	
	virtual void hit(HitInfo *info, /*const*/ vec /*&*/origin, /*const*/ vec /*&*/direction);
	
	virtual vec randomSurfacePoint(vec &newNormal);
	
	virtual Material* getMaterial() { return material; }
};
