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
#include "PhotonCache.h"
#include "constants.h"

PhotonCache::PhotonCache() { }

PhotonCache::PhotonCache(std::vector<HitInfo> &initInfo, std::vector<Color> &initColors) {
	infos = initInfo;
	colors = initColors;
	
	resolution = std::pow(infos.size(), .3) * 3;
	
	grid = new std::vector<Photon>[resolution * resolution * resolution];
	
	min = infos[0].location;
	max = infos[0].location;
	for (int x=0; x<infos.size(); x++) {
		for (int c=0; c<3; c++) {
			if (infos[x].location.d[c] < min.d[c])
				min.d[c] = infos[x].location.d[c];
			if (infos[x].location.d[c] > max.d[c])
				max.d[c] = infos[x].location.d[c];
		}
	}
	unit = (max - min) * (1.0 / (resolution - 1));
	
	vec mapped;
	Photon temp;
	
	for (int x=0; x<infos.size(); x++) {
		mapped = (infos[x].location - min) / unit;
		temp.color = &colors[x];
		temp.info = &infos[x];
		temp.time = infos[x].distance + infos[x].previousDistances;
		at(mapped.d[0], mapped.d[1], mapped.d[2])->push_back(temp);
	}
	
	weight = 1000.0 / initInfo.size();
	
	initInfo.resize(0);
	initColors.resize(0);
}

std::vector<Photon>* PhotonCache::at(int x, int y, int z) {
	if (x < 0 || x >= resolution ||
		y < 0 || y >= resolution ||
		z < 0 || z >= resolution) {
		std::cerr << "PhotonCache::at: invalid position." << std::endl;
	}
	return grid + x + y * resolution + z * resolution * resolution;
}

bool PhotonCache::inBounds(int x, int y, int z) {
	return (x >= 0) && (x < resolution)
	&& (y >= 0) && (y < resolution)
	&& (z >= 0) && (z < resolution);
}

Color PhotonCache::averageValue(vec location, double radius) {
	double radiusSq = radius * radius;
	int count = 0;
	Color accum;
	vec mapped = (location - min) / unit;
	
	
	vec range(radius, radius, radius, 0);
	range = range / unit;
	for (int x=mapped.d[0] - range.d[0]; x <= mapped.d[0] + range.d[0] + 1; x++)
		for (int y=mapped.d[1] - range.d[1]; y<= mapped.d[1] + range.d[1] + 1; y++)
			for (int z=mapped.d[2] - range.d[2]; z <= mapped.d[2] + range.d[2] + 1; z++) {
				if (!inBounds(x,y,z))
					continue;
				std::vector<Photon> *current = at(x,y,z);
				for (int s=0; s< current->size(); s++) {
					if (((*current)[s].info->location - location).magnitudeSq() < radiusSq) {
						count++;
						accum += (*(*current)[s].color);
					}
				}
			}
	if (count == 0)
		return Color (0, 0, 0);
	return accum * (1.0/count);
	//return accum * (.0001/std::pow(radius,2));
}

Color PhotonCache::averageValue(vec startAt, vec location, double radius) {
	vec direction = startAt - location;
	direction.normalize();
	double radiusSq = radius * radius;
	int count = 0;
	Color accum;
	vec mapped = (location - min) / unit;
	
	
	vec range(radius, radius, radius, 0);
	range = range / unit;
	for (int x=mapped.d[0] - range.d[0]; x <= mapped.d[0] + range.d[0] + 1; x++)
		for (int y=mapped.d[1] - range.d[1]; y<= mapped.d[1] + range.d[1] + 1; y++)
			for (int z=mapped.d[2] - range.d[2]; z <= mapped.d[2] + range.d[2] + 1; z++) {
				if (!inBounds(x,y,z))
					continue;
				std::vector<Photon> *current = at(x,y,z);
				for (int s=0; s< current->size(); s++) {
					if (((*current)[s].info->location - location).magnitudeSq() < radiusSq) {
						count++;
						accum += (*(*current)[s].color)
//							* (*current)[s].info->mat->pdf(incoming, direction, normal);
							* (*current)[s].info->mat->pdf((*current)[s].info->incoming, direction,
														   (*current)[s].info->normal);
					}
				}
			}
	if (count == 0)
		return Color (0, 0, 0);
	return accum * (1.0/count);
	//return accum * (.001/std::pow(radius,3));
}

Color PhotonCache::seenValue(vec origin, vec location, HitInfo *info, double radius) {
	vec direction = origin - location;
	direction.normalize();
	double radiusSq = radius * radius;
	double count = 0;
	Color accum;
	vec mapped = (location - min) / unit;
	
	
	vec range(radius, radius, radius, 0);
	range = range / unit;
	for (int x=mapped.d[0] - range.d[0]; x <= mapped.d[0] + range.d[0] + 1; x++)
		for (int y=mapped.d[1] - range.d[1]; y<= mapped.d[1] + range.d[1] + 1; y++)
			for (int z=mapped.d[2] - range.d[2]; z <= mapped.d[2] + range.d[2] + 1; z++) {
				if (!inBounds(x,y,z))
					continue;
				std::vector<Photon> *current = at(x,y,z);
				for (int s=0; s< current->size(); s++) {
					if (((*current)[s].info->location - location).magnitudeSq() < radiusSq) {
						double ratio = info->normal.dot((*current)[s].info->normal);
						if (ratio < 0)
							ratio = 0;
						count += ratio;
						/*accum += (*(*current)[s].color) * ratio * info->mat->getColor()
							* info->mat->pdf(direction, (*current)[s].info->incoming * -1,
											 info->normal);*/
						accum += info->mat->combineColor((*(*current)[s].color) * ratio *
														 info->mat->pdf(direction, (*current)[s].info->incoming * -1,
																		info->normal));
					}
				}
			}
	if (count <= EPSILON)
		return Color (0, 0, 0);
	//return accum * (1.0/count);
	return accum * (weight/std::pow(radius,2));
}

ColorTimeCache PhotonCache::seenTimeValue(vec origin, vec location, HitInfo *info, double radius) {
	ColorTimeCache ctc;
	vec direction = origin - location;
	direction.normalize();
	double radiusSq = radius * radius;
	double count = 0;
	double radweight = (weight/std::pow(radius,2));
	Color accum;
	vec mapped = (location - min) / unit;
	
	
	vec range(radius, radius, radius, 0);
	range = range / unit;
	for (int x=mapped.d[0] - range.d[0]; x <= mapped.d[0] + range.d[0] + 1; x++)
		for (int y=mapped.d[1] - range.d[1]; y<= mapped.d[1] + range.d[1] + 1; y++)
			for (int z=mapped.d[2] - range.d[2]; z <= mapped.d[2] + range.d[2] + 1; z++) {
				if (!inBounds(x,y,z))
					continue;
				std::vector<Photon> *current = at(x,y,z);
				for (int s=0; s< current->size(); s++) {
					if (((*current)[s].info->location - location).magnitudeSq() < radiusSq) {
						double ratio = info->normal.dot((*current)[s].info->normal);
						if (ratio < 0)
							continue;
						count += ratio;
						/*accum += (*(*current)[s].color) * ratio * info->mat->getColor()
						 * info->mat->pdf(direction, (*current)[s].info->incoming * -1,
						 info->normal);*/
						Color c = info->mat->combineColor((*(*current)[s].color) * ratio *
														 info->mat->pdf(direction, (*current)[s].info->incoming * -1,
																		info->normal)) * radweight;
						double time = (*current)[s].time;
						ctc.addSample(time, c);
					}
				}
			}
	return ctc;
}
