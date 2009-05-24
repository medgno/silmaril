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

//FIXME: fix this up for windows, WRT binary files and the /n -> crlf thing.
//FIXME: Endianness is problematic (probably)
//FIXME: Reading files might only work with pfms written by writePFM()

#include <iostream>
#include <fstream>
#include <string>
#include "pfm.h"

pfmInfo readPFM(const char *filename)
{
	pfmInfo info;
	std::ifstream in(filename);
	
	std::string format;
	in >> format;
	if (format != "PF")
	{
		std::cerr << "Unexpected format: got " << format << " instead of PF." << std::endl;
		return info;
	}
	
	in >> info.width >> info.height;
	info.data = new float[info.width * info.height * 3];
	
	float scale;
	in >> scale;
	
	if (scale > 0)
	{
		std::cerr << "Unable to handle big-endian files. Try again later." << std::endl;
		return info;
	}
	in.get();
	in.read((char*)info.data, info.width * info.height * 3 * sizeof(float));
	return info;
}

void writePFM(const char *filename, pfmInfo info)
{
	std::ofstream out(filename);
	out << "PF" << std::endl;
	out << info.width << " " << info.height << std::endl;
	out << -1 << std::endl;
	/* 
	 FIXME: correctly detect float's endianness and properly output this line.
	 For the moment I'm assuming floats on x86(-64) are little-endian.
	 */
	
	out.write((char *)info.data, info.width*info.height*3*sizeof(float));
	
	out.close();
}
