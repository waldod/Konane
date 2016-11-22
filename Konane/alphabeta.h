#ifndef ALPHABETA_H
#define ALPHABETA_H


#define foreach(item, list) \
    for(T * item = list->head; item != NULL; item = item->next)

char alpha_beta_search(void *state);
int max_value(void *state, int alpha, int beta);
int min_value(void *state, int alpha, int beta);
char actions(void *state);
int cutoff_test(void *state);
int evaluation(void *state);
char result(void *state, char a);
char get_move(void *state, int v);

#endif /* ALPHABETA_H */