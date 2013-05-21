/*
 * Player.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: vspathak
 */

#include "Player.h"

Player::Player() {
	name = "";
	isMax = true;
}
Player::Player(string myname, bool maxMin) {
	name = myname;
	isMax = maxMin;
}

Player::~Player() {
	// TODO Auto-generated destructor stub
}

