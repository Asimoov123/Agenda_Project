#ifndef AGENDA_PROJECT_AGENDA_H
#define AGENDA_PROJECT_AGENDA_H

#include "cell.h"
#include "list.h"

typedef struct date {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct temps {
    int heure;
    int minute;
} Temps;


typedef struct s_d_rdv {
    Date date;
    Temps horaire;
    Temps duree;
    struct s_d_rdv *next;
    char *objet;
} t_d_rdv;


typedef struct s_d_contact {
    char *nom;
    t_d_rdv *rdv_head;
    t_d_rdv *rdv_tail;
    struct s_d_contact **next;
} t_d_contact;


typedef struct s_d_ContactList { //Liste des contacts
    t_d_contact **heads;
    int max_level;
} t_d_ContactList;


char *scanString(void);

char *Scan_name();

t_d_contact *createContact(char *, int);

void display_level_Contact_aligned(t_d_ContactList mylist, int lvl);

void display_all_levels_Contact_aligned(t_d_ContactList mylist);

int charPlaces(char *name);

t_d_ContactList createContactList();


void insertContact(t_d_ContactList *, char *);

int *checkDate(char *strInput);

int *checkTime(char *);

void rendez_Vous(t_d_contact *);

void display_rendez_vous(t_d_rdv);

void display_all_rendez_vous(t_d_contact);

void display_all_appointment(t_d_contact);


void delete_all_RDV(t_d_contact *);

void delete_Contact(t_d_ContactList *, t_d_contact *);

#endif //AGENDA_PROJECT_AGENDA_H
