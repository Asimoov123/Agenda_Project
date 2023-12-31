#ifndef AGENDA_PROJECT_AGENDA_H
#define AGENDA_PROJECT_AGENDA_H


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
    struct s_d_contact **next;
} t_d_contact;


typedef struct s_d_ContactList { //Liste des contacts
    t_d_contact **heads;
    int max_level;
} t_d_ContactList;


char *scanString(void);
char *Scan_name(char*);

t_d_ContactList createContactList();
t_d_contact *createContact(char *, int);
t_d_contact* insertContact(t_d_ContactList *, char *);

void display_level_Contact_aligned(t_d_ContactList contactList, int lvl);
void display_all_levels_Contact_aligned(t_d_ContactList contactList);


int *checkDate(char *strInput);
int *checkTime(char *);

void load_Rendez_Vous(t_d_contact*, char*, char*, char*, char*);
void create_Rendez_Vous(t_d_contact *);
void insert_Rendez_Vous(t_d_contact *, char*, const int*, const int*, const int*);
void display_Rendez_Vous(t_d_rdv);
void display_all_rendez_vous(t_d_ContactList, char *);

int charPlaces(char *name);

void delete_RDV(t_d_contact *, int);
void delete_all_RDV(t_d_contact *);

t_d_contact **isContactInList(t_d_ContactList, char *);
t_d_contact *isContactInListLinear(t_d_ContactList, char *);

void searchContact(t_d_ContactList contactList, char * searchContactName);

void freeAll(t_d_ContactList *);


#endif //AGENDA_PROJECT_AGENDA_H
