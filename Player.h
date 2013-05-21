/*
 * Player.h
 *
 *  Created on: Apr 18, 2013
 *      Author: vspathak
 */

#ifndef PLAYER_H_
#define PLAYER_H_
#include <string>

using namespace std;

class Player {
public:
	string name;
	bool isMax;
	Player();
	Player(string myname, bool maxMin);
	virtual ~Player();
};

#endif /* PLAYER_H_ */
