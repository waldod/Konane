/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: Majo
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
	// PLAYER = c

	char board[8][8];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// don't check if null character because board should be right
			board[i][j] = fgetc(fp);
		}
		// get rid of new character line.
	}

    return (EXIT_SUCCESS);
}

// open file and read board

// store board -- 2D array

//print out move
//		form X#-X#

// accept move input
