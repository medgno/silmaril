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

#include <iostream>
#include <string>

class InputHandler {
private:
	std::istream *in;
	std::string description;
	int line;
	int character;
public:
	InputHandler(std::istream *initIn, std::string desc);
	InputHandler(std::string filename);
	char readChar();
	std::string readToken();
	double readDouble();
	int readInt();
	bool expect(std::string expected);
	void swallowWhitespace();
	bool isWhitespace(char inQuestion);
	void currentPosition(std::ostream &out);
	bool eof();
	std::string readLine();
	std::string readString();
	int getLine();
	bool fail();
};
