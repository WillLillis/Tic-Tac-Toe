#pragma once

#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<cstdint>
#include<assert.h>
#include<string>

enum PlayerSide {
	X,
	O,
	Cat
};


class tttPlayer {
private:
	PlayerSide player;
public:
	// constructor, just allows specificiation of P1 (X's) or P2 (O's)
	tttPlayer(PlayerSide which)
	{
		assert(which == PlayerSide::X || which == PlayerSide::O);
		player = which;
	}
	// Destructor, is this even needed
	~tttPlayer()
	{
		;
	}
	PlayerSide getplayer()
	{
		return player;
	}
	std::string playertoString()
	{
		if (player == PlayerSide::X)
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
		if (player == PlayerSide::X)
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
	// Default constructor for a 3 x 3 board
	tttBoard()
	{
		board = new char[9]; // I have been told that using malloc is bad
		rows = 3;
		cols = 3;
		numspaces = 9;
		for (uint8_t i = 0; i < 9; i++)
		{
			board[i] = 'U';
		}
	}
	// Parametrized constructor for an m x n board
	tttBoard(uint32_t inrows, uint32_t incols)
	{
		rows = inrows;
		cols = incols;
		numspaces = (uint64_t)rows * (uint64_t)cols;
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
	BOOL placeMarker(tttPlayer inplayer, uint64_t pos)
	{
		assert(pos < numspaces);
		if (board[pos] == 'U')
		{
			board[pos] = inplayer.playertoChar();
			return TRUE;
		}
		return FALSE;
	}
	// checks if the winning/ ending conditions of the game have been met
	// returns TRUE if so, FALSE otherwise
	// winner arg returns with a X if P1 won, O if P2, and Cat if neither
	BOOL CheckEndConditions(tttBoard* board, PlayerSide* winner)
	{
		BOOL indic;
		BOOL gameover = TRUE;
		char first;

		for (uint64_t i = 0; i < numspaces; i++) // iterate through all spaces
		{
			if (board->board[i] == 'U')
			{
				gameover = FALSE;
				break;
			}
		}

		// check rows
		for (uint32_t currRow = 0; currRow < rows; currRow++) // iterate through all rows
		{
			indic = TRUE;
			first = board->board[currRow * cols]; // grab first entry from the current row
			if (first != 'U') // avoid some uneccessary early game checks
			{
				for (uint32_t i = 1; i < cols; i++) // check each space in a given row
				{
					if (first != board->board[(currRow * cols) + i])
					{
						indic = FALSE;
						break;
					}
				}
				// check results
				if (indic) 
				{
					goto WinnerReturn;
				}
			}
		}

		// check columns
		for (uint32_t currCol = 0; currCol < cols; currCol++) // iterate through all rows
		{
			indic = TRUE;
			first = board->board[currCol]; // grab first entry from the current column
			if (first != 'U') // avoid some uneccessary early game checks
			{
				for (uint32_t i = 1; i < rows; i++) // check each space in a given column
				{
					if (first != board->board[(cols * i) + currCol])
					{
						indic = FALSE;
						break;
					}
				}
				// check results
				if (indic)
				{
					goto WinnerReturn;
				}
			}
		}
		// check diagonals
		if (rows == cols) // first need to make sure diagonals make sense on the given board
		{
			// top left to bottom right first
			indic = TRUE;
			first = board->board[0]; // grab first entry from the current row
			for (uint32_t i = 1; i < cols; i++) // check each space in a given column
			{
				if (first != board->board[(cols * i) + i])
				{
					indic = FALSE;
					break;
				}
			}
			// check results
			if (indic)
			{
				goto WinnerReturn;
			}

			// now top right to bottom left
			// top left to bottom right first
			indic = TRUE;
			first = board->board[cols - 1]; // grab first entry from the current row
			for (uint32_t i = 1; i < cols; i++) // check each space in a given column
			{
				if (first != board->board[(cols * i) + (cols - i - 1)])
				{
					indic = FALSE;
					break;
				}
			}
			// check results
			if (indic)
			{
				goto WinnerReturn;
			}
		}

		// no one has won 
		*winner = PlayerSide::Cat; // games 'goes to the cat'
		return gameover; 

	WinnerReturn:
		*winner = (first == 'X') ? PlayerSide::X : PlayerSide::O;
		return TRUE;
	}
};