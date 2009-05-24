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
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

#include "TriangleMesh.h"
#include "Triangle.h"
#include "InputHandler.h"

TriangleMesh::TriangleMesh(std::string filename, const vec& translate, const vec& rotate, double scale) {
	std::ifstream infile(filename.c_str());
	if (!infile) {
		std::cerr << "Error opening file!" << std::endl;
		return;
	}
	InputHandler in(&infile, filename);
	
	matrix rotation = matrix::identity();
	matrix tempm = matrix::identity();
	
	tempm(1,1) = std::cos(rotate[0]);
	tempm(1,2) = std::sin(rotate[0]);
	tempm(2,1) = -std::sin(rotate[0]);
	tempm(2,2) = std::cos(rotate[0]);
	rotation = tempm * rotation;
	
	tempm = matrix::identity();
	tempm(0,0) = std::cos(rotate[1]);
	tempm(0,2) = -std::sin(rotate[1]);
	tempm(2,0) = std::sin(rotate[1]);
	tempm(2,2) = std::cos(rotate[1]);
	rotation = tempm * rotation;
	
	tempm = matrix::identity();
	tempm(0,0) = std::cos(rotate[2]);
	tempm(0,1) = std::sin(rotate[2]);
	tempm(1,0) = -std::sin(rotate[2]);
	tempm(1,1) = std::cos(rotate[2]);
	rotation = tempm * rotation;
	
	std::vector<vec> v;
	std::vector<vec> vn;
	std::vector<vec> vt;
	int curv = 1, curvn = 1;
	vec temp;
	
	//Following, so that indices match up (.obj starts counting at 1)
	v.push_back(temp);
	vn.push_back(temp);
	vt.push_back(temp);
	while (!in.eof()) {
		
		if (in.expect("#")) {
			// comment, ignore rest of line.
			in.readLine();
		}
		else if (in.expect("o")) {
			// object name? ignored.
			in.readLine();
		}
		else if (in.expect("usemtl")) {
			//material. ignored.
			in.readLine();
		}
		else if (in.expect("s")) {
			//shading information. ignored.
			std::cerr << in.readLine() << std::endl;
		}
		else if (in.expect("g")) {
			//group. ignored.
			std::cerr << in.readLine() << std::endl;
		}
		else if (in.expect("vn")) {
			//vertex. not ignored.
			temp[0] = in.readDouble();
			temp[1] = in.readDouble();
			temp[2] = in.readDouble();
			temp = rotation * temp;
			temp.normalize();
			vn.push_back(temp);
			curvn++;
		}
		else if (in.expect("v")) {
			// TODO: move this after vn and vt to prevent failure.
			//vertex. not ignored.
			temp[0] = in.readDouble() * scale;
			temp[1] = in.readDouble() * scale;
			temp[2] = in.readDouble() * scale;
			temp = rotation * temp + translate;
			v.push_back(temp);
			curv++;
		}
		else if (in.expect("f")) {
			int currentLine = in.getLine();
			int index = 0;
			int indices[3];
			bool normals = false;
			int normalindices[3];
			while(in.getLine() == currentLine) {
				indices[index] = in.readInt();
				if (in.expect("/")) {
					if (!in.expect("/")) {
						in.readInt();
						in.expect("/");
					}
					normalindices[index] = in.readInt();
					normals = true;
				}
				if (index < 2)
					index++;
				else {
					if (normals)
						mesh.add(new Triangle(v[indices[0]], v[indices[1]], v[indices[2]],
											  vn[normalindices[0]], vn[normalindices[1]], vn[normalindices[2]]));
					else
						mesh.add(new Triangle(v[indices[0]], v[indices[1]], v[indices[2]]));
					indices[1] = indices[2];
					normalindices[1] = normalindices[2];
				}
			}
		}
		else {
			std::cerr << "I am lost." << std::endl;
		}
	}
	mesh.buildGrid(.04);
	//FIXME: The above should really be an octree or other better structure.
}

void TriangleMesh::hit(HitInfo *info, vec origin, vec direction) {
	mesh.hit(info, origin, direction);
}

BoundingBox TriangleMesh::bounds() {
	return mesh.bounds();
}

void TriangleMesh::setMaterial(Material *mat) {
	material = mat;
	mesh.setMaterial(mat);
}

vec TriangleMesh::randomSurfacePoint(vec &newNormal) {
	return mesh.randomSurfacePoint(newNormal);
}
