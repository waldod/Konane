#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "alphabeta.h"

char alpha_beta_search(void *state){
	int v;
	v = max_value(state, -INFINITY, INFINITY);
	return get_move(state, v);
}

int max_value(void *state, int alpha, int beta){
	int v;
	if (cutoff_test(state))
		return evaluation(state);
	v = -INFINITY;
	foreach(a,actions(state)){
		v = fmax(v, min_value(result(state,a), alpha, beta));
		if (v>=beta)
			return v;
		alpha = fmax(alpha,v);
	}
	return v;
}

int min_value(void *state, int alpha, int beta){
	int v;
	if (cutoff_test(state))
		return evaluation(state);
	v = INFINITY;
	foreach(a,actions(state)){
		v = fmin(v, max_value(result(state,a), alpha, beta));
		if (v<=alpha)
			return v;
		beta = fmin(beta,v);
	}
	return v;
}

/*
 * function ACTIONS(state) returns the set of moves that can be taken
 */
char actions(void *state){
	char moves;
	return moves;
}

/*
 * function CUTOFF-TEST(state) returns EVALUATION(state) 
 */
int cutoff_test(void *state){
	return evaluation(state);
}

/*
 * function EVALUATION(state) returns the evaluation value
 */
int evaluation(void *state){
	int value;
	return value;
}

/*
 * function RESULT(state,a) returns the result of move a on the state
 */
char result(void *state, char a){
	char move;
	return move;
}
  
/* 
 * function GET-MOVE(state,v) returns the move from the state with value v
 */
char get_move(void *state, int v){
	char move;
	return move;
}