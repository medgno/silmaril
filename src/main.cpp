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
#include <pthread.h>

#include <iostream>
#include <sstream>
#include <string>

#include <cmath>

#include "dlas.h"
#include "Sphere.h"
#include "Plane.h"
#include "DumbContainer.h"
#include "Lambert.h"
#include "Phong.h"
#include "PointCamera.h"
#include "GLUTCamera.h"
#include "GridContainer.h"
#include "SimpleSceneReader.h"
#include "Random.h"
#include "pfm.h"

using namespace dlas;

const int WIDTH=500;
const int HEIGHT=500;

bool singleFrame = false;//true;
bool done = false;

const int NUM_THREADS = 8;
const int GRANULARITY = 25;
pthread_mutex_t lock;
pthread_cond_t waiting;
pthread_cond_t finished;
pthread_t threads[NUM_THREADS];
bool dispatched[GRANULARITY][GRANULARITY];
int numremaining;

char buffers[2][HEIGHT][WIDTH][3];
float fbuffs[2][HEIGHT * WIDTH * 3];
ColorTimeCache *ctcs;
int activeBuf;

World *w;

void drawRegion(int xmin, int ymin, int xmax, int ymax);

void* runner(void *a) {
	while (true) {
		bool found = false;
		int fx, fy;
		pthread_mutex_lock(&lock);
		do {
			for (int x=0; x<GRANULARITY; x++)
				for (int y=0; y<GRANULARITY; y++)
					if (!dispatched[x][y]) {
						found = true;
						fx = x;
						fy = y;
					}
			if (!found)
				pthread_cond_wait(&waiting, &lock);
		} while (!found);
		dispatched[fx][fy] = true;
		pthread_mutex_unlock(&lock);
		
		drawRegion(WIDTH/GRANULARITY*fx, HEIGHT/GRANULARITY*fy,
				   WIDTH/GRANULARITY*(fx+1), HEIGHT/GRANULARITY*(fy+1));
		
		pthread_mutex_lock(&lock);
		numremaining--;
		std::cerr << "Number of chunks remaining: " << numremaining << ", "
			<< (1-numremaining * 1.0/(GRANULARITY * GRANULARITY)) * 100 << "%" << std::endl;
		pthread_cond_broadcast(&finished);
		pthread_mutex_unlock(&lock);
	}
	return 0;
}

void drawRegion(int xmin, int ymin, int xmax, int ymax) {
	for (int x=xmin; x<xmax; x++)
		for (int y=ymin; y<ymax; y++)
		{
			//pc.map(((double)x*2)/WIDTH-1,((double)y*2)/HEIGHT-1,origin, direction);
			//HitInfo i;
			//dc.hit(&i, origin, direction);
			ctcs[(y) * WIDTH + x] = w->trace(((double)x*2)/WIDTH-1,((double)y*2)/HEIGHT-1);
			Color c = ctcs[(HEIGHT - y) * WIDTH + x].avg();
			buffers[activeBuf][y][x][0] = c.intR();
			buffers[activeBuf][y][x][1] = c.intG();
			buffers[activeBuf][y][x][2] = c.intB();
			fbuffs[activeBuf][((HEIGHT - y) * WIDTH + x) * 3 + 0] = (float)c.r;
			fbuffs[activeBuf][((HEIGHT - y) * WIDTH + x) * 3 + 1] = (float)c.g;
			fbuffs[activeBuf][((HEIGHT - y) * WIDTH + x) * 3 + 2] = (float)c.b;
		}
}

void swapAndGo() {
	pthread_mutex_lock(&lock);
	activeBuf = 1 - activeBuf;
	for (int x=0; x<GRANULARITY; x++)
		for (int y=0; y<GRANULARITY; y++)
			dispatched[x][y] = false;
	numremaining = GRANULARITY*GRANULARITY;
	pthread_cond_broadcast(&waiting);
	pthread_mutex_unlock(&lock);
}

void writeAnimation(std::string prepath, std::string name) {
	float frame[HEIGHT * WIDTH * 3 + 30];
	for (int f=0; f<w->numFrames; f++) {
		if (f%10 == 0)
			std::cerr << "Writing frame " << f << std::endl;
		for (int x=0; x<WIDTH; x++)
			for (int y=0; y<HEIGHT; y++) {
				Color c = ctcs[y * WIDTH + x].filter(f*w->lightspeed, 8.5 * w->lightspeed);
				frame[(y * WIDTH + x) * 3 + 0] = c.r;
				frame[(y * WIDTH + x) * 3 + 1] = c.g;
				frame[(y * WIDTH + x) * 3 + 2] = c.b;
				
			}
		pfmInfo info;
		info.data = frame;
		info.height = HEIGHT;
		info.width = WIDTH;
		
		std::ostringstream ss;
		ss << prepath << name << f << ".pfm";
		
		writePFM(ss.str().c_str(), info);
	}
}
	

void myDisplay()
{
	pthread_mutex_lock(&lock);
	while (numremaining != 0)
		pthread_cond_wait(&finished, &lock);
	pthread_mutex_unlock(&lock);
	if (!singleFrame)
		swapAndGo();
	else {
		if (!done)
			activeBuf = 1 - activeBuf;
		done = true;
	}
	
	pfmInfo info;
	info.width = WIDTH;
	info.height = HEIGHT;
	info.data = fbuffs[1 - activeBuf];
	
	//writePFM("/Users/medgno/project/silmaril/out.pfm", info);
	std::cerr << "Writing animation" << std::endl;
	writeAnimation(".", "anim");
}

int main (int argc, char *  argv[]) {
	srand(time(0));
	std::string inputfile = "scene.inp";
	if (argc > 1)
		inputfile = argv[1];
	SimpleSceneReader sr(inputfile);
	w = sr.world;
	Random::init();
	w->createCache();
	//w->initPhotonCache();
	singleFrame = sr.singleFrame;
	ctcs = new ColorTimeCache[(HEIGHT + 1) * (WIDTH + 1)];
	
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&waiting, NULL);
	pthread_cond_init(&finished, NULL);
	
	for (int x=0; x<GRANULARITY; x++)
		for (int y=0; y<GRANULARITY; y++)
			dispatched[GRANULARITY][GRANULARITY] = false;
	
	activeBuf = 1;
	
	for (int t=0; t<NUM_THREADS; t++) {
		pthread_create(&threads[t], NULL, runner, NULL);
		pthread_detach(threads[t]);
	}

	myDisplay();
	
}
