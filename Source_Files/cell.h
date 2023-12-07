#ifndef AGENDA_PROJECT_CELL_H
#define AGENDA_PROJECT_CELL_H


typedef struct s_d_cell {
    int value;
    int level;
    struct s_d_cell **next;
} t_d_cell;

t_d_cell *createCell(int val, int lvl);

#endif //AGENDA_PROJECT_CELL_H
