#ifndef AGENDA_PROJECT_FILES_H
#define AGENDA_PROJECT_FILES_H

#include "agenda.h"

char *saveToFile(t_d_ContactList contactList, char *filePath);
void loadFromFile(t_d_ContactList* contactList, char* filePath);
void insertFromFile(t_d_ContactList contactList, char *filePath);

#endif //AGENDA_PROJECT_FILES_H
