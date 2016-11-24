#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "alphabeta.h"

static void **(*get_actions)(void *, int *count);
static bool (*cutoff_test)(void *);
static float (*evaluation)(void *);
static void *(*result)(void *, void *action);
static void *(*get_move)(void *, float);

void set_actions_function(void **(*fn)(void *, int *count))
{
	get_actions = fn;
}

void set_cutoff_test_function(bool (*fn)(void *))
{
	cutoff_test = fn;
}

void set_evaluation_function(float (*fn)(void *))
{
	evaluation = fn;
}

void set_result_function(void *(*fn)(void *, void *action))
{
	result = fn;
}

void set_get_move_function(void *(*fn)(void *, float))
{
	get_move = fn;
}


void *alpha_beta_search(void *state){
	float v;
	v = max_value(state, -INFINITY, INFINITY);
	return get_move(state, v);
}

float max_value(void *state, float alpha, float beta){
	float v;
	if (cutoff_test(state))
		return evaluation(state);
	v = -INFINITY;

	int count = 0;
	void **actions = get_actions(state, &count);
	for (int i=0; i<count; i++) {
		v = fmax(v, min_value(result(state,actions[i]), alpha, beta));
		if (v>=beta)
			return v;
		alpha = fmax(alpha,v);
	}
	return v;
}

float min_value(void *state, float alpha, float beta){
	float v;
	if (cutoff_test(state))
		return evaluation(state);
	v = INFINITY;
	int count=0;
	void **actions = get_actions(state, &count);
	for (int i=0; i<count; i++) {
		v = fmin(v, max_value(result(state,actions[i]), alpha, beta));
		if (v<=alpha)
			return v;
		beta = fmin(beta,v);
	}
	return v;
}
