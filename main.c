#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "list.h"
#include "timer.h"
#include "agenda.h"

/*

void compareExecutionTime(int nbOfSearches){
    srand( time( NULL ) );

    t_d_list mylist;
    for (int n = 7; n<16; n++){
        int *levels = malloc(sizeof(int)*(pow(2, n)-1));
        for (int i=0; i<pow(2, n)-1; i++){
            levels[i] = 0;
        }

        for (int i=2; i<=pow(2,n); i*=2){
            for (int j=i-1; j<pow(2,n)-1; j+=i){
                levels[j]++;
            }
        }

        mylist = createList(n);

        for (int i=0; i<pow(2,n)-1; i++){
            add_Increasing_Order(&mylist, i+1, levels[i]);
        }

        printf("%-2d ", n);

        int searchedValue;
        startTimer();
        for (int k=0; k<nbOfSearches ; k++){
            searchedValue = rand() % (int)pow(2,n);
            isValueInListLineaire(mylist, searchedValue);
        }
        stopTimer();
        displayTime();
        startTimer();
        for (int k=0; k<nbOfSearches ; k++){
            searchedValue = rand() % (int)pow(2,n);
            isValueInListMultiniveaux(mylist, searchedValue);
        }
        stopTimer();
        displayTime();
        printf("\n");
    }
}
*/

void autocompletion(char *str, t_d_ContactList mylist) {
    t_d_contact *current = mylist.heads[0];
    int found = 0;
    if (strlen(str) >= 3) {
        while (current != NULL) {
            if (strncmp(str, current->nom, strlen(str)) == 0) {
                if (found == 0) {
                    printf("Contacts found : ");
                    found = 1;
                }

                printf("%s, ", current->nom);
            }
            current = current->next[0];


        }
    }
}

char *scanString1(t_d_ContactList mylist) {
    char *p = NULL;
    char character;
    int index = 0;
    int size = 1;
    p = (char *) malloc(sizeof(char));

    while (1) {
        character = (char) getchar(); // Permet d'attendre la frappe d'un caractère au clavier

        if (character == '\n') { // Vérifie si le charactere est le dernier saisi par l'utilisateur
            p[index] = '\0';  // Termine la chaîne
            break;
        }
        if (character == '\t') {
            p[index] = '\0';
            autocompletion(p, mylist);

        }
        if (index >= size - 1) { // Realloue de la memoire si necessaire
            size *= 2;
            char *temp = (char *) realloc(p, size * sizeof(char));
            if (temp == NULL) {
                free(p);
                p = NULL;
                printf("ERROR: Failed to reallocate memory!\n");
                exit(1);
            }
            p = temp;
        }

        p[index] = character;
        index++;
    }

    return p;
}

int main() {
    t_d_ContactList mylist = createContactList();
    char *buff = malloc(sizeof(char) * 128);
    int run = 1;

    insertContact(&mylist, "flamel");
    insertContact(&mylist, "gallouin");
    insertContact(&mylist, "gartner");
    insertContact(&mylist, "guetta");

    while (run) {
        printf("1. Create Contact\n2. Delete Contact\n3. Display Contact List\n4. Search Contact\n5. Afficher RDV\n6. Exit\n> ");
        if (fgets(buff, 128, stdin) != NULL) {
            if (strcmp(buff, "1\n") == 0) {
                insertContact(&mylist, Scan_name());
            } else if (strcmp(buff, "2\n") == 0) {
                delete_Contact(&mylist, Scan_name());
            } else if (strcmp(buff, "3\n") == 0) {
                display_all_levels_Contact_aligned(mylist);
            } else if (strcmp(buff, "4\n") == 0) {
                searchContact(mylist, Scan_name());
            } else if (strcmp(buff, "5\n") == 0) {
                display_all_rendez_vous(mylist, Scan_name());
            } else if (strcmp(buff, "6\n") == 0) {
                run = 0;
            } else if (strcmp(buff, "\n") != 0) {
                printf("Unknown Function\n");
            }
        } else {
            printf("Input Error\n");
        }
        printf("\n");

    }

    return 0;
}
