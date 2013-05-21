/*
 * Case.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: vspathak
 */

#include "Case.h"

Case::Case() {
	value = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			myArray[i][j] = 0;
		}
	}
}

Case::~Case() {
	// TODO Auto-generated destructor stub
}

