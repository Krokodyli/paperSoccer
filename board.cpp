#include "board.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

int board::threePowers[10] = {0, 1, 3, 9, 27, 81, 243, 729, 2187, 6561};


void board::makeConnection(int p, int d, int v, const bool rec){
	if(d < 0 or d > 9)
		throw "wrong direction in makeConnection";
	if(d == 5)
		throw "wrong direction in makeConnection";

	if(bArr[p]/threePowers[d]%3 != 0 and v != 0) return;

	if(v == 0)
		bArr[p] -= threePowers[d]*operator()(p, d);
	else
		bArr[p] += threePowers[d]*v;
	if(rec) return;
	int x = p%BOARDWIDTH;
	int y = p/BOARDWIDTH;
	switch(d){
		case 1:
			if(x > 0 and y > 0)
				makeConnection(p-BOARDWIDTH-1, 10-d, v, true);
			break;
		case 2:
			if(y > 0)
				makeConnection(p-BOARDWIDTH, 10-d, v, true);
			break;
		case 3:
			if(y > 0 and x < BOARDWIDTH-1)
				makeConnection(p-BOARDWIDTH+1, 10-d, v, true);
			break;
		case 4:
			if(x > 0)
				makeConnection(p-1, 10-d, v, true);
			break;
		case 6:
			if(x < BOARDWIDTH-1)
				makeConnection(p+1, 10-d, v, true);
			break;
		case 7:
			if(x > 0 and y < BOARDHEIGHT-1)
				makeConnection(p+BOARDWIDTH-1, 10-d, v, true);
			break;
		case 8:
			if(y < BOARDHEIGHT-1)
				makeConnection(p+BOARDWIDTH, 10-d, v, true);
			break;
		case 9:
			if(x < BOARDWIDTH-1 and y < BOARDHEIGHT-1)
				makeConnection(p+BOARDWIDTH+1, 10-d, v, true);
			break;
	}
}

void board::connect(int p, int d){
	makeConnection(p, d, 1, false);
}

void board::block(int p, int d){
	makeConnection(p, d, 2, false);
}

void board::clear(int p, int d){
	makeConnection(p, d, 0, false);
}

int board::alphaBeta(int player, int depth, int alpha, int beta){
	if(depth == 0 or checkForWin() != 0) return ratePosition(player);
	if(turn == player){
		int val = -ALPHABETAINF;
		for(int i = 1; i <= 9; i++){
			if(i == 5 or operator()(pos, i) != 0) continue;
			int oldPos = pos;
			int oldTurn = turn;
			move(i, false);

			val = std::max(val, alphaBeta(player, depth-1, alpha, beta));

			pos = oldPos;
			turn = oldTurn;
			clear(pos, i);

			alpha = std::max(alpha, val);
			if(alpha >= beta)
				break;
		}
		return val;
	}
	else{
		int val = ALPHABETAINF;
		for(int i = 1; i <= 9; i++){
			if(i == 5 or operator()(pos, i) != 0) continue;
			int oldPos = pos;
			int oldTurn = turn;
			move(i, false);

			val = std::min(val, alphaBeta(player, depth-1, alpha, beta));

			pos = oldPos;
			turn = oldTurn;
			clear(pos, i);

			beta = std::min(beta, val);
			if(alpha >= beta)
				break;
		}
		return val;
	}
	return 0;
}

int board::ratePosition(int player){
	int win = checkForWin();
	if(win == player) return ALPHABETAWIN;
	if(win > 0) return -ALPHABETAWIN;
	if(player == 1) return BOARDHEIGHT-pos/BOARDWIDTH;
	else if(player == 2) return pos/BOARDWIDTH;
	return 0;
}

bool board::move(int direction, bool verbose){
	if(operator()(pos, direction) != 0) return false;

	if(verbose and turn != lastMovesTurn){
		lastMovesListSize = 0;
		lastMovesTurn = turn;
		lastMovePos = pos;
	}

	connect(pos, direction);
	pos = modifyPos(pos, direction);
	if(verbose){
		lastMoves[lastMovesListSize] = direction;
		lastMovesListSize++;
	}

	int connections = 0;
	for(int i = 1; i <= 9; i++){
		if(i == 5) continue;
		if(operator()(pos, i) == 1) connections++;
	}

	if(connections == 1)
		turn = (turn==1)?2:1;

	return true;
}

board::board(){
	startingPlayer = 1;
	lastMovesListSize = 0;
	setupBoard();
}

board::board(const board& b){
	for(int i = 0; i < BOARDWIDTH * BOARDHEIGHT; i++)
		bArr[i] = b.bArr[i];
	startingPlayer = b.startingPlayer;
	pos = b.pos;
	turn = b.turn;
	lastMovePos = b.lastMovePos;
	for(int i = 0; i < LASTMOVESBUFFSIZE; i++)
		lastMoves[i] = b.lastMoves[i];
	lastMovesListSize = b.lastMovesListSize;
	lastMovesTurn = b.lastMovesTurn;
}

void board::setupBoard(){
	for(int i = 0; i < BOARDWIDTH*BOARDHEIGHT; i++)
		bArr[i] = 0;

	// left wall upper corner
	int coord = BOARDWIDTH;
	block(coord, 1);
	block(coord, 2);
	block(coord, 3);
	block(coord, 4);
	block(coord, 7);
	connect(coord, 6);
	connect(coord, 8);
	// left wall
	for(int y = 2; y < BOARDHEIGHT-2; y++){
		coord = y*BOARDWIDTH;
		block(coord, 1);
		block(coord, 4);
		block(coord, 7);
		connect(coord, 2);
		connect(coord, 8);
	}	
	// left wall lower corner	
	coord = BOARDWIDTH*(BOARDHEIGHT-2);	
	block(coord, 1);
	block(coord, 8);
	block(coord, 9);
	block(coord, 4);
	block(coord, 7);
	connect(coord, 6);
	connect(coord, 2);
	// right wall upper corner
	coord = BOARDWIDTH+(BOARDWIDTH-1);
	block(coord, 1);
	block(coord, 2);
	block(coord, 3);
	block(coord, 6);
	block(coord, 9);
	connect(coord, 4);
	connect(coord, 8);
	// right wall
	for(int y = 3; y < BOARDHEIGHT-1; y++){
		coord = y*BOARDWIDTH-1;
		block(coord, 9);
		block(coord, 6);
		block(coord, 3);
		connect(coord, 2);
		connect(coord, 8);
	}
	// right wall lower corner
	coord = BOARDWIDTH*(BOARDHEIGHT-2)+BOARDWIDTH-1;
	block(coord, 6);
	block(coord, 3);
	block(coord, 9);
	block(coord, 8);
	block(coord, 7);
	connect(coord, 2);
	connect(coord, 4);
	// upper wall
	for(int x = 1; x < BOARDWIDTH-1; x++){
		coord = BOARDWIDTH+x;
		block(coord, 1);
		block(coord, 2);
		block(coord, 3);
		connect(coord, 4);
		connect(coord, 6);
		if(x == BOARDWIDTH/2-BOARDGOALWIDTH/2-1)
			x += BOARDGOALWIDTH+1;
	}
	// upper goal upper left corner
	coord = BOARDWIDTH/2-BOARDGOALWIDTH/2;
	block(coord, 1);
	block(coord, 2);
	block(coord, 3);
	block(coord, 4);
	connect(coord, 6);
	connect(coord, 8);
	// upper goal
	for(int x = BOARDWIDTH/2-BOARDGOALWIDTH/2+1; x < BOARDWIDTH/2+BOARDGOALWIDTH/2; x++){
		block(x, 1);
		block(x, 2);
		block(x, 3);
		connect(x, 4);
		connect(x, 6);
	}
	// upper goal upper right corner
	coord = BOARDWIDTH/2+BOARDGOALWIDTH/2;
	block(coord, 1);
	block(coord, 2);
	block(coord, 3);
	block(coord, 6);
	block(coord, 9);
	connect(coord, 4);
	connect(coord, 8);
	// upper goal lower left corner
	coord = BOARDWIDTH/2-BOARDGOALWIDTH/2+BOARDWIDTH;
	block(coord, 1);
	connect(coord, 2);
	connect(coord, 4);
	// upper goal lower right corner
	coord = BOARDWIDTH/2+BOARDGOALWIDTH/2+BOARDWIDTH;
	block(coord, 3);
	connect(coord, 2);
	connect(coord, 6);
	// lower wall
	for(int x = 1; x < BOARDWIDTH-1; x++){
		coord = BOARDWIDTH*(BOARDHEIGHT-2)+x;
		block(coord, 7);
		block(coord, 8);
		block(coord, 9);
		connect(coord, 4);
		connect(coord, 6);
		if(x == BOARDWIDTH/2-BOARDGOALWIDTH/2-1)
			x += BOARDGOALWIDTH+1;
	}
	// lower goal upper left corner
	coord = BOARDWIDTH/2-BOARDGOALWIDTH/2+(BOARDHEIGHT-2)*BOARDWIDTH;
	block(coord, 7);
	connect(coord, 4);
	connect(coord, 8);
	// lower goal
	for(int x = BOARDWIDTH/2-BOARDGOALWIDTH/2+1; x < BOARDWIDTH/2+BOARDGOALWIDTH/2; x++){
		coord = x+(BOARDHEIGHT-1)*BOARDWIDTH;
		block(coord, 7);
		block(coord, 8);
		block(coord, 9);
		connect(coord, 4);
		connect(coord, 6);
	}
	// lower goal lower left corner
	coord = BOARDWIDTH/2-BOARDGOALWIDTH/2 + (BOARDHEIGHT-1)*BOARDWIDTH;
	block(coord, 1);
	block(coord, 4);
	block(coord, 7);
	block(coord, 8);
	block(coord, 9);
	connect(coord, 2);
	connect(coord, 6);
	// lower goal upper right corner
	coord = BOARDWIDTH/2+BOARDGOALWIDTH/2+BOARDWIDTH*(BOARDHEIGHT-2);
	block(coord, 9);
	connect(coord, 8);
	connect(coord, 6);
	// lower goal lower right corner
	coord = BOARDWIDTH/2+BOARDGOALWIDTH/2+BOARDWIDTH*(BOARDHEIGHT-1);
	block(coord, 7);
	block(coord, 8);
	block(coord, 9);
	block(coord, 6);
	block(coord, 3);
	connect(coord, 2);
	connect(coord, 4);

	pos = BOARDWIDTH*(BOARDHEIGHT/2)+BOARDWIDTH/2;
	turn = startingPlayer;
	lastMovePos = pos;
	lastMovesListSize = 0;
	lastMovesTurn = 2;
}

int board::operator()(int p, int d) const{
	return (bArr[p]/threePowers[d]%3);
}

int board::getTurn() const{
	return turn;
}

int board::getBallPos() const{
	return pos;
}

int board::getCurrentPosition() const{
	return pos;
}

int board::getLastMovePosition() const{
	return lastMovePos;
}

int board::getLastMovesListSize() const{
	return lastMovesListSize;
}

int board::getLastMoves(int i) const{
	return lastMoves[i];
}

void board::setStartingPlayer(int player){
	if(player == 1 or player == 2)
	  startingPlayer = player;
}

int board::modifyPos(int position, int direction) const{
	switch(direction){
		case 1:
			return position - BOARDWIDTH - 1;
		case 2:
			return position - BOARDWIDTH;
		case 3:
			return position - BOARDWIDTH + 1;
		case 4:
			return position - 1;
		case 6:
			return position + 1;
		case 7:
			return position + BOARDWIDTH - 1;
		case 8:
			return position + BOARDWIDTH;
		case 9:
			return position + BOARDWIDTH + 1;
	}
	throw "Wrong direction";
}

int board::checkForWin(){
	if(pos/BOARDWIDTH == 0) return 1;
	if(pos/BOARDWIDTH == BOARDHEIGHT-1) return 2;

	int possMoves = 0;
	for(int i = 1; i <= 9; i++){
		if(i == 5) continue;
		if(operator()(pos, i) == 0) possMoves++;
	}
	if(possMoves == 0) return (turn==1)?2:1;

	return 0;
}

bool board::moveBall(int direction){
	return move(direction, true);
}

int board::getBestMove(int depth){
	int max = -ALPHABETAINF;
	int maxit = 1;
	for(int i = 1; i <= 9; i++){
		if(i == 5 or operator()(pos, i) != 0) continue;
		int oldPos = pos;
		int oldTurn = turn;
		move(i, false);

		int val = alphaBeta(oldTurn, depth-1, -ALPHABETAINF, ALPHABETAINF);

		pos = oldPos;
		turn = oldTurn;
		clear(pos, i);

		if(val > max)
		{
			max = val;
			maxit = i;
		}
	}
	return maxit;
}
