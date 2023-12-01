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

        if (character == '\n') { // Vérifie si le charactere est le dernier saisi par l'utilisateur
            p[index] = '\0';  // Termine la chaîne
            break;
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


char *Scan_name() {
    char *nom_complet;
    printf("Veuillez saisir le nom et le prenom du nouveau contact :\n");
    nom_complet = scanString();
    for (int i = 0; i < strlen(nom_complet); i++) {
        if (nom_complet[i] >= 'A' && nom_complet[i] <= 'Z') {
            nom_complet[i] = (char) (nom_complet[i] + 'a' - 'A');
        } else {
            if (nom_complet[i] == ' ') {
                nom_complet[i] = '_';
                i--;
            }
        }
    }
    return nom_complet;
}


t_d_contact *createContact(char *name, int lvl) {
    t_d_contact *contact = malloc(sizeof(t_d_contact));
    contact->nom = name;

    contact->next = (t_d_contact **) malloc(sizeof(t_d_contact *) * (lvl + 1));
    for (int i = 0; i < (lvl + 1); i++) {
        contact->next[i] = NULL;
    }
    contact->rdv_head = NULL;
    contact->rdv_tail = NULL;

    return contact;
}

t_d_ContactList createContactList() {
    t_d_ContactList mylist;
    mylist.max_level = 4;
    mylist.heads = (t_d_contact **) malloc(sizeof(t_d_contact *) * mylist.max_level);

    for (int i = 0; i < 4; i++) {
        mylist.heads[i] = NULL;
    }
    return mylist;
}


void insertContact(t_d_ContactList *mylist, char *newContactName) {
    t_d_contact *prevCell = NULL, *currCell = mylist->heads[3];
    int currentLvl = 3, previousLvl = 3;

    if (currCell == NULL || strcmp(currCell->nom, newContactName) > 0) {
        t_d_contact *newContact = createContact(newContactName, 3);
        for (int c = 0; c < 4; c++) {
            newContact->next[c] = currCell;
            mylist->heads[c] = newContact;
        }
        return;
    }

    while (currentLvl >= 0) {
        while (currCell != NULL && strncmp(currCell->nom, newContactName, 4 - currentLvl) < 0) {
            prevCell = currCell;
            currCell = currCell->next[currentLvl];
            previousLvl = currentLvl;
        }
        if (currCell == NULL || strncmp(currCell->nom, newContactName, 4 - currentLvl) > 0) break;
        currentLvl--;
    }
    if (currCell == NULL && strcmp(prevCell->nom, newContactName) < 0) { // Insert End
        t_d_contact *newContact = createContact(newContactName, currentLvl);
        t_d_contact *temp;
        for (int i = 0; i < currentLvl + 1; i++) {
            temp = prevCell;
            while (temp->next[i] != NULL) {
                temp = temp->next[i];
            }
            temp->next[i] = newContact;
        }
    } else if (strcmp(currCell->nom, newContactName) > 0) { // Insert Before CurrentCell
        t_d_contact *updateCurrCell = createContact(currCell->nom,
                                                    currentLvl); // update current contact number of cells
        t_d_contact *newContact = createContact(newContactName, previousLvl); // create new contact

        for (int i = 0; i <= currentLvl; i++) {
            updateCurrCell->next[i] = currCell->next[i];
            newContact->next[i] = updateCurrCell;
        }
        for (int i = currentLvl + 1; i <= previousLvl; i++) {
            newContact->next[i] = currCell->next[i];
        }
        t_d_contact *temp;
        for (int i = 0; i <= previousLvl; i++) {
            temp = prevCell;
            while (temp->next[i] != currCell) {
                temp = temp->next[i];
            }
            temp->next[i] = newContact;
        }
    }
    free(currCell);
    currCell = NULL;
}


void delete_all_RDV(t_d_contact *contact) {
    t_d_rdv *prec, *temp;
    temp = contact->rdv_head;
    prec = NULL;
    while (temp != NULL) {
        prec = temp;
        temp = temp->next;
        free(prec);
    }
}

void delete_Contact(t_d_ContactList *mylist, t_d_contact *contact_del) {
    t_d_contact *temp, *prec;
    for (int i = 0; i < 4; i++) {
        prec = NULL;
        temp = mylist->heads[i];
        while (strcmp(temp->nom, contact_del->nom) == 0) {
            prec = temp;
            temp = temp->next[i];
        }
        prec->next[i] = temp->next[i];
        contact_del->next[i] = NULL;
    }
    delete_all_RDV(contact_del);
    free(contact_del);
}


int *checkDate(char *strInput) {
    const char *separator = "/";
    int *intDate = malloc(4 * sizeof(int));
    char *strToken = strtok(strInput, separator);
    int i = 1, jj, mm, aaaa;
    while (strToken != NULL && i < 4) {
        for (int k = 0; k < strlen(strToken); k++) {
            if (!isdigit(strToken[k])) {
                intDate[0] = 0;
                return intDate;
            }
        }
        intDate[i++] = strtol(strToken, NULL, 10);
        strToken = strtok(NULL, separator);
    }

    jj = intDate[1];
    mm = intDate[2];
    aaaa = intDate[3];

    if (aaaa >= 1900 && aaaa <= 9999) {
        if (mm >= 1 && mm <= 12) {
            if (((jj >= 1 && jj <= 31) &&
                 (mm == 1 || mm == 3 || mm == 5 || mm == 7 || mm == 8 || mm == 10 || mm == 12)) || // Mois à 31 jours
                ((jj >= 1 && jj <= 30) && (mm == 4 || mm == 6 || mm == 9 || mm == 11)) || // Mois à 30 jours
                ((jj >= 1 && jj <= 28) && (mm == 2)) || // Mois de février
                (jj == 29 && mm == 2 && (aaaa % 400 == 0 || (aaaa % 4 == 0 && aaaa % 100 != 0)))) { // Années Bisextile
                intDate[0] = 1;
                return intDate;
            }
        }
    }
    intDate[0] = 0;
    return intDate;
}

int *checkTime(char *strInput) {
    const char *separator = ":";
    int *intTime = malloc(3 * sizeof(int));
    char *strToken = strtok(strInput, separator);
    int i = 1, hh, mm;
    while (strToken != NULL && i < 3) {
        for (int k = 0; k < strlen(strToken); k++) {
            if (!isdigit(strToken[k])) {
                intTime[0] = 0;
                return intTime;
            }
        }
        intTime[i++] = strtol(strToken, NULL, 10);
        strToken = strtok(NULL, separator);
    }

    hh = intTime[1];
    mm = intTime[2];

    if (hh >= 0 && hh <= 23) {
        if (mm >= 0 && mm <= 59) {
            intTime[0] = 1;
            return intTime;
        }
    }

    intTime[0] = 0;
    return intTime;
}


void rendez_Vous(t_d_contact *nom) {
    t_d_rdv *rdv = malloc(sizeof(t_d_rdv));
    rdv->next = NULL;
    t_d_rdv *temp;
    t_d_rdv *prev;
    temp = nom->rdv_head;

    printf("Quel est l'objet de votre rendez-vous ? : \n");
    char *texte = scanString();
    rdv->objet = texte;
    printf("Veuillez saisir la date de votre rendez-vous sous la forme suivante jj/mm/aaaa \n");
    char *dateStr = scanString();
    int *intDate = checkDate((dateStr));
    while (intDate[0] == 0) {
        printf("Date invalide\nVeuillez saisir la date de votre rendez-vous sous la forme suivante jj/mm/aaaa \n");
        dateStr = scanString();
        intDate = checkDate((dateStr));
    }
    rdv->date.jour = intDate[1];
    rdv->date.mois = intDate[2];
    rdv->date.annee = intDate[3];

    printf("Veuillez rentrer l'heure du rendez-vous sous la forme suivante hh:mm \n");
    char *timeStr = scanString();
    int *intTime = checkTime((timeStr));
    while (intTime[0] == 0) {
        printf("Horaire invalide\nVeuillez rentrer l'heure du rendez-vous sous la forme suivante hh:mm \n");
        timeStr = scanString();
        intTime = checkTime((timeStr));
    }

    rdv->horaire.heure = intTime[1];
    rdv->horaire.minute = intTime[2];


    printf("Veuillez saisir combien de temps durera votre rendez-vous sous la forme hh:mm  \n");
    char *durationStr = scanString();
    int *intDuration = checkTime((durationStr));
    while (intDuration[0] == 0) {
        printf("Temps invalide\nVeuillez saisir combien de temps durera votre rendez-vous sous la forme hh:mm  \n");
        durationStr = scanString();
        intDuration = checkTime((durationStr));
    }
    rdv->duree.heure = intDuration[1];
    rdv->duree.minute = intDuration[2];

    // Je sais pas s'il sont tous utiles
    free(dateStr);
    dateStr = NULL;
    free(intDate);
    intDate = NULL;
    free(timeStr);
    timeStr = NULL;
    free(intTime);
    intTime = NULL;
    free(durationStr);
    durationStr = NULL;
    free(intDuration);
    intDuration = NULL;


    //Rajout dans la liste par ordre croissant

    if (temp == NULL) {
        nom->rdv_head = rdv;
        nom->rdv_tail = rdv;
        return;
    }
    if (rdv->date.annee < temp->date.annee ||
        temp->date.annee == rdv->date.annee && rdv->date.annee < temp->date.annee ||
        temp->date.annee == rdv->date.annee && rdv->date.annee == temp->date.annee &&
        rdv->date.jour < temp->date.jour) {
        rdv->next = temp;
        nom->rdv_head = rdv;
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
                temp->next = nom->rdv_head;
                nom->rdv_head = temp;
                temp = NULL;
                break;
            } else {
                if (rdv->horaire.heure == temp->horaire.heure && rdv->horaire.minute < temp->horaire.minute) {
                    temp = rdv;
                    temp->next = nom->rdv_head;
                    nom->rdv_head = temp;
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

void display_rendez_vous(t_d_rdv rdv) {
    printf("\t- Votre rendez-vous '%s' est prevu:\n", rdv.objet);
    printf("\t\tLe %d/%d/%d;\n", rdv.date.jour, rdv.date.mois, rdv.date.annee);
    if (rdv.horaire.minute < 10) {
        printf("\t\tA %dh0%d;\n", rdv.horaire.heure, rdv.horaire.minute);
    } else {
        printf("\t\tA %dh%d;\n", rdv.horaire.heure, rdv.horaire.minute);
    }
    printf("\t\tD'une duree de %d heures et %d minutes.\n\n", rdv.duree.heure, rdv.duree.minute);
}


void display_all_rendez_vous(t_d_contact contact) {
    t_d_rdv *rdv_cur;
    rdv_cur = contact.rdv_head;
    printf("Voici les rendez-vous de %s :\n", contact.nom);
    while (rdv_cur != NULL) {
        display_rendez_vous(*rdv_cur);
        rdv_cur = rdv_cur->next;
    }
}

void display_all_appointment(t_d_contact contact) {
    t_d_rdv *cur;
    cur = contact.rdv_head;
    while (cur != NULL) {
        display_rendez_vous(*cur);
        cur = cur->next;
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
        if (currentLvlCell == NULL) break;
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

