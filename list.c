#include "list.h"
#include "cell.h"
#include <stdio.h>
#include <stdlib.h>


t_d_list createList(int lvl) {
    t_d_list mylist;
    mylist.max_level = lvl + 1;
    mylist.heads = (t_d_cell **) malloc(sizeof(t_d_cell *) * mylist.max_level);

    for (int i = 0; i < mylist.max_level; i++) {
        mylist.heads[i] = NULL;
    }
    return mylist;
}

void addHead(t_d_list *mylist, int val, int lvl) {
    // Same as :
    // if (lvl > mylist->max_level-1)
    //      lvl = mylist->max_level-1;
    lvl = (lvl > mylist->max_level - 1) ? mylist->max_level - 1 : lvl;
    t_d_cell *newCell = createCell(val, lvl);
    for (int i = 0; i < newCell->level; i++) {
        newCell->next[i] = mylist->heads[i];
    }
    for (int i = 0; i < newCell->level; i++) {
        mylist->heads[i] = newCell;
    }
}


void display_level(t_d_list mylist, int lvl) {
    printf("[list head_%d @-]-->", lvl);
    t_d_cell *currentCell = mylist.heads[lvl];
    while (currentCell != NULL) { // for each cell on the given level
        printf("[ %d|@-]-->", currentCell->value); // display the cell with its value
        currentCell = currentCell->next[lvl]; // move through the list
    }
    printf("NULL\n");
}

void display_all_levels(t_d_list mylist) {
    for (int i = 0; i < mylist.max_level; i++) {
        display_level(mylist, i);
    }
}

int numPlaces(int n) {
    /* Count the number of character of an integer
    Function based on https://stackoverflow.com/questions/1068849/how-do-i-determine-the-number-of-digits-of-an-integer-in-c
    We use raw speed instead of iterative or recursive function for performance :
    Raw speed with 0:            0 seconds
    Raw speed with 2^31-1:       1 second
    Iterative with 2^31-1:       5 seconds
    Recursive with 2^31-1:       6 seconds
    Floating point with 1:       6 seconds
    Floating point with 2^31-1:  7 seconds */
    int r = 0;
    if (n < 0) {
        n = (n == INT_MIN) ? INT_MAX : -n;
        r++;
    }
    if (n < 10) r += 1;
    else if (n < 100) r += 2;
    else if (n < 1000) r += 3;
    else if (n < 10000) r += 4;
    else if (n < 100000) r += 5;
    else if (n < 1000000) r += 6;
    else if (n < 10000000) r += 7;
    else if (n < 100000000) r += 8;
    else if (n < 1000000000) r += 9;

    return r;
}

void display_level_aligned(t_d_list mylist, int lvl) {

    printf("[list head_%d @-]--", lvl);

    t_d_cell *current0Cell = mylist.heads[0]; // temporary cell
    t_d_cell *currentLvlCell = mylist.heads[lvl]; // temporary cell

    while (current0Cell != NULL) {
        if (current0Cell !=
            currentLvlCell) { // If the cell on the first line is different from the one on the line we want to display
            while (current0Cell != currentLvlCell) { // while it is true, display '-' instead of cell
                printf("---%.*s------", numPlaces(current0Cell->value),
                       "----------"); // https://stackoverflow.com/questions/14678948/how-to-repeat-a-char-using-printf
                current0Cell = current0Cell->next[0]; // temporary cell is set to its successor (on the first line)
            }
        }
        if (currentLvlCell == NULL) break;
        printf(">[ %d|@-]--", currentLvlCell->value); // display the cell
        current0Cell = current0Cell->next[0]; // the cell on the first line is set to its successor
        currentLvlCell = currentLvlCell->next[lvl]; // the cell on level we want to display is set to its successor
    }
    printf(">NULL\n");
}


void display_all_levels_aligned(t_d_list mylist) {
    for (int i = 0; i < mylist.max_level; i++) {
        display_level_aligned(mylist, i);
    }
}


void add_Increasing_Order(t_d_list *mylist, int val, int lvl) {
    if (mylist->heads[0] == NULL) {
        addHead(mylist, val, lvl);
        return;
    }
    if (mylist->heads[0]->value > val) { // addHead if the value we want to insert is smaller than the first element
        addHead(mylist, val, lvl);
    } else {
        lvl = (lvl > mylist->max_level - 1) ? mylist->max_level - 1 : lvl;

        t_d_cell *newCell = createCell(val, lvl); // create new cell
        t_d_cell *current; // temporary cell to browse the list
        t_d_cell *update[mylist->max_level]; // list of pointers initialized at NULL
        for (int i = 0; i < mylist->max_level; i++) {
            update[i] = NULL;
        }

        for (int i = newCell->level - 1; i >= 0; i--) { // going from the highest lvl of the new cell to the first level
            current = mylist->heads[i]; // set current to head
            if (current == NULL) {
                newCell->next[i] = NULL;
                mylist->heads[i] = newCell;

            } else {
                while (current->next[i] != NULL && current->value <
                                                   newCell->value) { // while current cell's value is smaller than the new cell's value
                    update[i] = current; // save the current cell
                    current = current->next[i]; // set current cell to its successor (on the same line)
                }

                // once the current cell is at the right position
                if (current->next[i] == NULL && current->value <
                                                newCell->value) // it the successor is null (meaning the value is higher than every value in the list)
                    update[i] = current; // save the current cell

                // insert the new cell right after the current cell
                newCell->next[i] = update[i]->next[i];
                update[i]->next[i] = newCell;
            }
        }
    }
}

int isValueInListLineaire(t_d_list mylist, int val) {
    t_d_cell *tempCell = mylist.heads[0];
    if (tempCell != NULL) {
        while (tempCell->next[0] != NULL) {
            if (tempCell->value == val) {
                return 1;
            }
            tempCell = tempCell->next[0];
        }
    }
    return 0;
}


int isValueInListMultiniveaux(t_d_list mylist, int val) {
    if (mylist.heads[0] == NULL) {
        return 0;
    }
    if (mylist.heads[0]->value == val) { // addHead if the value we want to insert is smaller than the first element
        return 1;
    } else {
        int current_lvl = mylist.max_level - 1;
        t_d_cell *current = mylist.heads[current_lvl]; // temporary cell to browse the list
        while (current_lvl > 0 && current != NULL) {
            while (current->next[current_lvl] != NULL &&
                   current->value < val) { // while current cell's value is smaller than the new cell's value
                current = current->next[current_lvl]; // set current cell to its successor (on the same line)
            }
            if (current->value == val) {
                return 1;
            }
            current_lvl--;
        }
        return 0;
    }
}

int isValueInListLvl1(t_d_list mylist, int val) {
    if (mylist.heads[0] == NULL) {
        return 0;
    }
    if (mylist.heads[0]->value == val) { // addHead if the value we want to insert is smaller than the first element
        return 1;
    } else {
        t_d_cell *current; // temporary cell to browse the list

        for (int i = mylist.max_level - 1;
             i >= 0; i--) { // going from the highest lvl of the new cell to the first level
            current = mylist.heads[i]; // set current to head
            if (current != NULL) {
                while (current->next[i] != NULL &&
                       current->value < val) { // while current cell's value is smaller than the new cell's value
                    current = current->next[i]; // set current cell to its successor (on the same line)
                }
                if (current->value == val) {
                    return 1;
                }
            }
        }
        return 0;
    }
}