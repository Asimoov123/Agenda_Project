#ifndef AGENDA_PROJECT_LIST_H
#define AGENDA_PROJECT_LIST_H

#include "cell.h"

typedef struct s_d_list {
    t_d_cell **heads;
    int max_level;
} t_d_list;


t_d_list createList(int lvl);

void addHead(t_d_list *, int, int);

void display_level(t_d_list, int);

void display_all_levels(t_d_list);

void display_level_aligned(t_d_list, int);

void display_all_levels_aligned(t_d_list); // En option : afficher en alignant les cellules

void add_Increasing_Order(t_d_list *, int, int); // En option

// Partie 2
int isValueInListLinear(t_d_list, int);

int isValueInListMultiLvl(t_d_list, int);


#endif //AGENDA_PROJECT_LIST_H
