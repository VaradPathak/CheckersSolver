/*
 * Case.h
 *
 *  Created on: Apr 18, 2013
 *      Author: vspathak
 */

#ifndef CASE_H_
#define CASE_H_
#include <sstream>
#include <string>

using namespace std;

class Case {
public:
	std::stringstream nextBestMove;
	int myArray[8][8];
	std::stringstream move;
	int value;
	Case();
	virtual ~Case();
};

#endif /* CASE_H_ */
