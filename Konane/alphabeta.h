#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stdbool.h>

void *alpha_beta_search(void *state);
float max_value(void *state, float alpha, float beta);
float min_value(void *state, float alpha, float beta);

void set_actions_function(void **(*fn)(void *, int *count));
void set_cutoff_test_function(bool (*fn)(void *));
void set_evaluation_function(float (*fn)(void *));
void set_result_function(void *(*fn)(void *, void *action));
void set_get_move_function(void *(*fn)(void *, float));

#endif /* ALPHABETA_H */
