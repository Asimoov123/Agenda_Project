#include "files.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <time.h>


char *saveToFile(t_d_ContactList contactList, char *filePath) {
    FILE *fpt = NULL;
    if (filePath == NULL) {
        filePath = malloc(512 * sizeof(char));
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(filePath, "CSV_Files/appointments_%02d-%02d-%04d_%02d-%02d.csv", tm.tm_mday, tm.tm_mon + 1,
                tm.tm_year + 1900, tm.tm_hour, tm.tm_min);

    }

    fpt = fopen(filePath, "w");
    if (fpt == NULL) {
        printf("can't open\n");
        return NULL;
    }

    t_d_contact *temp = contactList.heads[0];
    while (temp != NULL) {
        fprintf(fpt, "%s;", temp->nom);
        t_d_rdv *tempRDV = temp->rdv_head;
        while (tempRDV != NULL) {
            fprintf(fpt, "{;%s;%02d/%02d/%04d;%02d:%02d;%02d:%02d;};", tempRDV->objet, tempRDV->date.jour,
                    tempRDV->date.mois, tempRDV->date.annee, tempRDV->horaire.heure, tempRDV->horaire.minute,
                    tempRDV->duree.heure, tempRDV->duree.minute);
            tempRDV = tempRDV->next;
        }
        fprintf(fpt, "\n");
        temp = temp->next[0];
    }
    fclose(fpt);
    return filePath;
}


void loadFromFile(t_d_ContactList* contactList, char* filePath) {
    FILE *fpt = fopen(filePath, "r");
    if (fpt == NULL) {
        printf("[-] Failed to load file : %s\n", strerror(errno));
        return;
    }
    char line[1024];
    char *strToken, *name, *object, *dateStr, *timeStr, *durationStr;
    t_d_contact *temp;
    while (fgets(line, 1024, fpt)) {
        strToken = strtok(line, ";");
        name = Scan_name(strdup(strToken));

        if (!isContactInList(*contactList, name)){
            printf("[+] %s\n", name);

        }
        temp = insertContact(contactList, name);

        while (strToken != NULL) {
            if (strcmp(strToken, "{") == 0) {
                strToken = strtok(NULL, ";");
                object = strdup(strToken);
                strToken = strtok(NULL, ";");
                dateStr = strdup(strToken);
                strToken = strtok(NULL, ";");
                timeStr = strdup(strToken);
                strToken = strtok(NULL, ";");
                durationStr = strdup(strToken);
                strToken = strtok(NULL, ";");
                if (strcmp(strToken, "}") == 0) {
                    load_Rendez_Vous(temp, object, dateStr, timeStr, durationStr);
                    printf("[+] Appointment successfully added\n");
                } else {
                    printf("[-] Can't load file : Wrong file structure\n");
                    return;
                }
            }
            strToken = strtok(NULL, ";");
        }
    }
}

// Function to insert contacts from a csv file
void insertFromFile(t_d_ContactList contactList, char *filePath) {
    // Open the file for reading
    FILE *fp = fopen(filePath, "r");

    // Check if the file opening was successful
    if (fp == NULL) {
        printf("Can't open file : %s\n", strerror(errno));
        exit(1); // Exit the program with an error code
    }

    char buffer[128];
    // Read each line from the file
    while (fgets(buffer, 128, fp)) {
        buffer[strlen(buffer) - 1] = '\0';

        // Duplicate the buffer using strdup to avoid modifying the original string
        char *scannedName = Scan_name(strdup(buffer));
        insertContact(&contactList, scannedName); // Insert the modified contact name into the contact list
        // Cleaning Memory
        free(scannedName);
        scannedName = NULL; // Set the pointer to NULL for safety
    }

    // Close the file after reading
    fclose(fp);
}