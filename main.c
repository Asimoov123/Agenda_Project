#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "list.h"
#include "timer.h"
#include <windows.h>
#include "files.h"


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


void compareExecutionTime_Contact(int nbOfSearches){
    srand( time( NULL ) );
    char buffer[128];

    t_d_ContactList mylist;
    for (int n = 7; n<11; n++){
        mylist = createContactList();

        // Open the file for reading
        FILE *fpin = fopen("CSV_Files/namesMany.csv", "r");



        int i = 0;

        // Read each line from the file
        while (fgets(buffer, 128, fpin) && i < pow(2,n)-1) {
            buffer[strlen(buffer) - 1] = '\0';

            // Duplicate the buffer using strdup to avoid modifying the original string
            char *scannedName = Scan_name(strdup(buffer));
            insertContact(&mylist, scannedName); // Insert the modified contact name into the contact list
            // Cleaning Memory
            free(scannedName);
            scannedName = NULL; // Set the pointer to NULL for safety
            i++;
        }
        // Close the file after reading
        fclose(fpin);

        char *searchedName;
        // Open the file for reading
        FILE *fpop;

        startTimer();

        for (int i = 0; i< nbOfSearches; i++){

            fpop = fopen("CSV_Files/namesMany.csv", "r");
            int randomLine = rand() % 5001;
            for (int k=0; k < randomLine; k++){
                fgets(buffer, 128, fpop);
            }
            // Duplicate the buffer using strdup to avoid modifying the original string
            char *scannedName = Scan_name(strdup(buffer));
            isContactInListLinear(mylist, scannedName); // Insert the modified contact name into the contact list
             // Cleaning Memory
            free(scannedName);
            scannedName = NULL; // Set the pointer to NULL for safety
            fclose(fpop);
        }
        stopTimer();
        displayTime();

        startTimer();
        for (int i = 0; i< nbOfSearches; i++){
            int randomLine = rand() % 5001;
            fpop = fopen("CSV_Files/namesMany.csv", "r");

            for (int k=0; k < randomLine; k++){
                fgets(buffer, 128, fpop);
            }
            // Duplicate the buffer using strdup to avoid modifying the original string
            char *scannedName = Scan_name(strdup(buffer));

            isContactInList(mylist, scannedName); // Insert the modified contact name into the contact list

            // Cleaning Memory
            free(scannedName);
            scannedName = NULL; // Set the pointer to NULL for safety
            fclose(fpop);
        }
        stopTimer();
        displayTime();



        printf("\n");
    }
}









// Function to read a key press without waiting for enter
char getch() {
    char buf = 0;
    DWORD mode, NumberOfEventsRead;
    INPUT_RECORD Event;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    // Get the current console mode
    GetConsoleMode(hIn, &mode);
    // Modify the console mode to disable line input and echo input
    SetConsoleMode(hIn, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    // Wait for a key event
    while (ReadConsoleInput(hIn, &Event, 1, &NumberOfEventsRead)) {
        if (Event.EventType == KEY_EVENT && Event.Event.KeyEvent.bKeyDown) {
            // Store the ASCII character of the key pressed
            buf = Event.Event.KeyEvent.uChar.AsciiChar;
            break;
        }

    }
    // Restore the original console mode
    SetConsoleMode(hIn, mode);
    // Return the pressed key
    return buf;
}


// Autocomplete function
char **autocomplete(char input[], t_d_ContactList mylist) {
    // If list is empty
    if (mylist.heads[3] == NULL) {
        return NULL;
    }

    int matchCount = 0, i;
    char *ScannedName = Scan_name(input);

    // Init the matches array
    char **matches = malloc(1024 * sizeof(char *));
    for (i = 0; i < 1024; i++) {
        matches[i] = NULL;
    }

    t_d_contact *currCell = mylist.heads[3];
    int currentLvl = 3;

    // Find matching names
    while (currentLvl >= 0) {
        while (currCell != NULL &&
               ((currentLvl == 0) ? strcmp(currCell->nom, ScannedName) :
                strncmp(currCell->nom, ScannedName, 4 - currentLvl)) < 0) {
            currCell = currCell->next[currentLvl];
        }
        if (currCell != NULL && strncmp(currCell->nom, ScannedName, strlen(ScannedName)) == 0) {
            currentLvl = 0;
            while (currCell != NULL && strncmp(currCell->nom, ScannedName, strlen(ScannedName)) == 0) {
                // Allocate memory for a matched name and store it in the matches array
                matches[matchCount] = malloc(25 * sizeof(char));
                strcpy(matches[matchCount++], currCell->nom);
                currCell = currCell->next[currentLvl];
            }
            break;
        }

        currentLvl--;
    }

    // If no matches, do nothing and return an empty string
    if (matchCount == 0) {
        return NULL;
    }

    // Return the matches
    return matches;
}

// Function to perform autocomplete for user input based on a given contact list
char *autocompleteResearch(t_d_ContactList mylist) {
    // Initialize input arrays
    char input[25], alphabeticalInput[25] = "\0", previousInput[25] = "\0";

    // Allocate memory for the result array
    char **result = malloc(1024 * sizeof(char *));
    for (int i = 0; i < 1024; i++) {
        result[i] = NULL;
    }

    int i = 0, j = 0, k = 0, c = 0, found = 0;

    // Main loop to read user input until Enter key (ASCII 13) is pressed
    while ((input[i] = getch()) != 13 && input[i] != 27) {
        // Check if the input character is an alphabetical character or a space
        if (('A' <= input[i] && input[i] <= 'Z') || ('a' <= input[i] && input[i] <= 'z') || input[i] == ' ' || input[i] == '_' || input[i] == '\'') {
            printf("%c", input[i]);
            alphabeticalInput[k++] = (char) (int) input[i];
        }

        // Check if Backspace key (ASCII 8) is pressed and there is input to delete
        if (input[i] == 8 && alphabeticalInput[0] != '\0') {
            alphabeticalInput[--k] = '\0';
            printf("\33[2K\r%s", alphabeticalInput); // Clear the current line and print the modified input
            input[--i] = '\0';
        }

        // Check if Tab key (ASCII 9) is pressed and the input meets the minimum length
        if (input[i] == '\t' && strlen(alphabeticalInput) >= 3) {
            // Compute autocomplete matches only if it hasn't been done before
            if (!found || strcmp(previousInput, alphabeticalInput) != 0) {
                strcpy(previousInput, alphabeticalInput);
                result = autocomplete(alphabeticalInput, mylist);
                found = 1;
            }
            // If no autocomplete results, skip the rest of this iteration
            if (result == NULL) {
                continue;
            } else {
                // Display the current autocomplete result
                if (result[j + 1] == NULL) {
                    printf("\33[2K\r%s", result[j]);
                    j = 0;
                } else {
                    if (result[j] != NULL) {
                        printf("\33[2K\r%s", result[j++]);
                    }
                }
            }
        } else {
            i++;
        }
    }
    if (input[i]==27){
        return NULL;
    }

    // Null-terminate the input string
    input[i] = '\0';

    // If no autocomplete results check if the input is in the contact list
    if (!result || result[j] == NULL) {
        // If the contact is not found, return the input
        return Scan_name(alphabeticalInput);
    }
    // If there are autocomplete results, return the current result
    while (result[c + 1]) c++;
    j = (j == 0) ? c : j - 1;
    return result[j];
}


int main() {
    t_d_ContactList mylist = createContactList();
    char buff[128];
    int run = 1;
    char *result;
    compareExecutionTime(10000);
//    compareExecutionTime_Contact(10000);

//    insertFromFile(mylist, "CSV_Files/names1000_1.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_2.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_3.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_4.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_5.csv");


    while (run) {
        printf("1. Search Contact\n2. Display Appointments\n3. Create Contact\n4. Create Appointment\n5. Delete Appointment\n6. Save Appointments To File\n7. Load Appointments\n8. Insertion Time\n9. Exit\n> ");
        if (fgets(buff, 128, stdin) != NULL) {
            if (strcmp(buff, "1\n") == 0) {
                printf("Please enter the first and last name of the contact to search:\n");
                result = autocompleteResearch(mylist);
                if (result) {
                    searchContact(mylist, result);
                    free(result);
                    result = NULL;
                }

                fgets(buff, 128, stdin);
                printf("\033c");
            } else if (strcmp(buff, "2\n") == 0) {
                printf("Please enter the first and last name of the contact to display appointments:\n");
                result = autocompleteResearch(mylist);
                if (result){
                    printf("\33[2K\r");
                    display_all_rendez_vous(mylist, result);
                    free(result);
                    result = NULL;
                }
                fgets(buff, 128, stdin);
                printf("\033c");

            } else if (strcmp(buff, "3\n") == 0) {
                printf("Please enter the first and last name of the contact to insert:\n");
                char *contact_name = Scan_name(scanString());
                if (!isContactInList(mylist, contact_name)){
                    insertContact(&mylist, contact_name);
                    printf("\033c");
                    printf("[+] Contact successfully registered\n");
                    fgets(buff, 128, stdin);
                    printf("\033c");

                } else{
                    printf("Contact already registered\n");
                }

            } else if (strcmp(buff, "4\n") == 0) {
                printf("Please enter the first and last name of the contact:\n");
                result = autocompleteResearch(mylist);
                if (result) {
                    t_d_contact **temp = isContactInList(mylist, result);
                    if (temp == NULL) {
                        printf("\nContact not found. Do you want to create a new contact ? (y/n)\n");
                        if (fgets(buff, 128, stdin) != NULL) {
                            if (strcmp(buff, "y\n") == 0) {
                                insertContact(&mylist, result);
                                create_Rendez_Vous(isContactInList(mylist, result)[1]);
                                printf("[+] Appointment successfully added\n");
                            } else {
                                printf("Back to the menu\n");
                            }
                        }
                    } else {
                        create_Rendez_Vous(temp[1]);
                        printf("[+] Appointment successfully added\n");
                    }
                    free(result);
                    result = NULL;
                    fgets(buff, 128, stdin);
                    printf("\033c");
                }

            } else if (strcmp(buff, "5\n") == 0) {
                printf("Please enter the first and last name of the contact:\n");
                char *contact_name = Scan_name(scanString());
                t_d_contact *temp = isContactInList(mylist, contact_name)[1];
                printf("1. Delete an Appointment\n2. Delete All Appointments\n> ");
                if (fgets(buff, 128, stdin) != NULL) {
                    if (strcmp(buff, "1\n") == 0) {
                        printf("Soon...\n");
                    }
                    if (strcmp(buff, "2\n") == 0) {
                        if (temp != NULL) {
                            delete_all_RDV(temp);
                            printf("[-] All appointments successfully deleted\n");
                        } else {
                            printf("\nContact not found\n");
                        }
                    }
                }
                free(contact_name);
                contact_name = NULL;
                fgets(buff, 128, stdin);
                printf("\033c");

            } else if (strcmp(buff, "6\n") == 0) {
                char *filePath = NULL;
                filePath = strdup(saveToFile(mylist, NULL));
                if (filePath != NULL) printf("[+] File Saved at : %s", filePath);
                fgets(buff, 128, stdin);
                printf("\033c");
            } else if (strcmp(buff, "7\n") == 0) {
                loadFromFile(&mylist, scanString());
                fgets(buff, 128, stdin);
                printf("\033c");
            }else if (strcmp(buff, "8\n") == 0) {
                printf("Please enter the first and last name of the contact to insert:\n");
                char *contact_name = Scan_name(scanString());
                t_d_contact * temp = isContactInList(mylist, contact_name)[1];
                printf("1. Delete an Appointment\n2. Delete All Appointments\n> ");
                if (fgets(buff, 128, stdin) != NULL) {
                    if (strcmp(buff, "1\n") == 0) {
                        printf("Soon...\n");
                    }
                    if (strcmp(buff, "2\n") == 0) {
                        if (temp != NULL) {
                            delete_all_RDV(temp);
                            printf("[-] All appointments successfully deleted\n");
                        } else {
                            printf("\nContact not found\n");
                        }
                    }
                }
                free(contact_name);
                contact_name = NULL;
                fgets(buff, 128, stdin);
                printf("\033c");




                if (!isContactInList(mylist, contact_name)){
                    startTimer();
                    insertContact(&mylist, contact_name);
                    stopTimer();
                    printf("\033c");
                    printf("[+] Contact successfully registered\n");
                    displayTime();
                    printf(" : Execution Time\n");
                    fgets(buff, 128, stdin);
                    printf("\033c");

                } else{
                    printf("Contact already registered\n");
                }

            } else if (strcmp(buff, "9\n") == 0) {
                run = 0;
            } else if (strcmp(buff, "\n") != 0) {
                printf("Unknown Function\n");
                fgets(buff, 128, stdin);
                printf("\033c");
            }
        } else {
            printf("Input Error\n");
            fgets(buff, 128, stdin);
            printf("\033c");
        }


    }

    freeAll(&mylist);


    return 0;
}
