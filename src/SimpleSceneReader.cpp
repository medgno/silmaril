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
#include "SimpleSceneReader.h"

#include "InputHandler.h"
#include "constants.h"

SimpleSceneReader::SimpleSceneReader(std::string filename) {
	singleFrame = false;
	Camera *cam = 0;
	Thing *things = 0;
	int cacheSize=100000;
	double sampleRadius = .3;
	Color bg;
	int depth = 2;
	int samples = 1;
	world = 0;
	double lightspeed = 1;
	int numFrames = 10;
	InputHandler ih(filename);
	if (ih.fail()) {
		std::cerr << "Error opening " << filename << std::endl;
		return;
	}
	std::string fileVersion = ih.readToken();
	ifnot(fileVersion == "SceneVersion3", ih, "Unexpected token. Expected SceneVersion3");
	while (!ih.eof()) {
		if (ih.expect("Camera")) {
			ifnot((!cam), ih, "Redefinition of Camera");
			cam = readCamera(ih);
		}
		if (ih.expect("Things")) {
			ifnot((!things), ih, "Redefinition of Things");
			things = readThing(ih);
		}
		if (ih.expect("Depth")) {
			depth = ih.readInt();
		}
		if (ih.expect("Samples")) {
			samples = ih.readInt();
		}
		if (ih.expect("Background")) {
			bg = readColor(ih);
		}
		if (ih.expect("Lights")) {
			emitters = readLights(ih);
		}
		if (ih.expect("SingleFrame")) {
			singleFrame = true;
		}
		if (ih.expect("CacheSize")) {
			cacheSize = ih.readInt();
		}
		if (ih.expect("SampleRadius")) {
			sampleRadius = ih.readDouble();
		}
		if (ih.expect("Lightspeed")) {
			lightspeed = ih.readDouble();
		}
		if (ih.expect("Frames")) {
			numFrames = ih.readInt();
		}
	}
	std::cerr << "Done with input" << std::endl;
	world = new World(things, bg, cam, depth, samples);
	world->emitters = emitters;
	world->cacheSize = cacheSize;
	world->sampleRadius = sampleRadius;
	world->numFrames = numFrames;
	world->lightspeed = lightspeed;
	ColorTimeCache::numFrames = numFrames;
	ColorTimeCache::lightspeed = lightspeed;
}

std::vector<Thing*> SimpleSceneReader::readLights(InputHandler &ih) {
	std::vector<Thing *> lights;
	ifnot(ih.expect("{"), ih, "Expected '{'");
	while (!ih.expect("}")) {
		lights.push_back(readThing(ih));
	}
	return lights;
}

Lambert* SimpleSceneReader::readLambert(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	Color c;
	while (!ih.expect("}")) {
		if (ih.expect("Color")) {
			c = readColor(ih);
		}
	}
	return new Lambert(c);
}

Phong* SimpleSceneReader::readPhong(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	Color c;
	double exponent = 10;
	while (!ih.expect("}")) {
		if (ih.expect("Color")) {
			c = readColor(ih);
		}
		if (ih.expect("Exponent")) {
			exponent = ih.readDouble();
		}
	}
	return new Phong(c, exponent);
}

Emitter* SimpleSceneReader::readEmitter(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	Color c;
	while (!ih.expect("}")) {
		if (ih.expect("Color")) {
			c = readColor(ih);
		}
	}
	return new Emitter(c);
}

Material* SimpleSceneReader::readMaterial(InputHandler &ih) {
	if (ih.expect("Lambert"))
		return readLambert(ih);
	if (ih.expect("Phong"))
		return readPhong(ih);
	if (ih.expect("Emitter"))
		return readEmitter(ih);
	return 0;
}

PointCamera* SimpleSceneReader::readPointCamera(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	vec eye(5,5,0,0);
	vec look(0,0,0,0);
	vec up(0,0,1,0);
	double dist = 3;
	while (!ih.expect("}")) {
		if (ih.expect("Eye"))
			eye = readVector(ih);
		if (ih.expect("Look"))
			look = readVector(ih);
		if (ih.expect("Up"))
			up = readVector(ih);
		if (ih.expect("Distance"))
			dist = ih.readDouble();
	}
	return new PointCamera(eye, look, up, dist);
}

GLUTCamera* SimpleSceneReader::readGlutCamera(InputHandler &ih) {
	if (ih.expect("{"))
		ih.expect("}");
	return new GLUTCamera;
}

Camera* SimpleSceneReader::readCamera(InputHandler &ih) {
	if (ih.expect("PointCamera"))
		return readPointCamera(ih);
	if (ih.expect("GLUTCamera"))
		return readGlutCamera(ih);
	return 0;
}

Sphere* SimpleSceneReader::readSphere(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	vec center;
	double radius = 1.0;
	Material *mat = 0;
	while (!ih.expect("}")) {
		if (ih.expect("Radius"))
			radius = ih.readDouble();
		if (ih.expect("Center"))
			center = readVector(ih);
		if (ih.expect("Material")) {
			ifnot(mat, ih, "Redefinition of Material");
			mat = readMaterial(ih);
		}
	}
	Sphere *s = new Sphere(center, radius);
	s->setMaterial(mat);
	return s;
}

DumbContainer* SimpleSceneReader::readDumbContainer(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	DumbContainer *dc = new DumbContainer;
	while (!ih.expect("}")) {
		Thing *t = readThing(ih);
		ifnot(t, ih, "Invalid Thing");
		dc->add(t);
	}
	return dc;
}

Plane* SimpleSceneReader::readPlane(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	vec normal;
	double distance;
	Material* mat = 0;
	while (!ih.expect("}")) {
		if (ih.expect("Normal"))
			normal = readVector(ih);
		if (ih.expect("Distance"))
			distance = ih.readDouble();
		if (ih.expect("Material")) {
			ifnot(mat, ih, "Redefinition of Material");
			mat = readMaterial(ih);
		}
	}
	Plane *p = new Plane(normal, distance);
	p->setMaterial(mat);
	return p;
}

GridContainer* SimpleSceneReader::readGridContainer(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	double resolution=1;
	GridContainer *gc = new GridContainer;
	while (!ih.expect("}")) {
		if (ih.expect("Resolution"))
			resolution = ih.readDouble();
		if (ih.expect("AddLights"))
			for (int x=0; x<emitters.size(); x++)
				gc->add(emitters[x]);
		Thing *t = readThing(ih);
		ifnot(t, ih, "Invalid Thing");
		gc->add(t);
	}
	gc->buildGrid(resolution);
	return gc;
}

Triangle* SimpleSceneReader::readTriangle(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	vec v1, v2, v3;
	Material* mat = 0;
	while (!ih.expect("}")) {
		if (ih.expect("Vertex1"))
			v1 = readVector(ih);
		if (ih.expect("Vertex2"))
			v2 = readVector(ih);
		if (ih.expect("Vertex3"))
			v3 = readVector(ih);
		if (ih.expect("Material")) {
			ifnot(mat, ih, "Redefinition of Material");
			mat = readMaterial(ih);
		}
	}
	Triangle *t = new Triangle(v1, v2, v3);
	t->setMaterial(mat);
	return t;
}

TriangleMesh* SimpleSceneReader::readTriangleMesh(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	Material *mat = 0;
	std::string filename;
	double scale = 1.0;
	vec trans,rotate;
	while (!ih.expect("}")) {
		if (ih.expect("Filename"))
			filename = ih.readString();
		if (ih.expect("Material")) {
			ifnot(mat, ih, "Redefinition of Material");
			mat = readMaterial(ih);
		}
		if (ih.expect("Scale")) {
			scale = ih.readDouble();
		}
		if (ih.expect("Translate")) {
			trans = readVector(ih);
		}
		if (ih.expect("Rotate")) {
			rotate = readVector(ih);
			rotate *= (PI/180);
		}
	}
	TriangleMesh *tm = new TriangleMesh(filename, trans, rotate, scale);
	tm->setMaterial(mat);
	return tm;
}

Box* SimpleSceneReader::readBox(InputHandler &ih) {
	ifnot(ih.expect("{"), ih, "Expected '{'");
	Material *mat = 0;
	vec min, max;
	while (!ih.expect("}")) {
		if (ih.expect("Vertex1"))
			min = readVector(ih);
		if (ih.expect("Vertex2"))
			max = readVector(ih);
		if (ih.expect("Material"))
			mat = readMaterial(ih);
	}
	Box* b = new Box(min, max);
	b->setMaterial(mat);
	return b;
}

Thing* SimpleSceneReader::readThing(InputHandler &ih) {
	if (ih.expect("Sphere"))
		return readSphere(ih);
	if (ih.expect("DumbContainer"))
		return readDumbContainer(ih);
	if (ih.expect("Plane"))
		return readPlane(ih);
	if (ih.expect("GridContainer"))
		return readGridContainer(ih);
	if (ih.expect("TriangleMesh"))
		return readTriangleMesh(ih);
	if (ih.expect("Triangle"))
		return readTriangle(ih);
	if (ih.expect("Box"))
		return readBox(ih);
	return 0;
}

Color SimpleSceneReader::readColor(InputHandler &ih) {
	vec v = readVector(ih);
	Color c(v[0], v[1], v[2]);
	return c;
}

vec SimpleSceneReader::readVector(InputHandler &ih) {
	vec v;
	v[0] = ih.readDouble();
	ifnot(ih.expect(","), ih, "Expected ','");
	v[1] = ih.readDouble();
	ifnot(ih.expect(","), ih, "Expected ','");
	v[2] = ih.readDouble();
	if (ih.expect(",")) {
		v[3] = ih.readDouble();
	}
	return v;
}

void SimpleSceneReader::ifnot(bool result, InputHandler &ih, std::string description) {
	if (result)
		return;
	std::cerr << description << std::endl;
	ih.currentPosition(std::cerr);
}
