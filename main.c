#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "Source_Files/list.h"
#include "Source_Files/timer.h"
#include "Source_Files/files.h"
#include "Source_Files/autocompletion.h"


void compareExecutionTime(int nbOfSearches) {

    t_d_list mylist;
    for (int n = 7; n < 16; n++) {
        int *levels = malloc((long)(pow(2, n) - 1) * sizeof(int));
        for (int i = 0; i < pow(2, n) - 1; i++) {
            levels[i] = 0;
        }

        for (int i = 2; i <= pow(2, n); i *= 2) {
            for (int j = i - 1; j < pow(2, n) - 1; j += i) {
                levels[j]++;
            }
        }

        mylist = createList(n);

        for (int i = 0; i < pow(2, n) - 1; i++) {
            add_Increasing_Order(&mylist, i + 1, levels[i]);
        }

        printf("%-2d ", n);

        int searchedValue;
        startTimer();
        for (int k = 0; k < nbOfSearches; k++) {
            searchedValue = rand() % (int) pow(2, n);
            isValueInListLinear(mylist, searchedValue);
        }
        stopTimer();
        displayTime();
        startTimer();
        for (int k = 0; k < nbOfSearches; k++) {
            searchedValue = rand() % (int) pow(2, n);
            isValueInListMultiLvl(mylist, searchedValue);
        }
        stopTimer();
        displayTime();
        printf("\n");
    }
}

void compareExecutionTime_Contact(int nbOfSearches) {

    FILE *fpin = fopen("CSV_Files/namesMany.csv", "r");
    if (fpin == NULL) {
        printf("Can't open file : %s\n", strerror(errno));
        return;
    }

    t_d_ContactList mylist;
    int lines_extracted, randomLine, num_lines_to_extract, current_line = 0, *lines_to_extract;
    char buffer[128], *searchedName, *all_lines[5003];

    // Read each line from the file
    while (fgets(buffer, 128, fpin) != NULL) {
        all_lines[current_line] = malloc(25 * sizeof(char));
        all_lines[current_line] = strdup(buffer);
        current_line++;
    }
    fclose(fpin);

    for (int n = 7; n < 13; n++) {
        mylist = createContactList();

        num_lines_to_extract = (int) (pow(2, n) - 1);  // Change this to the number of lines you want to extract
        char *extracted_line[num_lines_to_extract];


        lines_to_extract = (int *) malloc(num_lines_to_extract * sizeof(int));
        // Assign the line numbers you want to extract
        for (int i = 0; i < num_lines_to_extract; i++) {
            extracted_line[i] = malloc(25 * sizeof(char));
            lines_to_extract[i] = i * (int) (5000 / (pow(2, n) - 1));  // Adjust this as needed
        }

        // Open the file for reading
        fpin = fopen("CSV_Files/namesMany.csv", "r");
        if (fpin == NULL) {
            printf("Can't open file : %s\n", strerror(errno));
            free(lines_to_extract);
            return;
        }

        current_line = 0;
        lines_extracted = 0;

        // Read each line from the file
        while (fgets(buffer, 128, fpin) != NULL) {
            if (current_line == lines_to_extract[lines_extracted]) {
                // Print or process the extracted line
                insertContact(&mylist, buffer);
                extracted_line[lines_extracted++] = strdup(buffer);
            }
            if (lines_extracted >= num_lines_to_extract) {
                break;
            }
            current_line++;
        }
        fclose(fpin);
        free(lines_to_extract);


        // Open the file for reading
        startTimer();
        for (int i = 0; i < nbOfSearches; i++) {
            randomLine = rand() % 5001;
            // Duplicate the buffer using strdup to avoid modifying the original string
            searchedName = strdup(all_lines[randomLine]);
            isContactInListLinear(mylist, searchedName); // Insert the modified contact name into the contact list
            // Cleaning Memory
            free(searchedName);
            searchedName = NULL; // Set the pointer to NULL for safety
        }
        stopTimer();
        displayTime();

        startTimer();
        for (int i = 0; i < nbOfSearches; i++) {
            randomLine = rand() % 5001;
            // Duplicate the buffer using strdup to avoid modifying the original string
            searchedName = strdup(all_lines[randomLine]);
            isContactInList(mylist, searchedName); // Insert the modified contact name into the contact list
            // Cleaning Memory
            free(searchedName);
            searchedName = NULL; // Set the pointer to NULL for safety
        }
        stopTimer();
        displayTime();

        for (int i = 0; i < num_lines_to_extract; i++) {
            free(extracted_line[i]);
            extracted_line[i] = NULL;
        }

        printf("\n");

    }

    current_line = 0;
    while (current_line < 5003) {
        free(all_lines[current_line]);
        current_line++;
    }
}


int main1() {

    char buff[128];

//    t_d_list mylist2 = createList(9);
//    addHead(&mylist2, 8, 4);
//    addHead(&mylist2, 12, 3);
//    addHead(&mylist2, 4, 8);
//    display_all_levels_aligned(mylist2);

//    t_d_list mylist2 = createList(9);
//    add_Increasing_Order(&mylist2, 8, 4);
//    add_Increasing_Order(&mylist2, 12, 7);
//    add_Increasing_Order(&mylist2, 4, 9);
//    display_all_levels_aligned(mylist2);


    int n = 5;
    int *levels = malloc((long)(pow(2, n) - 1) * sizeof(int));
    for (int i = 0; i < pow(2, n) - 1; i++) {
        levels[i] = 0;
    }

    for (int i = 2; i <= pow(2, n); i *= 2) {
        for (int j = i - 1; j < pow(2, n) - 1; j += i) {
            levels[j]++;
        }
    }

    t_d_list mylist1 = createList(n);

    for (int i = 0; i < pow(2, n) - 1; i++) {
        add_Increasing_Order(&mylist1, i + 1, levels[i]);
    }

    printf("%-2d \n", n);
    display_all_levels_aligned(mylist1);

    compareExecutionTime(1000);

    fgets(buff, 128, stdin);

    return 0;
}


int main() {
    srand(time(NULL));
    t_d_ContactList mylist = createContactList();
    char buff[128];
    int run = 1;
    char *result;

    compareExecutionTime_Contact(100000);

    insertFromFile(mylist, "CSV_Files/names1000_1.csv");
    insertFromFile(mylist, "CSV_Files/names1000_2.csv");
    insertFromFile(mylist, "CSV_Files/names1000_3.csv");
    insertFromFile(mylist, "CSV_Files/names1000_4.csv");
    insertFromFile(mylist, "CSV_Files/names1000_5.csv");


//    insertContact(&mylist, Scan_name("DosSantos Thomas"));
//    insertContact(&mylist, Scan_name("Bracquemart Theo"));
//    insertContact(&mylist, Scan_name("Prevot ROmain"));
//    insertContact(&mylist, Scan_name("Pression blonde"));
//    insertContact(&mylist, Scan_name("Precis bam"));
//    display_all_levels_Contact_aligned(mylist);




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
                if (result) {
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
                if (!isContactInList(mylist, contact_name)) {
                    insertContact(&mylist, contact_name);
                    printf("\033c");
                    printf("[+] Contact successfully registered\n");
                    fgets(buff, 128, stdin);
                    printf("\033c");

                } else {
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
                        if (temp != NULL) {
                            delete_all_RDV(temp);
                            printf("[-] Appointments successfully deleted\n");
                        } else {
                            printf("\nContact not found\n");
                        }
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
            } else if (strcmp(buff, "8\n") == 0) {
                printf("Please enter the first and last name of the contact to insert:\n");
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


                if (!isContactInList(mylist, contact_name)) {
                    startTimer();
                    insertContact(&mylist, contact_name);
                    stopTimer();
                    printf("\033c");
                    printf("[+] Contact successfully registered\n");
                    displayTime();
                    printf(" : Execution Time\n");
                    fgets(buff, 128, stdin);
                    printf("\033c");

                } else {
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
