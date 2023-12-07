#ifndef AGENDA_PROJECT_AUTOCOMPLETION_H
#define AGENDA_PROJECT_AUTOCOMPLETION_H

#include "agenda.h"

char getch();
char **autocomplete(char input[], t_d_ContactList contactList);
char *autocompleteResearch(t_d_ContactList contactList);

#endif //AGENDA_PROJECT_AUTOCOMPLETION_H
