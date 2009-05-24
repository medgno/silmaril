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

#include <GLUT/glut.h>

#include "Camera.h"
#include "PointCamera.h"


class GLUTCamera : public Camera {
private:
	static double around;
	static double up;
	static double distance;
	static int mouseStartButton;
	static int mouseX;
	static int mouseY;
	static PointCamera cam;
public:
	GLUTCamera();
	static void processMouseStart(int button, int state, int x, int y);
	static void processMouseMovement(int x, int y);
	virtual void map(double x, double y, vec &origin, vec &direction);
};


