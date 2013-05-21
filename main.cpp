/*
 * main.cpp
 *
 *  Created on: Apr 18, 2013
 *      Author: vspathak
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <strings.h>
#include <vector>
#include <limits>

#include "Case.h"
#include "Player.h"

using namespace std;

int imin = std::numeric_limits<int>::min(); // -infinity
int imax = std::numeric_limits<int>::max(); // +infinity

void parseFile(char *fileName, vector<Case*>*cases) {
	string inputString;
	ifstream myfile(fileName);
	if (myfile.is_open()) {
		while (myfile.good() && (getline(myfile, inputString) != NULL)) {
			if (strncmp(inputString.c_str(), "case", 4) == 0) {
				Case *newCase = new Case();

				for (int i = 0; i < 8; ++i) {
					if (myfile.good()
							&& (getline(myfile, inputString) != NULL)) {
						for (int j = 0; j < 8; ++j) {
							if ((inputString[j] == '+')
									|| (inputString[j] == 'O')) {
								newCase->myArray[i][j] = 0;
							} else if (inputString[j] == 'A') {
								newCase->myArray[i][j] = 1;
							} else if (inputString[j] == 'k') {
								newCase->myArray[i][j] = 2;
							} else if (inputString[j] == 'B') {
								newCase->myArray[i][j] = -1;
							} else if (inputString[j] == 'K') {
								newCase->myArray[i][j] = -2;
							}
						}
					}
				}
				cases->push_back(newCase);
			}
		}
	}
}

char* ParseCommandLine(int argc, char *argv[], vector<Case*> *cases) {
	argc--, argv++;
	if (argc < 2) {
		cerr << "Malformed command. Correct format: hw4 input.txt output.txt"
				<< endl;
		exit(1);
	}
	parseFile(argv[0], cases);
	return argv[1];
}

int getHeuristicValue(Case * newCase) {
	int h = 0;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			h += newCase->myArray[i][j];
		}
	}
	return h;
}

void copyArray(int arrayFrom[8][8], int arrayTo[8][8]) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			arrayTo[i][j] = arrayFrom[i][j];
		}
	}
}

void printChildren(vector<Case*> *children) {
	std::vector<Case*>::iterator itr;
	for (itr = children->begin(); itr < children->end(); ++itr) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				cout << (*itr)->myArray[i][j];
			}
			cout << endl;
		}
		cout << "------------------------" << endl;
	}
}

void printAlphaBeta(int alpha, int beta, ofstream *output) {
	std::stringstream ss;
	ss << alpha;
	std::string alphaStr(ss.str());
	std::stringstream ss1;
	ss1 << beta;
	std::string betaStr(ss1.str());
	if (alpha == imin) {
		alphaStr = std::string("-Inf");
	} else if (alpha == imax) {
		alphaStr = std::string("Inf");
	}
	if (beta == imin) {
		betaStr = std::string("-Inf");
	} else if (beta == imax) {
		betaStr = std::string("Inf");
	}

	*output << "alpha=" << alphaStr << ", beta=" << betaStr << "." << endl;
}

void findAllChildren(Case* newCase, vector<Case*> *children, bool isMax) {
	//Find all the cases of jumps if any
	//This is same as finding all the cases where opponent (-1 or -2) is
	//on the diagonally opposite position. Spl case for kings
	if (isMax) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (newCase->myArray[i][j] == 1) {
					//Pawns
					if ((i > 1) && (j < 6)
							&& ((newCase->myArray[(i - 1)][(j + 1)] == -1)
									|| (newCase->myArray[i - 1][j + 1] == -2))) {
						if (newCase->myArray[i - 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << j + 2 << ")";
							children->push_back(nextCase);
						}
					}
					if ((i > 1) && (j > 1)
							&& ((newCase->myArray[i - 1][j - 1] == -1)
									|| (newCase->myArray[i - 1][j - 1] == -2))) {
						if (newCase->myArray[i - 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
				} else if (newCase->myArray[i][j] == 2) {
					//Kings
					if ((i > 1) && (j < 6)
							&& ((newCase->myArray[i - 1][j + 1] == -1)
									|| (newCase->myArray[i - 1][j + 1]) == -2)) {
						if (newCase->myArray[i - 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << (j + 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i > 1) && (j > 1)
							&& ((newCase->myArray[i - 1][j - 1] == -1)
									|| (newCase->myArray[i - 1][j - 1] == -2))) {
						if (newCase->myArray[i - 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i < 6) && (j < 6)
							&& ((newCase->myArray[i + 1][j + 1] == -1)
									|| (newCase->myArray[i + 1][j + 1] == -2))) {
						if (newCase->myArray[i + 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j + 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i < 6) && (j > 1)
							&& ((newCase->myArray[i + 1][j - 1] == -1)
									|| (newCase->myArray[i + 1][j - 1] == -2))) {
						if (newCase->myArray[i + 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
				}
			}
		}
	} else {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (newCase->myArray[i][j] == -1) {
					//Pawns
					if ((i < 6) && (j < 6)
							&& ((newCase->myArray[(i + 1)][(j + 1)] == 1)
									|| (newCase->myArray[i + 1][j + 1] == 2))) {
						if (newCase->myArray[i + 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j + 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i < 6) && (j > 1)
							&& ((newCase->myArray[i + 1][j - 1] == 1)
									|| (newCase->myArray[i + 1][j - 1] == 2))) {
						if (newCase->myArray[i + 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
				} else if (newCase->myArray[i][j] == -2) {
					//Kings
					if ((i > 1) && (j < 6)
							&& ((newCase->myArray[i - 1][j + 1] == 1)
									|| (newCase->myArray[i - 1][j + 1]) == 2)) {
						if (newCase->myArray[i - 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << (j + 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i > 1) && (j > 1)
							&& ((newCase->myArray[i - 1][j - 1] == 1)
									|| (newCase->myArray[i - 1][j - 1] == 2))) {
						if (newCase->myArray[i - 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i - 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i - 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i - 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i < 6) && (j < 6)
							&& ((newCase->myArray[i + 1][j + 1] == 1)
									|| (newCase->myArray[i + 1][j + 1] == 2))) {
						if (newCase->myArray[i + 2][j + 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j + 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j + 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j + 2) << ")";
							children->push_back(nextCase);
						}
					}
					if ((i < 6) && (j > 1)
							&& ((newCase->myArray[i + 1][j - 1] == 1)
									|| (newCase->myArray[i + 1][j - 1] == 2))) {
						if (newCase->myArray[i + 2][j - 2] == 0) {
							Case *nextCase = new Case();
							copyArray(newCase->myArray, nextCase->myArray);
							nextCase->myArray[i + 2][j - 2] =
									nextCase->myArray[i][j];
							nextCase->myArray[i + 1][j - 1] = 0;
							nextCase->myArray[i][j] = 0;
							nextCase->move << "(" << i << ", " << j << ") => ("
									<< (i + 2) << ", " << (j - 2) << ")";
							children->push_back(nextCase);
						}
					}
				}
			}
		}
	}

	//If there are no cases where you can jump then find all
	//the other possible moves
	//Find all moves for normal pawns
	//Find all possible moves for Kings
	if (children->empty()) {
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (isMax && newCase->myArray[i][j] == 1) {
					//Pawns
					if ((i > 0) && (j < 7)
							&& (newCase->myArray[(i - 1)][(j + 1)] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i - 1][j + 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i - 1) << ", " << (j + 1) << ")";
						children->push_back(nextCase);
					}
					if ((i > 0) && (j > 0)
							&& (newCase->myArray[i - 1][j - 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i - 1][j - 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i - 1) << ", " << (j - 1) << ")";
						children->push_back(nextCase);
					}
				} else if ((!isMax) && newCase->myArray[i][j] == -1) {
					//Pawns
					if ((i < 7) && (j < 7)
							&& (newCase->myArray[(i + 1)][(j + 1)] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i + 1][j + 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i + 1) << ", " << (j + 1) << ")";
						children->push_back(nextCase);
					}
					if ((i < 7) && (j > 0)
							&& (newCase->myArray[i + 1][j - 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i + 1][j - 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i + 1) << ", " << (j - 1) << ")";
						children->push_back(nextCase);
					}
				} else if ((isMax && newCase->myArray[i][j] == 2)
						|| ((!isMax) && newCase->myArray[i][j] == -2)) {
					//Kings
					if ((i > 0) && (j < 7)
							&& (newCase->myArray[i - 1][j + 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i - 1][j + 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i - 1) << ", " << (j + 1) << ")";
						children->push_back(nextCase);
					}
					if ((i > 0) && (j > 0)
							&& (newCase->myArray[i - 1][j - 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i - 1][j - 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i - 1) << ", " << (j - 1) << ")";
						children->push_back(nextCase);
					}
					if ((i < 7) && (j < 7)
							&& (newCase->myArray[i + 1][j + 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i + 1][j + 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i + 1) << ", " << (j + 1) << ")";
						children->push_back(nextCase);
					}
					if ((i < 7) && (j > 0)
							&& (newCase->myArray[i + 1][j - 1] == 0)) {
						Case *nextCase = new Case();
						copyArray(newCase->myArray, nextCase->myArray);
						nextCase->myArray[i + 1][j - 1] =
								nextCase->myArray[i][j];
						nextCase->myArray[i][j] = 0;
						nextCase->move << "(" << i << ", " << j << ") => ("
								<< (i + 1) << ", " << (j - 1) << ")";
						children->push_back(nextCase);
					}
				}
			}
		}
	}
}

/*
 * function alphabeta(node, depth, α, β, Player)
 *  if  depth = 0 or node is a terminal node
 *       return the heuristic value of node
 *   if  Player = MaxPlayer
 *       for each child of node
 *           α := max(α, alphabeta(child, depth-1, α, β, not(Player) ))
 *           if β ≤ α
 *               break                             (* Beta cut-off *)
 *       return α
 *   else
 *       for each child of node
 *           β := min(β, alphabeta(child, depth-1, α, β, not(Player) ))
 *           if β ≤ α
 *               break                             (* Alpha cut-off *)
 *       return β
 * (* Initial call *)
 * alphabeta(origin, depth, -infinity, +infinity, MaxPlayer)
 *
 */
int alphaBeta(Case *newCase, int depth, int alpha, int beta, Player *newPlayer,
		Player *notPlayer, ofstream *output) {
	if (depth == 0) {
		int temp = getHeuristicValue(newCase);
		newCase->value = temp;
		*output << "h=" << temp << "." << endl;
		return temp;
	}
	if (newPlayer->isMax) {
		vector<Case*> children;
		findAllChildren(newCase, &children, newPlayer->isMax);
		std::vector<Case*>::iterator itr;
		for (itr = children.begin(); itr < children.end(); ++itr) {
			for (int p = 0; p < 4 - depth; ++p) {
				*output << "|-";
			}
			*output << "A" << 5 - depth << ": " << (*itr)->move.str() << "."
					<< endl;
			alpha = max(alpha,
					alphaBeta(*itr, depth - 1, alpha, beta, notPlayer,
							newPlayer, output));
			if (beta <= alpha) {         //Beta cut-off
				++itr;
				if (itr < children.end()) {
					for (int p = 0; p < 4 - depth; ++p) {
						*output << "|-";
					}
					*output << "A" << 5 - depth << ": pruning ";
					//for each remaining children the branch is pruned
					int counter = 1;
					for (; itr < children.end(); ++itr) {
						if (counter == 1) {
							*output << (*itr)->move.str();
						} else {
							*output << ", " << (*itr)->move.str();
						}
						counter++;
					}
					*output << "; ";
					printAlphaBeta(alpha, beta, output);
				}
				break;
			}
		}
		newCase->value = imin;
		if (children.size() == 0) {
			newCase->value = getHeuristicValue(newCase);
			alpha = newCase->value;
		}
		for (itr = children.begin(); itr < children.end(); ++itr) {
			if ((*itr)->value > newCase->value) {
				newCase->value = (*itr)->value;
				newCase->nextBestMove.str("");
				newCase->nextBestMove << (*itr)->move.str();
			}
		}
		if (depth == 4) {
			*output << endl << "first move: " << newCase->nextBestMove.str()
					<< "." << endl;
		}
		return alpha;
	} else {
		vector<Case*> children;
		findAllChildren(newCase, &children, newPlayer->isMax);
		std::vector<Case*>::iterator itr;
		for (itr = children.begin(); itr < children.end(); ++itr) {
			for (int p = 0; p < 4 - depth; ++p) {
				*output << "|-";
			}
			if (depth == 1) {
				*output << "B" << 5 - depth << ": " << (*itr)->move.str()
						<< ";  ";
			} else {
				*output << "B" << 5 - depth << ": " << (*itr)->move.str() << "."
						<< endl;
			}
			beta = min(beta,
					alphaBeta(*itr, depth - 1, alpha, beta, notPlayer,
							newPlayer, output));
			if (beta <= alpha) {         //alpha cut-off
				++itr;
				if (itr < children.end()) {
					for (int p = 0; p < 4 - depth; ++p) {
						*output << "|-";
					}
					*output << "B" << 5 - depth << ": pruning ";
					//for each remaining children the branch is pruned
					int counter = 1;
					for (; itr < children.end(); ++itr) {
						if (counter == 1) {
							*output << (*itr)->move.str();
						} else {
							*output << ", " << (*itr)->move.str();
						}
						counter++;
					}
					*output << "; ";
					printAlphaBeta(alpha, beta, output);
				}
				break;
			}
		}
		newCase->value = imax;
		if (children.size() == 0) {
			newCase->value = getHeuristicValue(newCase);
			beta = newCase->value;
		}
		for (itr = children.begin(); itr < children.end(); ++itr) {
			if ((*itr)->value < newCase->value) {
				newCase->value = (*itr)->value;
				newCase->nextBestMove.str("");
				newCase->nextBestMove << (*itr)->move;
			}
		}
		return beta;
	}
	return 0;
}

int main(int argc, char **argv) {
	vector<Case*> cases;
	char* filename = ParseCommandLine(argc, argv, &cases);
	ofstream output;
	output.open(filename);
	std::vector<Case*>::iterator itr;
	for (itr = cases.begin(); itr < cases.end(); ++itr) {
		Player *A = new Player("A", true);
		Player *B = new Player("B", false);
		alphaBeta(*itr, 4, imin, imax, A, B, &output);
		//output << "--------------------------------------------" << endl;
	}
	output.close();
	return 0;
}

