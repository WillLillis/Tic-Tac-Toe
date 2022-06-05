#pragma once

#include<conio.h>
#include <ctype.h>
#include<stdlib.h>
#include<time.h>
#include<Windows.h>
#include<cstdint>
#include<assert.h>
#include<string>
#include <iostream>

#define ASCII_SPACE 32

inline void cinReset()
{
	std::cin.clear();
	#pragma push_macro("max")
	#undef max
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	#pragma pop_macro("max")
}

enum class PlayerSide {
	X,
	O,
	Cat
};

class tttPlayer {
private:
	PlayerSide player;
	std::string name;
public:
	tttPlayer()
	{
		player = PlayerSide::Cat;
		name = "Player";
	}
	// parametrized constructor, allows specification of player side ('X' or 'O'), as well as the player's name
	tttPlayer(PlayerSide which, std::string pname)
	{
		assert(which == PlayerSide::X || which == PlayerSide::O);
		player = which;
		name = pname;
	}
	// Destructor, is this even needed
	~tttPlayer()
	{
		;
	}
	void setPlayer(PlayerSide which)
	{
		assert(which == PlayerSide::X || which == PlayerSide::O);
		player = which;
	}
	void setName(std::string pname)
	{
		name = pname;
	}
	PlayerSide getplayer()
	{
		return player;
	}
	std::string getName()
	{
		return name;
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
		else if (player == PlayerSide::O)
		{
			return 'O';
		}
		else
		{
			return 'C'; // 'C' for "Cat"
		}
	}
	static std::string promptUserforName(uint8_t id)
	{
		char namebuffer[20];
		BOOL cinFlag;

		// Player 1 name selection
		std::cin.clear();
		do {
			printf("Player %d, what would you like to be called?\n", id);
			//scanf_s("%c", namebuffer, (unsigned)_countof(namebuffer));
			std::cin >> namebuffer;
			cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
			cinReset();
		} while (cinFlag);
		std::string name1(namebuffer);
		
		return name1;
	}
};

class tttBoard {
private:
	char* board;
	uint32_t rows;
	uint32_t cols;
	uint64_t numSpaces;
	tttPlayer Player1;
	tttPlayer Player2;
public:
	// Default constructor for a 3 x 3 board
	tttBoard()
	{
		board = new char[9]; // I have been told that using malloc is bad
		rows = 3;
		cols = 3;
		numSpaces = 9;
		for (uint8_t i = 0; i < 9; i++)
		{
			board[i] = 'U';
		}
		Player1.setPlayer(PlayerSide::X);
		Player1.setName("Player 1");
		Player2.setPlayer(PlayerSide::O);
		Player2.setName("Player 2");
	}
	// Parametrized constructor for an m x n board
	tttBoard(uint32_t inrows, uint32_t incols, std::string p1name, std::string p2name)
	{
		rows = inrows;
		cols = incols;
		numSpaces = (uint64_t)rows * (uint64_t)cols;
		board = new char[numSpaces];
		memset(board, 'U', sizeof(char) * (size_t)numSpaces);
		/*for (uint64_t i = 0; i < numSpaces; i++)
		{
			board[i] = 'U';
		}*/
		Player1.setPlayer(PlayerSide::X);
		Player1.setName(p1name);
		Player2.setPlayer(PlayerSide::O);
		Player2.setName(p2name);
	}
	// Destructor
	~tttBoard()
	{
		delete[] board;
	}
	// prints a single box from the board and its contents (X, O, or empty) to the screen
	static void printBoardBox(char boxContents)
	{
		printf("[%c]" , boxContents != 'U' ? boxContents : ASCII_SPACE); 
	}
	// display's the game's "main menu"
	static DWORD WINAPI displayMenu(volatile BOOL* keepgoing)
	{
		// whole lotta printfs
		printf("Hello Menu!\n");
		// steal big letter printing from peg game?

		while (*keepgoing)
		{
			;
		}

		return 0;
	}
	static void promptUserforBoardDimensions(uint32_t* rowout, uint32_t* colout)
	{
		uint32_t rowchoice, colchoice;
		BOOL cinFlag;

		std::cin.clear();
		do {
			printf("How many rows would you like the Tic-Tac-Toe board to have?\n");
			std::cin >> rowchoice;
			cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
			cinReset();
		} while (cinFlag);

		*rowout = rowchoice;

		do {
			printf("How many columns would you like the Tic-Tac-Toe board to have?\n");
			std::cin >> colchoice;
			cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
			cinReset();
		} while (cinFlag);

		*colout = colchoice;
	}
	// driver function for user initializing a game
	// displays the game menu first, then...
	// takes in user choices for names and board size,
	// passes these choices to the tttBoard constructor,
	// and then returns a pointer to the newly created tttBoard object
	static tttBoard* startGame()
	{
		std::string name1, name2;
		uint32_t rowchoice, colchoice;
		char menuchoice;
		BOOL cinFlag;
		volatile BOOL threadsync = TRUE;
		
		// spawn off thread to display menu
		// don't really need this, but will be useful if I want to add a more involved menu/ opening animation in the future
		// not sure why the casts are strictly necessary, but hey it compiles
		HANDLE threadhandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)displayMenu, (void*)&threadsync, 0, NULL);

		std::cin.clear();
		do {
			// temporary menu prints here until I get the menu worked out
			printf("Press 'X' to exit the game.\n");
			printf("Press 'P' to play the game.\n");
			std::cin >> menuchoice;
			cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
			cinReset();
		} while (cinFlag || (menuchoice != 'X' && menuchoice != 'P'));
		
		threadsync = FALSE;

		if (threadhandle != NULL)
		{
			WaitForSingleObject(threadhandle, INFINITE);
		}

		promptUserforBoardDimensions(&rowchoice, &colchoice);
		name1 = tttPlayer::promptUserforName(1);
		name2 = tttPlayer::promptUserforName(2);

		tttBoard* gameBoard = new tttBoard(rowchoice, colchoice, name1, name2);
		return gameBoard;
	}
	// driver function for playing the game
	static PlayerSide playGame()
	{
		BOOL gameover;
		PlayerSide winner;
		
		tttBoard* gameBoard = startGame();
		PlayerSide turn = PlayerSide::X;

		do {
			gameBoard->DisplayBoard();
			(turn == PlayerSide::X) ? gameBoard->MakeUserMove(gameBoard->Player1) : gameBoard->MakeUserMove(gameBoard->Player2);
			turn = (turn == PlayerSide::X) ? PlayerSide::O : PlayerSide::X;
			gameover = gameBoard->CheckEndConditions(&winner);
		} while (!gameover);

		gameBoard->DisplayBoard();
		printf("Game Over!\n");
		if (winner != PlayerSide::Cat)
		{
			printf("Congratulations, %s! You win!\n", (winner == PlayerSide::X) ? 
				gameBoard->Player1.getName().c_str() : gameBoard->Player2.getName().c_str());
		}
		else
		{
			printf("The game goes to the cat!\n");
		}

		delete gameBoard;
		return winner;
	}
	// displays board to console
	void DisplayBoard()
	{
		printf("  "); // padding required due to row labels
		// first print number labels over each of the columns
		for (uint32_t i = 0; i < cols; i++)
		{
			printf(" %u ", i);
		}

		printf("\n");

		// now print the actual contents of the board
		// along with labels at the start of each row
		for (uint32_t i = 0; i < rows; i++)
		{
			printf("%u ", i);
			for (uint32_t j = 0; j < cols; j++)
			{
				printBoardBox(board[(i * cols) + j]);
			}
			printf("\n");
		}
	}
	void MakeUserMove(tttPlayer inplayer)
	{
		// have main driver function pass in appropriate player for turn
		// prompt player for move
		// attempt move, return if successful, reprompt if not
		uint32_t userchoicecol, userchoicerow;
		BOOL cinFlag, placeCheck;
		std::cin.clear(); // simply clear the input buffer before we start
		do {
			do {
				printf("%s, in which column do you want to place your next %c?\n", inplayer.getName().c_str(), inplayer.playertoChar());
				//scanf_s("%d", &userchoice);
				std::cin >> userchoicecol;
				cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
				cinReset(); // then do a full flush afterwards
			} while ((userchoicecol >= cols) || cinFlag);

			do {
				printf("%s, in which row do you want to place your next %c?\n", inplayer.getName().c_str(), inplayer.playertoChar());
				//scanf_s("%d", &userchoice);
				std::cin >> userchoicerow;
				cinFlag = (std::cin.bad() || std::cin.fail()) ? TRUE : FALSE; // check if cin's error flags were set
				cinReset(); // then do a full flush afterwards
			} while ((userchoicerow >= rows) || cinFlag);
			placeCheck = placeMarker(inplayer, userchoicecol, userchoicerow);
		} while (!placeCheck);
	}
	// Place an 'X' or 'O' on the board
	// returns TRUE if the marker was placed, FALSE otherwise
	BOOL placeMarker(tttPlayer inplayer, uint32_t col, uint32_t row)
	{
		assert(col < cols);
		assert(row < rows);
		if (board[((uint64_t)row * (uint64_t)cols) + (uint64_t)col] == 'U')
		{
			board[((uint64_t)row * (uint64_t)cols) + (uint64_t)col] = inplayer.playertoChar();
			return TRUE;
		}
		return FALSE;
	}
	// checks if the winning/ ending conditions of the game have been met
	// returns TRUE if so, FALSE otherwise
	// winner arg returns with a X if P1 won, O if P2, and Cat if neither
	BOOL CheckEndConditions(PlayerSide* winner)
	{
		BOOL indic;
		BOOL gameover = TRUE;
		char first;

		for (uint64_t i = 0; i < numSpaces; i++) // iterate through all spaces
		{
			if (board[i] == 'U')
			{
				gameover = FALSE;
				break;
			}
		}

		// check rows
		for (uint32_t currRow = 0; currRow < rows; currRow++) // iterate through all rows
		{
			indic = TRUE;
			first = board[currRow * cols]; // grab first entry from the current row
			if (first != 'U') // avoid some uneccessary early game checks
			{
				for (uint32_t i = 1; i < cols; i++) // check each space in a given row
				{
					if (first != board[(currRow * cols) + i])
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
			first = board[currCol]; // grab first entry from the current column
			if (first != 'U') // avoid some uneccessary early game checks
			{
				for (uint32_t i = 1; i < rows; i++) // check each space in a given column
				{
					if (first != board[(cols * i) + currCol])
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
			first = board[0]; // grab first entry from the diagonal
			if (first != 'U')
			{
				for (uint32_t i = 1; i < cols; i++) // check each space in the diagonal, moving left to right as we go down
				{
					if (first != board[(cols * i) + i])
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

			// now top right to bottom left
			indic = TRUE;
			first = board[cols - 1]; // grab first entry from the diagonal
			if (first != 'U')
			{
				for (uint32_t i = 1; i < cols; i++) // check each space in the diagonal, moving right to left as we go down
				{
					if (first != board[(cols * i) + (cols - i - 1)])
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

		// no one has won 
		*winner = PlayerSide::Cat; // game 'goes to the cat'
		return gameover; 

	WinnerReturn:
		*winner = (first == 'X') ? PlayerSide::X : PlayerSide::O;
		return TRUE;
	}
};