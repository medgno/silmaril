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
#include <string>

#include "dlas.h"

#include "Thing.h"
#include "GridContainer.h"
#include "Material.h"

using namespace dlas;

class TriangleMesh : public Thing {
private:
	GridContainer mesh;
	Material *material;
public:
	TriangleMesh(std::string filename, const vec& translate, const vec& rotate, double scale=1.0);
	
	virtual void hit(HitInfo *info, vec origin, vec direction);
	
	virtual BoundingBox bounds();
	
	virtual void setMaterial(Material *mat);
	
	virtual vec randomSurfacePoint(vec &newNormal);
	
	virtual Material* getMaterial() { return material; }
};
