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
#include <fstream>
#include <sstream>

#include "InputHandler.h"

InputHandler::InputHandler(std::istream *initIn, std::string desc) {
	in = initIn;
	description = desc;
	line = 1;
	character = 1;
}

InputHandler::InputHandler(std::string filename) {
	in = new std::ifstream(filename.c_str());
	description = filename;
	line = 1;
	character = 1;
}

char InputHandler::readChar() {
	char data = in->get();
	if (data == '\n') {
		character = 1;
		line++;
	}
	else
		character++;
	return data;
}

std::string InputHandler::readToken() {
	
	swallowWhitespace();
	
	std::string accum;
	char c;
	c = in->get();
	character++;
	while (!isWhitespace(c)) {
		accum += c;
		c = in->get();
		character++;
	}
	
	swallowWhitespace();
	return accum;
}

double InputHandler::readDouble() {
	swallowWhitespace();
	std::string accum;
	char c;
	c = in->get();
	character++;
	while ((c >= '0' && c <= '9') || c == '-' || c == '.') {
		accum += c;
		c = in->get();
		character++;
	}
	character--;
	in->putback(c);
	
	std::istringstream converter(accum);
	double value;
	converter >> value;
	swallowWhitespace();
	return value;
	
}

int InputHandler::readInt() {
	swallowWhitespace();
	std::string accum;
	char c;
	c = in->get();
	character++;
	while ((c >= '0' && c <= '9') || c == '-' || c == '.') {
		accum += c;
		c = in->get();
		character++;
	}
	character--;
	in->putback(c);
	
	swallowWhitespace();
	
	std::istringstream converter(accum);
	int value;
	converter >> value;
	return value;
}

//FIXME: The character counting here is somewhat wrong.
bool InputHandler::expect(std::string expected) {
	swallowWhitespace();
	std::string accum;
	int nCharacter = character;
	int nLine = line;
	for (int i=0; i<expected.size() && !in->eof(); i++) {
		char recent = in->get();
		accum += recent;
		if (recent == ' ')
			nCharacter++;
		if (recent == '\t')
			nCharacter++;
		if (recent == '\n') {
			nCharacter = 1;
			nLine++;
		}
	}
	if (accum == expected) {
		character = nCharacter;
		line = nLine;
		return true;
	} else {
		//need to revert
		for (int i=accum.size()-1; i >= 0; i--) {
			in->putback(accum[i]);
		}
		return false;
	}
}

void InputHandler::swallowWhitespace() {
	char c;
	c = in->get();
	while (isWhitespace(c)) {
		if (c == ' ')
			character++;
		if (c == '\t')
			character++;
		if (c == '\n') {
			character = 1;
			line++;
		}
		if (c == '\r') {
			character = 1;
			line++;
		}
		c = in->get();
	}
	in->putback(c);
}

bool InputHandler::isWhitespace(char inQuestion) {
	return (inQuestion == ' ') || (inQuestion == '\n') || (inQuestion == '\t') || (inQuestion == '\r');
}

void InputHandler::currentPosition(std::ostream &out) {
	out << "In " << description << ", at line " << line << ", character " << character;
}

bool InputHandler::eof() {
	return in->eof();
}

std::string InputHandler::readLine() {
	char c = readChar();
	std::string s;
	while (c != '\n' && !eof()) {
		s += c;
		c = readChar();
	}
	return s;
}

std::string InputHandler::readString() {
	std::string s;
	swallowWhitespace();
	char c = readChar();
	if (c != '"') {
		std::cerr << "ASDF" << std::endl;
		return s;
	}
	c = readChar();
	while (c != '"' && !eof()) {
		s += c;
		c = readChar();
	}
	return s;
}

int InputHandler::getLine() {
	return line;
}

bool InputHandler::fail() {
	return !in || in->fail();
}
