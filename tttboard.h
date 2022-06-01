#pragma once

#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<cstdint>
#include<assert.h>
#include<string>

class player {
private:
	uint8_t whichplayer;
public:
	player(uint8_t which)
	{
		assert(which == 0 || which == 1);
		whichplayer = which;
	}
	uint8_t getplayer()
	{
		return whichplayer;
	}
	// 65clean this shit up later, idk how classes work
	std::string playertostring()
	{
		if (player::getplayer == 0)
		{
			return "P1";
		}
		else
		{
			return "P2";
		}
	}
	char playertoChar()
	{
		if (player::getplayer == 0)
		{
			return 'X';
		}
		else
		{
			return 'O';
		}
	}
};

class tttBoard {
private:
	char* board;
	uint32_t rows;
	uint32_t cols;
	uint64_t numspaces;
public:
	// Default constructor for a 3x3 board
	void board()
	{
		board = new char[9];
		rows = 3;
		cols = 3;
		for (uint8_t i = 0; i < 9; i++)
		{
			board[i] = 'U';
		}
	}
	// Parametrized constructor for an mxn board
	void board(uint32_t inrows, uint32_t incols)
	{
		rows = inrows;
		cols = incols;
		numspaces = rows * cols;
		board = new char[numspaces];
		for (uint64_t i = 0; i < numspaces; i++)
		{
			board[i] = 'U';
		}
	}
	// Destructor
	~tttBoard()
	{
		delete[] board;
	}
	// Place an 'X' or 'O' on the board
	// returns TRUE if the marker was placed, FALSE otherwise
	BOOL placeMarker(player inplayer, uint64_t pos)
	{
		assert(pos < numspaces);
		if (board[pos] == 'U')
		{
			board[pos] = inplayer.playertoChar();
			return TRUE;
		}
		return FALSE;
	}
	// checks if the end conditions of the game have been met
	// returns TRUE if so, FALSE otherwise
	// winner arg returns with a 0 if P1 won, 1 if P2 won, and 255 if the game isn't over
	BOOL checkendconditions(tttBoard* board, uint8_t* winner)
	{
		// to be implemented...

		// check rows

		// check columns

		// check diagonals
	}
};