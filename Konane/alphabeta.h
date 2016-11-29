#ifndef ALPHABETA_H
#define ALPHABETA_H

#include <stdbool.h>
#include "tree.h"

struct tree_node *alpha_beta_search(struct tree_node *state, int max_depth);
float max_value(struct tree_node *state, float alpha, float beta, int max_depth);
float min_value(struct tree_node *state, float alpha, float beta, int max_depth);

void set_eval_function(float (*fn)(struct tree_node *));

#endif /* ALPHABETA_H */
