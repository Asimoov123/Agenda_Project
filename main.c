#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include "Source_Files/list.h"
#include "Source_Files/timer.h"
#include "Source_Files/files.h"
#include "Source_Files/autocompletion.h"
#include "Complexity/complexity.h"





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



//    insertFromFile(mylist, "CSV_Files/names1000_1.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_2.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_3.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_4.csv");
//    insertFromFile(mylist, "CSV_Files/names1000_5.csv");


    while (run) {
        printf("1. Search Contact\n2. Display Appointments\n3. Create Contact\n4. Create Appointment\n5. Delete Appointment\n6. Save Appointments To File\n7. Load Appointments\n8. Insertion & Research Time\n9. Exit\n> ");
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
                result = autocompleteResearch(mylist);
                if (result) {
                    t_d_contact **temp = isContactInList(mylist, result);
                    printf("1. Delete an Appointment\n2. Delete All Appointments\n> ");
                    if (fgets(buff, 128, stdin) != NULL && temp != NULL) {
                        if (strcmp(buff, "1\n") == 0) {
                            if (temp != NULL) {
                                display_all_rendez_vous(mylist, temp[1]->nom);
                                printf("Number of the appointment to delete : ");
                                if (fgets(buff, 2, stdin) != NULL) {
                                    int isStrdigit = 1;
                                    for (int i = 0; i < strlen(buff); i++) {
                                        if (!isdigit(buff[i])) {
                                            isStrdigit = 0;
                                        }
                                        if (isStrdigit) {
                                            int j = strtol(buff, NULL, 2);
                                            delete_RDV(temp[1], j);
                                            printf("[-] Appointments successfully deleted\n");
                                        }
                                    }
                                } else {
                                    printf("Appointment not found\n");
                                }
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
                    free(result);
                    result = NULL;
                }

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

                plotCurves();
                fgets(buff, 128, stdin);
                printf("\033c");


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
