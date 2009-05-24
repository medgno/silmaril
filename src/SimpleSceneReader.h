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

#include <string>
#include <vector>

#include "dlas.h"

#include "World.h"

#include "InputHandler.h"

#include "Lambert.h"
#include "Phong.h"
#include "Emitter.h"

#include "PointCamera.h"
#include "GLUTCamera.h"

#include "Sphere.h"
#include "DumbContainer.h"
#include "Plane.h"
#include "GridContainer.h"
#include "Triangle.h"
#include "TriangleMesh.h"
#include "Box.h"

#include "Color.h"
#include "World.h"

using namespace dlas;

class SimpleSceneReader {
public:
	World *world;
	bool singleFrame;
	std::vector<Thing *> emitters;
	SimpleSceneReader(std::string filename);
private:
	std::vector<Thing*> readLights(InputHandler &ih);
	Lambert* readLambert(InputHandler &ih);
	Phong* readPhong(InputHandler &ih);
	Emitter* readEmitter(InputHandler &ih);
	Material* readMaterial(InputHandler &ih);
	PointCamera* readPointCamera(InputHandler &ih);
	GLUTCamera* readGlutCamera(InputHandler &ih);
	Camera* readCamera(InputHandler &ih);
	Sphere* readSphere(InputHandler &ih);
	DumbContainer* readDumbContainer(InputHandler &ih);
	Plane* readPlane(InputHandler &ih);
	GridContainer* readGridContainer(InputHandler &ih);
	Triangle* readTriangle(InputHandler &ih);
	TriangleMesh* readTriangleMesh(InputHandler &ih);
	Box* readBox(InputHandler &ih);
	Thing* readThing(InputHandler &ih);
	Color readColor(InputHandler &ih);
	vec readVector(InputHandler &ih);
	void ifnot(bool result, InputHandler &ih, std::string description);
};
