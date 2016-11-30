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
#include <ctype.h>

#include "tree.h"
#include "alphabeta.h"

// Time to make a move
#define TIME_IN_SECONDS 10

#define BOARD_SIZE 8


// Board state struct (a struct rather than 2d array to make it easier to
// pass around to different functions.
struct state {
    char board[BOARD_SIZE][BOARD_SIZE];
    char *action; // action to get to this move (use in get_moves())
    char player;
    int eval_value;
};

static struct tree_node *tree_root;
static bool tree_change = false; // Tree has been changed and thus tree creation should be updated
static bool is_running = true;
static char our_player;
static int max_depth = 0;

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
    fprintf(stdout, "\n");
}

static char switch_player(char player)
{
    return player == 'B' ? 'W' : 'B';
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
        state->player = switch_player(parent->player);
    } else {
        state->player = 'B';
    }

    state->action = action;

    return state;
}


/*
 * count_o() - return the number of "O" spaces on current board
 * used to determine minimum number of memory to allocate get_moves()
 */
int count_o (struct state *current)
{
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // don't check if null character because board should be right */
            if ('O' == current->board[i][j])
                count++;
        }
    }

    return count;
}

int translate_letter(char letter)
{
    return letter - 'A';
}

void update_board(struct state *change)
{

    char *move = change->action;
    char prev, *new;

    // location of initial square
    int j = translate_letter(move[0]); // letter is column
    int i = 8 - (strtol(&move[1], &new, 10)); // number is row.

    // adjust board
    prev = (change->board[i][j]);
    change->board[i][j] = 'O';

    if (move[2] == '-') {
        // location of square to move to
        int p = translate_letter(move[3]); // letter is column
        int q = 8 - (strtol(&move[4], &new, 10)); // number is row.

        // TODO :check if move is valid

        // change middle square being hopped over
        if ( i == q) {
            if (j > p) {
                change->board[i][j-1] = 'O';
				if ((j - p) > 2) {
					change->board[i][j-3] = 'O';
				}
            } else if (j < p) {
                change->board[i][j+1] = 'O';
				if ((p - j) > 2) {
					change->board[i][j+3] = 'O';
				}
            }
        }
        if ( j == p) {
            if (i > q) {
                change->board[i-1][j] = 'O';
				if ((i - q) > 2) {
					change->board[i-3][j] = 'O';
				}
            } else if (i < q) {
                change->board[i+1][j] = 'O';
				if ((q - i) > 2) {
					change->board[i+3][j] = 'O';
				}
            }
        }
        change->board[q][p] = prev;
    }
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
            max_depth = max_depth - tree_root->depth;
            return;
        }
    }

    // Have not found move.
    struct state *state = create_state(tree_root->value, move);
    update_board(state);
    tree_root = init_tree(state);
    max_depth = 0;
    tree_change = true;

}


char translate_number(int num)
{
    return 'A' + num;
}

char *get_space(int row, int col)
{
    char *move;

    move = calloc(14, sizeof(char));

    move[0] = translate_number(col);
    //if (move[0] == 'Z') {
    //	fprintf(stderr, "ERROR: couldn't match number index to letter");

    int i = 8 - row;
    move[1] = (char)(i + '0');
    return move;
}
/* get the move that can be performed given the current board and empty space
 * return as string LetterNumber-LetterNumber
 * which is the move
 */
char *gen_move(struct state *original, char *open, int row, int col,  const char *direction, char player)
{
    // check which color is above/below/right/left of it
    char *move;
    int indx;
    move = calloc(14, sizeof(char));
    if ((strcmp(direction, "up") == 0) && (row != 0)) {
        if ((original->board[row-1][col] == 'O') || (original->board[row-1][col] == player)) {
            /* if the piece beside the open spot is the player
             * then the player cannot move into the open spot
             */
            free(move);
            return NULL;
        } else if ((row != 1) && (original->board[row-2][col] == player)) {
            indx = row - 2;
            move = get_space(indx, col);
            move[2] = '-';
            move[3] = '\0';
            strncat(move, open, 3);
        } else if ((row != 1) && (original->board[row-2][col] == 'O')) {
			if ((row != 3) && (original->board[row-3][col] == switch_player(player))) {
				if (original->board[row-4][col] == player) {
					indx = row - 4;
					move = get_space(indx, col);
					move[2] = '-';
					move[3] = '\0';
					strncat(move, open, 3);
				}
			}
		}
    } else if ((strcmp(direction, "down") == 0) && (row != 7)) {
        if ((original->board[row+1][col] == 'O') || (original->board[row+1][col] == player)) {
            free(move);
            return NULL;
        } else if ((row != 6) && (original->board[row+2][col] == player)) {
            indx = row + 2;
            move = get_space(indx, col);
            move[2] = '-';
            move[3] = '\0';
            strncat(move, open, 2);
        } else if ((row != 6) && (original->board[row+2][col] == 'O')) {
			if ((row != 4) && (original->board[row+3][col] == switch_player(player))) {
				if (original->board[row+4][col] == player) {
					indx = row + 4;
					move = get_space(indx, col);
					move[2] = '-';
					move[3] = '\0';
					strncat(move, open, 3);
				}
			}
		}
    } else if ((strcmp(direction, "left") == 0) && (col != 0)) {
        if ((original->board[row][col-1] == 'O') || (original->board[row][col-1] == player)) {
            free(move);
            return NULL;
        } else if ((col != 1) && (original->board[row][col-2] == player)) {
            indx = col - 2;
            move = get_space(row, indx);
            move[2] = '-';
            move[3] = '\0';
            strncat(move, open, 2);
        } else if ((col != 1) && (original->board[row][col-2] == 'O')) {
			if ((col != 3) && (original->board[row][col-3] == switch_player(player))) {
				if (original->board[row][col-4] == player) {
					indx = col - 4;
					move = get_space(row, indx);
					move[2] = '-';
					move[3] = '\0';
					strncat(move, open, 3);
				}
			}
		}
    } else if ((strcmp(direction, "right") == 0) && (col != 7)) {
        if ((original->board[row][col+1] == 'O') || (original->board[row][col+1] == player)) {
            free(move);
            return NULL;
        } else if ((col != 6) && (original->board[row][col+2] == player)) {
            indx = col + 2;
            move = get_space(row, indx);
            move[2] = '-';
            move[3] = '\0';
            strncat(move, open, 2);
        } else if ((col != 6) && (original->board[row][col+2] == 'O')) {
			if ((col != 4) && (original->board[row][col+3] == switch_player(player))) {
				if (original->board[row][col+4] == player) {
					indx = col + 4;
					move = get_space(row, indx);
					move[2] = '-';
					move[3] = '\0';
					strncat(move, open, 3);
				}
			}
		}
    }
    if (!isdigit(move[1])) {
        free(move);
        return NULL;
    }
    if (move[0] == 'Z') {
        free(move);
        return NULL;
    }
    if (move[1] == '0') {
        free(move);
        return NULL;
    }
    return move;
}

/**
 * get_moves() - returns a list of avaiable moves from given state
 * @current - current state
 * @count - amount of moves
 */
struct state **get_moves (struct state *current, int *count, char player)
{

    struct state **states;
    int tmpcount = 0;
    int empty = count_o (current);

    if (current == NULL) {
        fprintf(stderr, "ERROR: No board to get moves from.\n");
        exit(1);
    }

    // initial states are  exceptions
    if ((empty == 0) && (player == 'B')) {
        states = malloc(sizeof(struct state *)*2);
        if (states == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failure.\n");
            exit(1);
        }
        struct state *state1 = create_state(current, "E4");
        update_board(state1);
        tmpcount++;
        struct state *state2 = create_state(current, "D5");
        update_board(state2);
        states[0] = state1;
        states[1] = state2;
        *count = 2;
        return states;
    } else if (empty == 1) {
        // we must be white
        states = malloc(sizeof(struct state *)*2);
        if (states == NULL) {
            fprintf(stderr, "ERROR: Memory allocation failure.\n");
            exit(1);
        }
        struct state *state1 = create_state(current, "D4");
        update_board(state1);
        tmpcount++;
        struct state *state2 = create_state(current, "E5");
        update_board(state2);
        states[0] = state1;
        states[1] = state2;
        *count = 2;
        return states;
    }
    const char *direction[] = {"up", "down", "left", "right"};
    int counter = 0;
    if (empty > 1) {
        states = malloc(sizeof(struct state *)* (empty*4));
        // allocate space, each empty spot has a possiblity of producing a max of 4 moves
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (current->board[i][j] == 'O') {
                    char *open = get_space(i, j);
                    for (int k = 0; k < 4; k++) {
                        char *tomove = gen_move(current, open, i, j, direction[k], player);
                        if (tomove != NULL) {
                            if (counter > (empty*4)) {
                                states = realloc(states, sizeof(struct state *)*counter);
                            }
                            struct state *possible = create_state(current, tomove);
                            update_board(possible);
                            states[counter] = possible;
                            counter++;
                        }
                    }
                    free(open);
                }
            }
        }
    }
    *count = counter;
    return states;
}


void check_moves(struct state *current)
{
    int count = 0;
    free(get_moves(current, &count, switch_player(current->player)));
    if (count == 0) {
        if ((switch_player(current->player) == our_player)) {
            // because if our player, then we have no moves to perform.
            fprintf(stdout, "Opponent wins! \n");
            is_running = false;
        } else {
            fprintf(stdout, "Player wins! \n");
            is_running = false;
        }
    }
}

static void free_board_state(struct state *state)
{
    free(state);
}

//---------------------------------------------------------
// Alphabeta stuff
//---------------------------------------------------------

float eval_fn(struct tree_node *cur_node)
{
    struct state *state = cur_node->value;
    return (float) state->eval_value;
}


static char *decide_move()
{
    int max_evaluated = 0;
    struct tree_node *take = NULL;

    time_t stop_time = time(NULL) + TIME_IN_SECONDS;

    while (time(NULL) < stop_time) {
        if (max_depth > max_evaluated) {
            struct tree_node *ret = alpha_beta_search(tree_root, ++max_evaluated, stop_time);
            if (ret != NULL)
                take = ret;
        }
    }

    if (take == NULL) {
        fprintf(stderr, "ERROR: failed to decide a move.\n");
        exit(1);
    }

    struct state *state = take->value;
    return state->action;
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
        struct state *state = node->value;
        struct state **states = get_moves(state, &count, switch_player(state->player));
        for (int i = 0; i < count; i++) {
            int our_count = 0;
            free(get_moves(states[i], &our_count, our_player));
            states[i]->eval_value = our_count;
            struct tree_node *new_node = add_tree_child(node, states[i]);
            max_depth = new_node->depth - tree_root->depth;
        }
        free(states);
    }
}

struct free_tree_args {
    struct tree_node *root;
    struct tree_node *ignore;
};

static void *thread_free_tree(void *arg)
{
    struct free_tree_args *args = arg;
    recursive_free_tree(args->root, _free_board_state, args->ignore);
    free(args);
    pthread_exit(NULL);
}


static void *make_tree()
{
    while (is_running) {
        struct tree_node *current = tree_root;
        breadth_traverse(current, -1, _tree_node_eval);
        if (current != tree_root) {
            pthread_t free_thread;
            struct free_tree_args *args = malloc(sizeof(struct free_tree_args));
            args->root = current;
            args->ignore = tree_root;
            if (args == NULL) {
                fprintf(stderr, "ERROR: Memory allocation failure!\n");
                exit(1);
            }
            if (pthread_create(&free_thread, NULL, thread_free_tree, args) != 0)
                fprintf(stderr, "ERROR: Failed to create a thread.\n");

        }
        tree_change = false;

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
        struct state *state = tree_root->value;
        check_moves(state);
        if (!is_running)
            break;
        if (switch_player(state->player) == our_player) {
            char *move = decide_move();
            fprintf(stdout, "%s\n", move);
			print_board(state);
            update_state(move);

        } else {
            char move[6];
            fgets(move, 6, stdin);
            update_state(move);
			print_board(state);
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

    our_player = *argv[2];

    struct state *initial_state = create_state(NULL, NULL);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // don't check if null character because board should be right
            initial_state->board[i][j] = (char)fgetc(fp);
        }
        // get rid of new character line at end of line.
        fgetc(fp);
    }

    initial_state->player = 'W';
    tree_root = init_tree(initial_state);
}

int main(int argc, char* argv[]) {
    setup_game(argc, argv);
    set_eval_function(eval_fn);

    pthread_t handler;
    pthread_t tree_creation;

    if (pthread_create(&handler, NULL, game_handler, NULL) != 0)
        fprintf(stderr, "ERROR: Failed to create game handler thread.\n");
    if (pthread_create(&tree_creation, NULL, make_tree, NULL) != 0)
        fprintf(stderr, "ERROR: Failed to create game handler thread.\n");

    pthread_exit(NULL);

    return 0;
}
