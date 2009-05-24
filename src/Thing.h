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

class Thing;

#include "dlas.h"
#include "HitInfo.h"
#include "BoundingBox.h"
#include "Material.h"

using namespace dlas;

class Thing {
public:
	/* Given the specified ray origin and direction, test to see if it hit the object.
	 * Exact behavior depends on the settings inside *info.
	 */
	virtual void hit(HitInfo *info, vec origin, vec direction) = 0;
	
	/* Set the object's material. In the case of a container, set the material of all
	 * contained objects.
	 */
	virtual void setMaterial(Material *mat) { }
	
	/* Returns the material. On a container, this is silly.
	 */
	virtual Material* getMaterial() = 0;
	
	/* Returns a bounding box for the object. The bounding box returned by this
	 * implementation is the 'infinite' box
	 */
	virtual BoundingBox bounds() {
		BoundingBox b;
		b.minExtent = vec(0,0,0,0);
		b.maxExtent = b.minExtent;
		return b;
	}
	
	/* return a random point on the surface of the object, as well as the
	 * normal at that point
	 */
	virtual vec randomSurfacePoint(vec &newNormal) = 0;
};
