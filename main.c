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


int main1() {

    t_d_contact *test1, *test2, *test3, *test4, *test5, *test6, *test7;
    test1 = createContact(Scan_name());
    test2 = createContact(Scan_name());
    test3 = createContact(Scan_name());
    test4 = createContact(Scan_name()); // Création d'une liste à niveau de 7 contact
    test5 = createContact(Scan_name());
    test6 = createContact(Scan_name());
    test7 = createContact(Scan_name());
    t_d_ContactList  mylist = createContactList();

    insertContact(&mylist,test1);
    insertContact(&mylist,test2);
    insertContact(&mylist,test3);
    insertContact(&mylist,test4);
    insertContact(&mylist,test5); // Insertion dans la liste
    insertContact(&mylist,test6);
    insertContact(&mylist,test7);
    display_all_levels_Contact_aligned(mylist); // Premier affichage

    delete_Contact(&mylist,test3); // Supression d'un élément (ici le 3ème)

    display_all_levels_Contact_aligned(mylist); // Deuxième affichage

    return 0;
}

int main() {
    t_d_contact *testContact;
    t_d_ContactList mylist = createContactList();
    char* buff = malloc(sizeof(char)*128);
    int run = 1;
    char* txt = NULL;

    while(run){
        printf("1. Create Contact\n2. Delete Contact\n3. Display contact list\n4. SearchContact\n5. Afficher RDV\n6. Exit\n>");
        if (fgets(buff, 128, stdin)!=NULL){
            if (strcmp(buff, "1\n") == 0) {
                testContact = createContact(Scan_name());
                insertContact(&mylist, testContact);
            } else if (strcmp(buff, "2\n") == 0){
                delete_Contact(&mylist,testContact); // Supression d'un élément (ici le 3ème)
            } else if (strcmp(buff, "3\n") == 0) {
                display_all_levels_Contact_aligned(mylist); // Premier affichage
            } else if (strcmp(buff, "4\n") == 0) {
                printf("blbblb");
            } else if (strcmp(buff, "5\n") == 0){
                printf("%s :\n", mylist.heads[0]->nom);
                printf("Objet : %s\n",  mylist.heads[0]->rdv_head->objet);
                printf("Date et Heure : %02d/%02d/%04d %02d:%02d\n",  mylist.heads[0]->rdv_head->date.jour, mylist.heads[0]->rdv_head->date.mois, mylist.heads[0]->rdv_head->date.annee, mylist.heads[0]->rdv_head->horaire.heure, mylist.heads[0]->rdv_head->horaire.minute);
                printf("Durée : %02d:%02d\n", mylist.heads[0]->rdv_head->duree.heure, mylist.heads[0]->rdv_head->duree.minute);
            } else if (strcmp(buff, "6\n") == 0){
                run = 0;
            } else if (strcmp(buff, "\n") != 0){
                printf("Unknown Function\n");
            }
        } else {
            printf("Input Error\n");
        }
        printf("\n");

    }

    return 0;
}
