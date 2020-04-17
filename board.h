#ifndef BOARD_H
#define BOARD_H

#define BOARDWIDTH 9
#define BOARDHEIGHT 13
#define BOARDGOALWIDTH 2
#define LASTMOVESBUFFSIZE 100
#define ALPHABETAINF 1000000000
#define ALPHABETAWIN 1000000

class board {
private:
	static int threePowers[10];
    int startingPlayer;
	int bArr[BOARDWIDTH*BOARDHEIGHT];
	int pos;
	int turn;
	void makeConnection(int p, int d, int v, const bool rec);
	void connect(int p, int d);
	void block(int p, int d);
	void clear(int p, int d);

	int alphaBeta(int player, int depth, int alpha, int beta);
	int ratePosition(int player);
	bool move(int direction, bool verbose);

	int lastMovePos;
	int lastMoves[LASTMOVESBUFFSIZE];
	int lastMovesListSize;
	int lastMovesTurn;
public:
	board();
	board(const board& b);
	void setupBoard();
	int operator()(int position, int direction) const;

	int getTurn() const;
    int getBallPos() const;
	int getCurrentPosition() const;
	int getLastMovePosition() const;
	int getLastMovesListSize() const;
	int getLastMoves(int i) const;
    void setStartingPlayer(int player);

	int modifyPos(int position, int direction) const;
	int checkForWin();
	bool moveBall(int direction);
	int getBestMove(int depth);
};


#endif
