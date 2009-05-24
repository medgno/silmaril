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

#include "dlas.h"

using namespace dlas;

class BoundingBox {
public:
	vec minExtent;
	vec maxExtent;
	bool contains(vec coords) const;
	bool isValid() const {
		return minExtent != maxExtent;
	}
	void combine(const BoundingBox &other);
	
	double hit(const vec& origin, const vec& direction);
};
