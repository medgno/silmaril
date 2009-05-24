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

#include "GLUTCamera.h"
#include "constants.h"


double GLUTCamera::around = .3;
double GLUTCamera::up = .3;
double GLUTCamera::distance = 18;
int GLUTCamera::mouseStartButton = 0;
int GLUTCamera::mouseX = 0;
int GLUTCamera::mouseY = 0;
PointCamera GLUTCamera::cam;

GLUTCamera::GLUTCamera() {
	vec eye(std::cos(around)*std::cos(up)*distance,
			std::sin(around)*std::cos(up)*distance,
			std::sin(up)*distance,0);
	vec look(0,0,0,0);
	vec camup(0,0,1,0);
	cam = PointCamera(eye, look, camup);
}

void GLUTCamera::processMouseStart(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		mouseStartButton = button;
		mouseX = x;
		mouseY = y;
	}
}
	
void GLUTCamera::processMouseMovement(int x, int y) {
	if (mouseStartButton == GLUT_LEFT_BUTTON) {
		double dx = (mouseX - x) / 200.0;
		double dy = (mouseY - y) / 300.0;
		
		mouseX = x;
		mouseY = y;
		
		around -= dx;
		up -= dy;
		
		while (around > TWO_PI)
			around -= TWO_PI;
		while (around < 0)
			around += TWO_PI;
		
		if (up > HALF_PI)
			up = HALF_PI;
		if (up < .1)
			up = .1;
	} else if (mouseStartButton == GLUT_MIDDLE_BUTTON) {
		double dy = (mouseY - y) / 30.0;
		mouseY = y;
		distance -= dy;
		if (distance < .1)
			distance = .1;
	}
	vec eye(std::cos(around)*std::cos(up)*distance,
			std::sin(around)*std::cos(up)*distance,
			std::sin(up)*distance,0);
	vec look(0,0,0,0);
	vec camup(0,0,1,0);
	cam = PointCamera(eye, look, camup);
}
	
void GLUTCamera::map(double x, double y, vec &origin, vec &direction) {
	cam.map(x, y, origin, direction);
}

