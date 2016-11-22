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
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "tree.h"

#define BOARD_SIZE 8

// Board state struct (a struct rather than 2d array to make it easier to
// pass around to different functions.
struct state {
	char board[BOARD_SIZE][BOARD_SIZE];
	char *action; // action to get to this move (use in get_moves())
	int eval_value;
};

static struct tree_node *tree_root;
static char player;
static char turn = 'B';

static bool tree_change = false; // Tree has been changed and thus tree creation plugin should change.

static bool is_running = true;

/**
 * print_board() - prints a board
 * @state - state to print
 */
static void print_board(struct state *state) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			// don't check if null character because board should be right */
			fprintf(stdout, "%c", state->board[i][j]);
		}
		// get rid of new character line at end of line. */
		fprintf(stdout, "\n");
	}
}


/**
 * print_usage() - prints usage of application
 */
void print_usage(char *exec_name)
{
	fprintf(stdout, "Usage: %s file player\n", exec_name);
	fprintf(stdout, "file - filename of board\n");
}

/**
 * update_state() - apply a move
 * @move - move to make
 */
static void update_state(char *move)
{
	int child_count = 0;
	struct tree_node **children_of_root = get_tree_children(tree_root, &child_count);

	for (int i = 0; i < child_count; i++) {
		struct state *state = children_of_root[i]->value;
		if (strcmp(state->action, move) == 0) {
			tree_root = children_of_root[i];
			tree_change = true;	
			return;
		}
	}

	fprintf(stderr, "ERROR: Failed to find new state in tree after move.\n");
	exit(1);
}

/**
 * create_state() - creates a new board state object
 * @parent - parent to inherit board state from
 * @action - action to get a change.
 *
 * Returns a new state
 */
static struct state *create_state(struct state *parent, char *action)
{
	struct state *state = malloc(sizeof(struct state));

	if (state == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failure.\n");
		exit(1);
	}

	size_t size_of_board = BOARD_SIZE * BOARD_SIZE;

	if (parent != NULL) {
		memcpy(state->board, parent->board, size_of_board);
	}

	state->action = action;

	return state;
}

/**
 * get_moves() - returns a list of avaiable moves from given state
 * @current - current state
 * @count - amount of moves
 */
struct state **get_moves (struct state *current, int *count)
{
	//TODO
	struct state **states;

	return states;
}



static void free_board_state(struct state *state)
{
	free(state);
}

static char *decide_move()
{
	// TODO
	char *move;

	return move;
}
//---------------------------------------------------------
// State tree creation thread
//---------------------------------------------------------

static void _free_board_state(void *state)
{
	free_board_state(state);
}

static void _tree_node_eval(struct tree_node *node)
{
	if (tree_change)
		return;

	if (node->child_count == 0) {
		int count = 0;
		struct state **states = get_moves(node->value, &count);
		for (int i = 0; i < count; i++) {
			add_tree_child(node, states[i]);
		}
	}
}

static void *make_tree()
{
	// TODO: Exit this thread cleanly.
	while (is_running) {
		struct tree_node *current = tree_root;
		breadth_traverse(current, -1, _tree_node_eval);	

		if (current != tree_root) {
			recursive_free_tree(current, _free_board_state, tree_root);
		}

		tree_change = true;
	}
	pthread_exit(NULL);
}

//---------------------------------------------------------

//---------------------------------------------------------
// Game handle thread 
//---------------------------------------------------------

static void *game_handler()
{
	while (is_running) {
		if (player == turn) {
			// IMPORTANT New line at end of move?
			char *move = decide_move();
			fprintf(stdout, "%s\n", move);
			update_state(move);
		} else {
			char move[6];
			fscanf(stdin, "%5s", move);
			update_state(move);
		}
	}
	pthread_exit(NULL);
}


//---------------------------------------------------------


void setup_game(int argc, char *argv[]) {
	// "board.txt", "B"
	// first arg is name of .txt file
	// second arg is B or W
	if (argc != 3) {
		fprintf(stderr, "ERROR: Invalid number of arguments provided\n");
		print_usage(argv[0]);
		exit(1);
	}

	char *filename = argv[1];

	FILE *fp;

	fp = fopen(filename, "r");

	// error check.
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Failed to open file %s", argv[0]);
		exit(1);
	}

	player = *argv[2];

	struct state *initial_state = create_state(NULL, NULL);

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			// don't check if null character because board should be right
			initial_state->board[i][j] = (char)fgetc(fp);
		}
		// get rid of new character line at end of line.
		fgetc(fp);
	}

	tree_root = init_tree(initial_state);	
}

int main(int argc, char* argv[]) {
	setup_game(argc, argv);

	pthread_t handler;
	pthread_t tree_creation;

	if (pthread_create(&handler, NULL, game_handler, NULL) != 0)
		fprintf(stderr, "ERROR: Failed to create game handler thread.\n");

	if (pthread_create(&tree_creation, NULL, make_tree, NULL) != 0)
		fprintf(stderr, "ERROR: Failed to create game handler thread.\n");

	
	pthread_exit(NULL);

	return 0;
}
