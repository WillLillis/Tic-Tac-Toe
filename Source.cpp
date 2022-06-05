#include "tttboard.h"

int main(void)
{
	PlayerSide winner;

	/*tttBoard* test = new tttBoard();
	test->DisplayBoard();

	delete test;*/

	winner = tttBoard::playGame();




	return 0;
}