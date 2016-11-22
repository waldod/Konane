/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: Delaney Waldo
 *
 * Created on October 31, 2016, 5:55 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 *
 */
int main(int argc, char* argv[]) {
	// "board.txt", "B"
	// first arg is name of .txt file
	// second arg is B or W
	FILE *fp;
	char player;

	fp = fopen(argv[1], "r");
	// error check.

	player = *argv[2];

	char board[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// don't check if null character because board should be right
			board[i][j] = (char)fgetc(fp);
		}
		// get rid of new character line at end of line.
		fgetc(fp);
	}

	char *mv, *oppmv;
	mv = malloc(sizeof(char)*5); // X#-Z#
	oppmv = malloc(sizeof(char)*5); // X#-Z#
	int play = 0;
	//input from opponent and displaying moves.
	// thread later?

	if (player != 'B') {
		// if we are not black player opponent goes first
		scanf("%s", oppmv);
	}

	while (play != 1) {

		//mv = getMove();
		//printf("%s", mv);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				// don't check if null character because board should be right
				printf("%c", board[i][j]);
			}
			// get rid of new character line at end of line.
			printf("\n");
		}



		scanf("%s", oppmv);
		/* adjust board according to opponents move
		newBoard(oppmv);
		calculate move from new board
		*/

		// if noMoves() play = 1
		play = 1; // until noMove() function made.

	}

	free(mv);
	free(oppmv);


    return (EXIT_SUCCESS);
}

/*
char* getMove()
{
 		will get the move from the algorithm
		check if the move is legal, if not get new  move.
}
*/
