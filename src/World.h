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

class World;

#include <vector>

#include "dlas.h"
#include "HitInfo.h"
#include "Thing.h"
#include "Color.h"
#include "Camera.h"
#include "PhotonCache.h"
#include "ColorTimeCache.h"

using namespace dlas;

class World {
private:
	Color background;
	Camera *cam;
	Thing *objects;
	PhotonCache *pc;
	int depth;
	int samples;
	std::vector<HitInfo> hicache;
	std::vector<Color> colorcache;
	double cacheweight;
public:
	int cacheSize;
	double sampleRadius;
	double lightspeed;
	int numFrames;
	std::vector<Thing*> emitters;
	World(Thing *obj, Color bg, Camera *cam, int d = 2, int s=1);
	ColorTimeCache trace(double x, double y) const;
	void hit(HitInfo *info, vec origin, vec direction) const;
	Color runBiDi(vec origin, vec direction) const;
	ColorTimeCache runCache(vec origin, vec direction) const;
	Color runPhotonCache(vec origin, vec direction) const;
	Color runDirectPhoton(vec origin, vec direction) const;
	Color runSurfacePhoton(vec origin, vec direction) const;
	
	ColorTimeCache runTimePhotonCache(vec origin, vec direction) const;
	
	void createCache();
	void initPhotonCache();
};

