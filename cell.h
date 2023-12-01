#ifndef AGENDA_PROJECT_CELL_H
#define AGENDA_PROJECT_CELL_H


typedef struct s_d_cell{
    int value;
    int level;
    struct s_d_cell **next;
} t_d_cell;

/*
typedef struct s_d_rdvCell {
    t_d_rdv value;
    int level;
    struct s_d_rdvCell **next;
} t_d_rdvCell;
*/

t_d_cell *createCell(int val, int lvl);
//t_d_rdvCell *createRdvCell(t_d_rdv rdv, int lvl);

#endif //AGENDA_PROJECT_CELL_H
