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

class PhotonCache;

#include <vector>
#include "dlas.h"
#include "HitInfo.h"
#include "Color.h"
#include "ColorTimeCache.h"

using namespace dlas;

struct Photon {
public:
	HitInfo* info;
	Color* color;
	double time;
};

class PhotonCache {
private:
	std::vector<HitInfo> infos;
	std::vector<Color> colors;
	std::vector<Photon> *grid;
	int resolution;
	vec min;
	vec max;
	vec unit;
	double weight;
public:
	PhotonCache();
	PhotonCache(std::vector<HitInfo>&, std::vector<Color>&);
	
	std::vector<Photon>* at(int x, int y, int z);
	
	bool inBounds(int x, int y, int z);
	
	Color averageValue(vec location, double radius);
	
	Color averageValue(vec startAt, vec location, double radius);
	
	Color seenValue(vec origin, vec location, HitInfo *info, double radius);
	
	ColorTimeCache seenTimeValue(vec origin, vec location, HitInfo *info, double radius);
};
