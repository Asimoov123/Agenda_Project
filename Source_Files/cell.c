#include "cell.h"
#include <stdlib.h>


t_d_cell *createCell(int val, int lvl) {
    t_d_cell *newCell = malloc(sizeof(t_d_cell));
    newCell->value = val;
    newCell->level = lvl + 1;
    newCell->next = (struct s_d_cell **) malloc(sizeof(struct s_d_cell *) * newCell->level);

    for (int i = 0; i < newCell->level; i++) {
        newCell->next[i] = NULL;
    }

    return newCell;
}