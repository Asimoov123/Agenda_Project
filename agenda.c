#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "agenda.h"


char *scanString(void) {
    char *p = NULL;
    char character;
    int index = 0;
    int size = 1;
    p = (char *) malloc(sizeof(char));
    while (1) {
        character = (char) getchar(); // Permet d'attendre la frappe d'un caractère au clavier

        if (character == '\n') { // Vérifie si le caractère est le dernier saisi par l'utilisateur
            p[index] = '\0';  // Termine la chaîne
            break;
        }
        if (index >= size - 1) { // Réalloue de la memoire si nécessaire
            size *= 2;
            char *temp = (char *) realloc(p, size * sizeof(char));
            if (temp == NULL) {
                free(p);
                p = NULL;
                printf("Error: Failed to reallocate memory! : %s\n", strerror(errno));
                exit(1);
            }
            p = temp;
        }
        p[index++] = character;
    }
    return p;
}

char *Scan_name(char *strInput) {
    char *full_name = malloc(strlen(strInput) + 1);
    strcpy(full_name, strInput);

    for (int i = 0; i < strlen(full_name); i++) {
        if ('A' <= full_name[i] && full_name[i] <= 'Z') {
            full_name[i] = (char) (full_name[i] + 'a' - 'A');
        } else {
            if (full_name[i] == ' ') {
                full_name[i] = '_';
                i--;
            } else if ((full_name[i] < 'a' || 'z' < full_name[i]) && full_name[i] != '_') {
                for (int j = i; j < strlen(full_name); j++) {
                    full_name[j] = full_name[j + 1];
                }
                i--;
            }
        }
    }
    return full_name;
}

/* This function creates a new contact with a given name and level.
It returns a pointer to the newly created contact. */
t_d_contact *createContact(char *name, int lvl) {
    t_d_contact *contact = malloc(sizeof(t_d_contact));
    contact->nom = strdup(name);

    contact->next = (t_d_contact **) malloc(sizeof(t_d_contact *) * (lvl + 1));
    for (int i = 0; i < (lvl + 1); i++) {
        contact->next[i] = NULL;
    }
    contact->rdv_head = NULL;
    contact->rdv_tail = NULL;

    return contact;
}


// This function creates and returns a new contact list.
t_d_ContactList createContactList() {
    t_d_ContactList mylist;
    mylist.max_level = 4;
    mylist.heads = (t_d_contact **) malloc(sizeof(t_d_contact *) * mylist.max_level);
    for (int i = 0; i < 4; i++) {
        mylist.heads[i] = (t_d_contact *) malloc(sizeof(t_d_contact));
        mylist.heads[i] = NULL;
    }
    return mylist;
}


// This function creates and inserts alphabetically a new contact with a given name.
t_d_contact * insertContact(t_d_ContactList *mylist, char *newContactName) {
    t_d_contact *prevCell = NULL, *currCell = mylist->heads[3];
    int currentLvl = 3, previousLvl = 3;

    // If the new contact is to be inserted at the beginning of the list
    if (currCell == NULL) {
        t_d_contact *newContact = createContact(newContactName, 3); // First element is always at max level
        for (int c = 0; c < 4; c++) {
            mylist->heads[c] = newContact;
        }
        return newContact;
    }


    // Locate insertion point for new contact
    while (currentLvl >= 0) {
        while (currCell != NULL &&
               ((currentLvl == 0) ? strcmp(currCell->nom, newContactName) :
                strncmp(currCell->nom, newContactName, 4 - currentLvl)) < 0) {
            prevCell = currCell;
            currCell = currCell->next[currentLvl];
            previousLvl = currentLvl;
        }
        if (currCell == NULL || strncmp(currCell->nom, newContactName, 4 - currentLvl) > 0) {
            break;
        } else if (strcmp(currCell->nom, newContactName) == 0) {
            printf("Contact already registered.\n");
            free(newContactName);
            newContactName = NULL;
            return currCell;
        }
        currentLvl--;
    }

    // Insert the new contact
    if (currCell == NULL && strcmp(prevCell->nom, newContactName) < 0) { // Insert at the end

        t_d_contact *newContact = createContact(newContactName, currentLvl);
        t_d_contact *temp;
        for (int i = 0; i < currentLvl + 1; i++) {
            temp = prevCell;
            while (temp->next[i] != NULL && i < mylist->max_level) {
                temp = temp->next[i];
            }
            temp->next[i] = newContact;
        }
        return newContact;
    } else if (strcmp(currCell->nom, newContactName) > 0) { // Insert Before CurrentCell
        t_d_contact *newContact = createContact(newContactName, previousLvl); // Create new contact

        for (int i = 0; i <= currentLvl; i++) {
            newContact->next[i] = currCell;
        }
        if (currentLvl < previousLvl) {
            for (int i = currentLvl + 1; i <= previousLvl; i++) {
                newContact->next[i] = currCell->next[i];
            }
        }
        if (prevCell == NULL) {
            for (int c = 0; c < 4; c++) {
                mylist->heads[c] = newContact;
            }
        } else {
            t_d_contact *temp;
            for (int i = 0; i <= previousLvl; i++) {
                temp = prevCell;
                while (temp->next[i] != currCell && i < mylist->max_level) {
                    temp = temp->next[i];
                }
                temp->next[i] = newContact;
            }
        }
        return newContact;
    }
}


void delete_all_RDV(t_d_contact *contact) {
    t_d_rdv *temp;
    while (contact->rdv_head != NULL) {
        temp = contact->rdv_head;
        contact->rdv_head = contact->rdv_head->next;
        free(temp);
    }
}



//void delete_Contact(t_d_ContactList *mylist, char *delContactName) {
//    t_d_contact *prevCell = NULL, *currCell = mylist->heads[3];
//    int currentLvl = 3, previousLvl = 3, found = 0;
//
//
//    if (strcmp(currCell->nom, delContactName) == 0) {
//        for (int i = 0; i < 4; i++) {
//            mylist->heads[i] = currCell->next[i];
//        }
//    } else {
//        // Locate contact to delete
//        while (currentLvl >= 0) {
//            while (currCell != NULL && strncmp(currCell->nom, delContactName, 4 - currentLvl) < 0) {
//                prevCell = currCell;
//                currCell = currCell->next[currentLvl];
//                previousLvl = currentLvl;
//            }
//            if (currCell == NULL || strcmp(currCell->nom, delContactName) > 0) {
//                printf("Contact not found.");
//                return;
//            } else if (strcmp(currCell->nom, delContactName) == 0) {
//                found = 1;
//                break;
//            }
//            currentLvl--;
//        }
//
//        t_d_contact *temp;
//        for (int i = 0; i <= previousLvl; i++) {
//            temp = prevCell;
//            while (temp->next[i] != currCell) {
//                temp = temp->next[i];
//            }
//            temp->next[i] = currCell->next[i];
//        }
//    }
//
//    delete_all_RDV(currCell);
//    free(currCell);
//    currCell = NULL;
//    printf("Contact successfully deleted.");
//}

t_d_contact **isContactInList(t_d_ContactList mylist, char *searchContactName) {
    // If list is empty
    if (mylist.heads[3] == NULL || searchContactName == NULL) {
        return NULL;
    }

    t_d_contact *prevCell = NULL, *currCell = mylist.heads[3];
    t_d_contact **res = malloc(2 * sizeof(t_d_contact *));

    if (res == NULL) {
        printf("ERROR: Failed to allocate memory.\n");
        exit(1);
    }
    int currentLvl = 3;

    // Locate contact in list
    while (currentLvl >= 0) {
        while (currCell != NULL && strncmp(currCell->nom, searchContactName, 4 - currentLvl) < 0) {
            prevCell = currCell;
            currCell = currCell->next[currentLvl];
        }
        if (currCell == NULL || strcmp(currCell->nom, searchContactName) > 0) {
            free(res);
            res = NULL;
        } else if (strcmp(currCell->nom, searchContactName) == 0) {
            res[0] = prevCell;
            res[1] = currCell;
        }
        currentLvl--;
    }
    return res;
}


void searchContact(t_d_ContactList mylist, char *searchContactName) {
    if (isContactInList(mylist, searchContactName))
        printf("Contact Found\n");
    else
        printf("Contact Not Found\n");
}


int *checkDate(char *strInput) {
    const char *separator = "/";
    int *intDate = malloc(3 * sizeof(int));
    char *strToken = strtok(strInput, separator);
    int i = 0, jj, mm, aaaa;
    while (strToken != NULL && i < 3) {
        for (int k = 0; k < strlen(strToken); k++) {
            if (!isdigit(strToken[k])) {
                printf("!digit\n");
                free(strInput);
                strInput = NULL;
                free(intDate);
                intDate = NULL;
                return NULL;
            }
        }
        intDate[i++] = strtol(strToken, NULL, 10);
        strToken = strtok(NULL, separator);
    }

    jj = intDate[0];
    mm = intDate[1];
    aaaa = intDate[2];

    if (aaaa >= 1900 && aaaa <= 9999) {
        if (mm >= 1 && mm <= 12) {
            if (((jj >= 1 && jj <= 31) &&
                 (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)) || // Mois à 31 jours
                ((jj >= 1 && jj <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11)) || // Mois à 30 jours
                ((jj >= 1 && jj <= 28) && (mm == 2)) || // Mois de février
                (jj == 29 && mm == 2 && (aaaa % 400 == 0 || (aaaa % 4 == 0 && aaaa % 100 != 0)))) { //Années bissextiles
                return intDate;
            }
        }
    }
    printf("NotvalidDate\n");
    free(strInput);
    strInput = NULL;
    free(intDate);
    intDate = NULL;
    return NULL;
}

int *checkTime(char *strInput) {
    const char *separator = ":";
    int *intTime = malloc(2 * sizeof(int));
    char *strToken = strtok(strInput, separator);
    int i = 0, hh, mm;

    while (strToken != NULL && i < 2) {
        for (int k = 0; k < strlen(strToken); k++) {
            if (!isdigit(strToken[k])) {
                free(strInput);
                strInput = NULL;
                free(intTime);
                intTime = NULL;
                return NULL;
            }
        }
        intTime[i++] = strtol(strToken, NULL, 10);
        strToken = strtok(NULL, separator);
    }

    hh = intTime[0];
    mm = intTime[1];

    if (hh >= 0 && hh <= 23) {
        if (mm >= 0 && mm <= 59) {
            return intTime;
        }
    }

    free(strInput);
    strInput = NULL;
    free(intTime);
    intTime = NULL;
    return NULL;
}

void load_Rendez_Vous(t_d_contact *contact, char *object, char *dateStr, char *timeStr, char *durationStr) {
    int *intDate = checkDate((dateStr));
    if (intDate == NULL) {
        return;
    }
    int *intTime = checkTime((timeStr));
    if (intTime == NULL) {
        return;
    }
    int *intDuration = checkTime((durationStr));
    if (intDuration == NULL) {
        return;
    }
    insert_Rendez_Vous(contact, object, intDate, intTime, intDuration);
}

void create_Rendez_Vous(t_d_contact *contact) {
    printf("\nWhat is your appointment about? : \n");
    char *object = scanString();
    printf("Please enter the date of your appointment in the following format dd/mm/yyyy : \n");
    char *dateStr = scanString();
    int *intDate = checkDate((strdup(dateStr)));
    while (intDate == NULL) {
        printf("Invalid date\nPlease enter the date of your appointment in the following format dd/mm/yyyy : \n");
        dateStr = scanString();
        intDate = checkDate(strdup(dateStr));
        free(dateStr);
        dateStr = NULL;
    }
    if (dateStr) {
        free(dateStr);
        dateStr = NULL;
    }

    printf("Please enter the appointment time in the following format hh:mm : \n");
    char *timeStr = scanString();
    int *intTime = checkTime((strdup(timeStr)));
    while (intTime == NULL) {
        printf("/!\\Invalid time\nPlease enter the appointment time in the following format hh:mm : \n");
        timeStr = scanString();
        intTime = checkTime((strdup(timeStr)));
        free(timeStr);
        timeStr = NULL;
    }
    if (timeStr) {
        free(timeStr);
        timeStr = NULL;
    }

    printf("Please enter the appointment duration time in the following format hh:mm : \n");
    char *durationStr = scanString();
    int *intDuration = checkTime((strdup(durationStr)));
    while (intDuration == NULL) {
        printf("/!\\Invalid time\nPlease enter the appointment duration time in the following format hh:mm : \n");
        durationStr = scanString();
        intDuration = checkTime((strdup(durationStr)));
        free(durationStr);
        durationStr = NULL;
    }
    if (durationStr) {
        free(durationStr);
        durationStr = NULL;
    }

    insert_Rendez_Vous(contact, object, intDate, intTime, intDuration);

    free(intDate);
    intDate = NULL;
    free(intTime);
    intTime = NULL;
    free(intDuration);
    intDuration = NULL;
}


void insert_Rendez_Vous(t_d_contact *contact, char *object, const int *intDate, const int *intTime, const int *intDuration) {
    t_d_rdv *rdv = malloc(sizeof(t_d_rdv));
    rdv->next = NULL;
    t_d_rdv *temp;
    t_d_rdv *prev;
    temp = contact->rdv_head;
    rdv->objet = object;


    rdv->date.jour = intDate[0];
    rdv->date.mois = intDate[1];
    rdv->date.annee = intDate[2];

    rdv->horaire.heure = intTime[0];
    rdv->horaire.minute = intTime[1];

    rdv->duree.heure = intDuration[0];
    rdv->duree.minute = intDuration[1];


    // Rajout dans la liste par ordre croissant
    if (temp == NULL) {
        contact->rdv_head = rdv;
        contact->rdv_tail = rdv;
        return;
    }
    if (rdv->date.annee < temp->date.annee ||
        temp->date.annee == rdv->date.annee && rdv->date.annee < temp->date.annee ||
        temp->date.annee == rdv->date.annee && rdv->date.annee == temp->date.annee &&
        rdv->date.jour < temp->date.jour) {
        rdv->next = temp;
        contact->rdv_head = rdv;
        return;
    }
    prev = temp;
    temp = temp->next;
    while (temp != NULL) {

        // Equal date
        if ((temp->date.annee == rdv->date.annee) && (temp->date.mois == rdv->date.mois) &&
            (temp->date.jour == rdv->date.jour)) {
            if (rdv->horaire.heure < temp->horaire.heure) {
                temp = rdv;
                temp->next = contact->rdv_head;
                contact->rdv_head = temp;
                temp = NULL;
                break;
            } else {
                if (rdv->horaire.heure == temp->horaire.heure && rdv->horaire.minute < temp->horaire.minute) {
                    temp = rdv;
                    temp->next = contact->rdv_head;
                    contact->rdv_head = temp;
                    temp = NULL;
                    break;
                } else {
                    rdv->next = temp->next;
                    temp->next = rdv;
                    break;
                }
            }
        }
        if (rdv->date.annee < temp->date.annee) {
            break;
        }
        if (temp->date.annee == rdv->date.annee) {    //for month
            if (rdv->date.mois < temp->date.mois) {
                break;
            }

            if (temp->date.mois == rdv->date.mois) {           //for day
                if (rdv->date.jour < temp->date.jour) {
                    break;
                }
            }
        }
        prev = temp;
        temp = temp->next;
    }
    prev->next = rdv;
    rdv->next = temp;
}

void display_Rendez_Vous(t_d_rdv rdv) {
    printf("\tObject : '%s'\n", rdv.objet);
    printf("\t\tDate : %02d/%02d/%04d\n", rdv.date.jour, rdv.date.mois, rdv.date.annee);
    printf("\t\tTime : %02d:%02d\n", rdv.horaire.heure, rdv.horaire.minute);
    if (rdv.duree.heure == 0) printf("\t\tDuration : %02d minutes\n\n", rdv.duree.minute);
    else printf("\t\tDuration : %02d hours and %02d minutes\n\n", rdv.duree.heure, rdv.duree.minute);
}


void display_all_rendez_vous(t_d_ContactList mylist, char *rdvContactName) {
    t_d_contact **isContact = isContactInList(mylist, rdvContactName);
    if (isContact == NULL) {
        printf("Contact not found\n");
        return;
    }
    t_d_contact *contact = isContact[1];
    t_d_rdv *rdv_cur;
    rdv_cur = contact->rdv_head;
    printf("%s's appointments :\n", contact->nom);
    while (rdv_cur != NULL) {
        display_Rendez_Vous(*rdv_cur);
        rdv_cur = rdv_cur->next;
    }
}

int charPlaces(char *name) {
    return (int) strlen(name) - 1;
}


void display_level_Contact_aligned(t_d_ContactList mylist, int lvl) {
    printf("[list head_%d @-]--", lvl);
    t_d_contact *current0Cell = mylist.heads[0]; // temporary cell
    t_d_contact *currentLvlCell = mylist.heads[lvl]; // temporary cell

    while (current0Cell != NULL) {
        if (current0Cell !=
            currentLvlCell) { // If the cell on the first line is different from the one on the line we want to display
            while (current0Cell != currentLvlCell) { // while it is true, display '-' instead of cell
                printf("---%.*s------", charPlaces(current0Cell->nom),
                       "-----------------------------"); // https://stackoverflow.com/questions/14678948/how-to-repeat-a-char-using-printf
                current0Cell = current0Cell->next[0]; // temporary cell is set to its successor (on the first line)
            }
        }
        if (currentLvlCell == NULL || current0Cell == NULL) break;
        printf(">[%s|@-]--", currentLvlCell->nom); // display the cell
        current0Cell = current0Cell->next[0]; // the cell on the first line is set to its successor
        currentLvlCell = currentLvlCell->next[lvl]; // the cell on level we want to display is set to its successor
    }
    printf(">NULL\n");
}


void display_all_levels_Contact_aligned(t_d_ContactList mylist) {
    for (int i = 0; i < 4; i++) {
        display_level_Contact_aligned(mylist, i);
    }
}


void freeAll(t_d_ContactList *mylist) {
    t_d_contact *temp;
    while (mylist->heads[0] != NULL) {
        temp = mylist->heads[0];
        mylist->heads[0] = mylist->heads[0]->next[0];
        delete_all_RDV(temp);
        free(temp->rdv_head);
        temp->rdv_head = NULL;
        free(temp);
        temp = NULL;
    }
}
