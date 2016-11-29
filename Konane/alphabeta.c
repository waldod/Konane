#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "alphabeta.h"

static float (*eval)(struct tree_node *);

static int starting_depth;

void set_eval_function(float (*fn)(struct tree_node *))
{
    eval = fn;
}

static bool cutoff_test(struct tree_node *node, int max_depth)
{
   return (node->depth - starting_depth) >= max_depth || node->child_count == 0;  
}

static struct tree_node *get_move(struct tree_node *root, float v)
{
    int count = 0;
    struct tree_node **children = get_tree_children(root, &count);
    struct tree_node *ret = NULL;
    
    for (int i=0; i<count; i++) {
        if (children[i]->temp_val == v) {
            ret = children[i];
            break;
        }
            
    }

    if (ret == NULL) {
        fprintf(stderr, "ERROR: Alphabeta failed to decide a move.\n"); 
        exit(1);
    }

    free(children);
    return ret;
}

struct tree_node *alpha_beta_search(struct tree_node *state, int max_depth){
	float v;
    starting_depth = state->depth;
	v = max_value(state, -INFINITY, INFINITY, max_depth);
	return get_move(state, v);
}

float max_value(struct tree_node *state, float alpha, float beta, int max_depth){
	float v;
	if (cutoff_test(state, max_depth))
        v = eval(state);
    else {
        v = -INFINITY;

        int count = 0;
        struct tree_node **children = get_tree_children(state, &count);
        for (int i=0; i<count; i++) {
            v = fmax(v, min_value(children[i], alpha, beta, max_depth));
            if (v>=beta)
                return v;
            alpha = fmax(alpha,v);
        }
        free(children);
    }
    state->temp_val = v;
	return v;
}

float min_value(struct tree_node *state, float alpha, float beta, int max_depth){
	float v;
	if (cutoff_test(state, max_depth))
        v = eval(state);
    else {
        v = INFINITY;
        int count=0;
        struct tree_node **children = get_tree_children(state, &count);
        for (int i=0; i<count; i++) {
            v = fmin(v, max_value(children[i], alpha, beta, max_depth));
            if (v<=alpha)
                return v;
            beta = fmin(beta,v);
        }
        free(children);
    }
    state->temp_val = v;
	return v;
}
