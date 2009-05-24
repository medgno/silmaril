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
#include <iostream>

#include "World.h"
#include "Random.h"
#include "constants.h"

World::World(Thing *obj, Color bg, Camera *c, int d, int s) {
	objects = obj;
	background = bg;
	depth = d;
	cam = c;
	samples = s;
}

//FIXME: Some more dignified way to choose the rendering method would be
// a very nice thing.
ColorTimeCache World::trace(double x, double y) const {
	vec origin, direction;
	if (samples == 1) {
		cam->map(x, y, origin, direction);
		//return runBiDi(origin, direction);
		return runCache(origin, direction);
		//return runPhotonCache(origin, direction);
		//return runTimePhotonCache(origin, direction);
		//return runDirectPhoton(origin, direction);
		//return runSurfacePhoton(origin, direction);
		
	}
	ColorTimeCache answer;
	double weight = 1.0/samples;
	double ox, oy;
	
	for (int j=0; j<samples; j++) {
		ox = x + Random::randomCenteredUnit()/500;
		oy = y + Random::randomCenteredUnit()/500;
		cam->map(ox, oy, origin, direction);
		//answer += runBiDi(origin, direction) * weight;
		answer.addWeighted(0.0, 1.0, runCache(origin, direction));
		//answer += runPhotonCache(origin, direction) * weight;
		//answer.addWeighted(0.0, weight, runTimePhotonCache(origin, direction));
		//answer += runDirectPhoton(origin, direction) * weight;
		//answer += runSurfacePhoton(origin, direction) * weight;
	}
	return answer;
	
}

Color World::runBiDi(vec origin, vec direction) const {
	Thing *emitter = emitters[Random::upto(emitters.size())];
	
	std::vector<Color> cumEmtWeight;
	std::vector<HitInfo> emtHits;
	
	std::vector<Color> cumEyeWeight;
	std::vector<HitInfo> eyeHits;
	std::vector<int> todo;
	
	cumEmtWeight.reserve(depth);
	emtHits.reserve(depth);
	
	cumEyeWeight.reserve(depth);
	eyeHits.reserve(depth);
	todo.reserve(depth);
	
	HitInfo emtInfo;
	vec emtN;
	vec source;
	source = emitter->randomSurfacePoint(emtN);
	emtInfo.hit = true;
	emtInfo.incoming = emtN;
	emtInfo.normal = emtN;
	emtInfo.location = source;
	emtInfo.mat = emitter->getMaterial();
	cumEmtWeight.push_back(emitter->getMaterial()->getColor());
	emtHits.push_back(emtInfo);
	bool done = false;
	for (int d = 1; d < depth && !done; d++) {
		HitInfo newI;
		newI.follow(emtHits[d-1]);
		
		vec newDirection = emtHits[d-1].mat->sample(emtHits[d-1].incoming, emtHits[d-1].normal);
		
		objects->hit(&newI, emtHits[d-1].location, newDirection);
		
		cumEmtWeight.push_back(emtHits[d-1].mat->combineColor(
							cumEmtWeight[d-1] * emtHits[d-1].mat->sampledPdf(
								emtHits[d-1].incoming, newDirection, emtHits[d-1].normal)));
		if (newI.hit)
			emtHits.push_back(newI);
		else
			done = true;
	}
	
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	
	done = false;
	if (!eyeInfo.hit)
		return Color(0,0,0);
	cumEyeWeight.push_back(eyeInfo.mat->getColor());
	eyeHits.push_back(eyeInfo);
	/*for (int d = 1; d < depth && !done; d++) {
		HitInfo newI;
		newI.follow(eyeHits[d-1]);
		
		vec newDirection = eyeHits[d-1].mat->sample(eyeHits[d-1].incoming, eyeHits[d-1].normal);
		
		objects->hit(&newI, eyeHits[d-1].location, newDirection);
		
		cumEyeWeight.push_back(eyeHits[d-1].mat->combineColor(
							   cumEyeWeight[d-1] * eyeHits[d-1].mat->sampledPdf(
									eyeHits[d-1].incoming, newDirection, eyeHits[d-1].normal)));
		if (newI.hit)
			eyeHits.push_back(newI);
		else
			done = true;
	}*/
	
	for (int d = 1; d < depth && !todo.empty(); d++) {
		HitInfo newI;
		int index = todo.back();
		todo.pop_back();
		newI.follow(eyeHits[index]);
		
		vec newDirection = eyeHits[index].mat->sample(eyeHits[index].incoming, eyeHits[index].normal);
		
		objects->hit(&newI, eyeHits[index].location, newDirection);
		
		if (newI.hit) {
			Color newc = eyeHits[index].mat->combineColor(
				cumEyeWeight[index] * eyeHits[index].mat->sampledPdf(
					eyeHits[index].incoming, newDirection, eyeHits[index].normal));
			int ints = 1;//newc.intensity() * 5 + 1;
			for (int x=0; x<ints; x++)
				todo.push_back(eyeHits.size());
			eyeHits.push_back(newI);
			cumEyeWeight.push_back(newc * (1.0/ints));
		}
		else
			done = true;
	}
	
	Color accum;
	for (int x=0; x<emtHits.size(); x++)
		for (int y=0; y<eyeHits.size(); y++) {
			vec dir = eyeHits[y].location - emtHits[x].location;
			double dist = dir.magnitude();
			dir.normalize();
			HitInfo inf;
			inf.distanceOnly = true;
			inf.distance = dist;
			objects->hit(&inf, emtHits[x].location, dir);
			if (inf.distance >= dist - EPSILON || !inf.hit) {
				//hit
				accum += cumEyeWeight[y] * cumEmtWeight[x] *
				eyeHits[y].mat->pdf(eyeHits[y].incoming, dir * -1, eyeHits[y].normal) * 
					emtHits[x].mat->pdf(emtHits[x].incoming, dir, emtHits[x].normal);
			}
		}
	return accum;
	/*if (eyeInfo.hit) {
		vec newn;
		vec dtl = eyeInfo.location - emitter->randomSurfacePoint(newn);
		return emitter->getMaterial()->getColor() * eyeInfo.mat->pdf(direction*-1, dtl, eyeInfo.normal);
	}
	else
		return Color(0,0,0);*/
}

ColorTimeCache World::runCache(vec origin, vec direction) const {
	ColorTimeCache accum;
	std::vector<Color> cumEyeWeight;
	std::vector<HitInfo> eyeHits;
	std::vector<int> todo;
	std::vector<double> mult;
	todo.reserve(depth);
	cumEyeWeight.reserve(depth); //8s without, 8s with, but should make a difference.
	eyeHits.reserve(depth);
	mult.reserve(depth);
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	
	bool done = false;
	if (!eyeInfo.hit)
		return accum;
	
	Color newc = eyeInfo.mat->getColor();
	int ints = 1;//newc.intensity() * (depth/2) + 1;
	for (int x=0; x<ints; x++)
		todo.push_back(eyeHits.size());
	cumEyeWeight.push_back(newc);
	eyeHits.push_back(eyeInfo);
	mult.push_back(1.0/ints);
	
	for (int d = 1; d < depth && !todo.empty(); d++) {
		HitInfo newI;
		int index = todo.back();
		todo.pop_back();
		newI.follow(eyeHits[index]);
		
		vec newDirection = eyeHits[index].mat->sample(eyeHits[index].incoming, eyeHits[index].normal);
		
		objects->hit(&newI, eyeHits[index].location, newDirection);
		
		if (newI.hit) {
			Color newc = newI.mat->combineColor(
				cumEyeWeight[index] * eyeHits[index].mat->sampledPdf(
					eyeHits[index].incoming, newDirection, eyeHits[index].normal));
//			newc *= mult[index];
			int ints = 1;//newc.intensity() * (depth/2) + 1;
			for (int x=0; x<ints; x++)
				todo.push_back(eyeHits.size());
			eyeHits.push_back(newI);
			cumEyeWeight.push_back(newc);
			mult.push_back(1.0/ints);
		}
		else
			done = true;
	}
	
	for (int x=0; x<depth; x++) {
		for (int y=0; y<eyeHits.size(); y++) {
			int index = Random::upto(cacheSize);
			vec dir = eyeHits[y].location - hicache[index].location;
			double dist = dir.magnitude();
			if (dist < EPSILON)
				continue;
			dir.normalize();
			HitInfo inf;
			inf.distanceOnly = true;
			inf.distance = dist;
			objects->hit(&inf, hicache[index].location, dir);
			if (inf.distance >= dist - EPSILON || !inf.hit) {
				//hit
				Color result = cumEyeWeight[y] * colorcache[index] * cacheweight *
				eyeHits[y].mat->pdf(eyeHits[y].incoming, dir * -1, eyeHits[y].normal) * 
				hicache[index].mat->pdf(hicache[index].incoming, dir, hicache[index].normal) * (1/BOUNCE_MULT);
				accum.addSample(hicache[index].distance + hicache[index].previousDistances
								+ eyeHits[y].distance + eyeHits[y].previousDistances + inf.distance, result);
			}
		}
	}
	return accum;
}

void World::hit(HitInfo *info, vec origin, vec direction) const {
	
}

void World::createCache() {
	Thing *emitter;
	HitInfo c;
	int length =0;
	int resetcount = 1;
	HitInfo emtInfo;
	vec emtN;
	vec source;
	emitter = emitters[Random::upto(emitters.size())];
	source = emitter->randomSurfacePoint(emtN);
	emtInfo.hit = true;
	emtInfo.incoming = emtN;
	emtInfo.normal = emtN;
	emtInfo.location = source;
	emtInfo.mat = emitter->getMaterial();
	colorcache.push_back(emitter->getMaterial()->getColor());
	hicache.push_back(emtInfo);
	bool done = false;
	for (int d = 1; d < cacheSize; d++) {
		int i = d - 1;
		double ints = colorcache[i].intensity();
		if (ints < .01 || length > 60 || done) {
			emitter = emitters[Random::upto(emitters.size())];
			source = emitter->randomSurfacePoint(emtN);
			emtInfo.distance = 0;
			emtInfo.previousDistances = 0;
			emtInfo.hit = true;
			emtInfo.incoming = emtN;
			emtInfo.normal = emtN;
			emtInfo.location = source;
			emtInfo.mat = emitter->getMaterial();
			int intes = 1;//emitter->getMaterial()->getColor().intensity() + 1;
			for (int x=0; x<intes; x++)	{
				colorcache.push_back(emitter->getMaterial()->getColor() * (1.0/intes));
				hicache.push_back(emtInfo);
			}
			resetcount++;
			d += intes-1;
			done = false;
			length = 0;
			continue;
		}
		length++;
		HitInfo newI;
		newI.follow(hicache[d-1]);
		
		vec newDirection = hicache[d-1].mat->sample(hicache[d-1].incoming, hicache[d-1].normal);
		
		objects->hit(&newI, hicache[d-1].location, newDirection);
		
		if (newI.hit) {
			Color newc = hicache[d-1].mat->combineColor(colorcache[d-1] *
				hicache[d-1].mat->sampledPdf(hicache[d-1].incoming, newDirection, hicache[d-1].normal));
			int intes = 1;//newc.intensity()*3 + 1;
			for (int x=0; x<intes; x++) {
				hicache.push_back(newI);
				colorcache.push_back(newc * (1.0/intes));
			}
			d += intes-1;
		}
		else {
			done = true;
			d--;
		}
	}
	cacheweight = (cacheSize * 1.0 / resetcount);
}

Color World::runPhotonCache(vec origin, vec direction) const {
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	
	if (!eyeInfo.hit)
		return Color(0,0,0);
	
	
	Color initc = eyeInfo.mat->getColor();
	Color accum;
	
	for (int d = 0; d < depth; d++) {
		HitInfo newI;
		newI.follow(eyeInfo);
		
		vec newDirection = eyeInfo.mat->sample(eyeInfo.incoming, eyeInfo.normal);
		
		objects->hit(&newI, eyeInfo.location, newDirection);
		
		if (newI.hit) {
			Color newc = initc * eyeInfo.mat->getColor() *
						Color(1.0, 1.0, 1.0) *
			eyeInfo.mat->sampledPdf(eyeInfo.incoming, newDirection, eyeInfo.normal);
			accum += newc * pc->seenValue(eyeInfo.location, newI.location, &newI, sampleRadius) * (1.0 / depth);
		}
	}
	
	return accum;
}

Color World::runDirectPhoton(vec origin, vec direction) const {
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	if (!eyeInfo.hit)
		return Color(0,0,0);
	return eyeInfo.mat->getColor() * pc->averageValue(eyeInfo.location, sampleRadius);
}

Color World::runSurfacePhoton(vec origin, vec direction) const {
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	if (!eyeInfo.hit)
		return Color(0,0,0);
	return pc->seenValue(origin, eyeInfo.location, &eyeInfo, sampleRadius);
}

void World::initPhotonCache() {
	if (!pc) {
		pc = new PhotonCache(hicache, colorcache);
	}
}

ColorTimeCache World::runTimePhotonCache(vec origin, vec direction) const {
	ColorTimeCache ctc;
	HitInfo eyeInfo;
	objects->hit(&eyeInfo, origin, direction);
	
	if (!eyeInfo.hit)
		return ctc;
	
	
	Color initc = eyeInfo.mat->getColor();
	
	for (int d = 0; d < depth; d++) {
		HitInfo newI;
		newI.follow(eyeInfo);
		
		vec newDirection = eyeInfo.mat->sample(eyeInfo.incoming, eyeInfo.normal);
		
		objects->hit(&newI, eyeInfo.location, newDirection);
		
		if (newI.hit) {
			Color newc = initc * eyeInfo.mat->getColor() * (1.0/depth) *
			eyeInfo.mat->sampledPdf(eyeInfo.incoming, newDirection, eyeInfo.normal);
			
			ctc.addWeighted(newI.distance + eyeInfo.distance, newc,
							pc->seenTimeValue(eyeInfo.location, newI.location, &newI, sampleRadius));
		}
	}
	return ctc;
}
