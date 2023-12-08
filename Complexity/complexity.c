#include "complexity.h"
#include "../Source_Files/list.h"
#include "../Source_Files/timer.h"
#include "../Source_Files/agenda.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

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

    FILE *fileNames = fopen("CSV_Files/namesMany.csv", "r");
    if (fileNames == NULL) {
        printf("Can't open file : %s\n", strerror(errno));
        return;
    }

    t_d_ContactList mylist;
    int lines_extracted, randomLine, num_lines_to_extract, current_line = 0, *lines_to_extract;
    char buffer[128], *all_lines[5003], *searchedName;

    char *dataLinear, *dataDichotomous;
    dataLinear = malloc(128 * sizeof(char));
    dataDichotomous = malloc(128 * sizeof(char));
    dataLinear[0] = dataDichotomous[0] = '\0';

    // Read each line from the file
    while (fgets(buffer, 128, fileNames) != NULL) {
        all_lines[current_line] = malloc(25 * sizeof(char));
        all_lines[current_line] = strdup(buffer);
        current_line++;
    }
    fclose(fileNames);

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
        fileNames = fopen("CSV_Files/namesMany.csv", "r");
        if (fileNames == NULL) {
            printf("Can't open file : %s\n", strerror(errno));
            free(lines_to_extract);
            return;
        }

        current_line = 0;
        lines_extracted = 0;

        // Read each line from the file
        while (fgets(buffer, 128, fileNames) != NULL) {
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
        fclose(fileNames);
        free(lines_to_extract);
        lines_to_extract = NULL;

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
        strcat(dataLinear, getTimeAsStringAlt());
        strcat(dataLinear, "\t");

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
        strcat(dataDichotomous, getTimeAsStringAlt());
        strcat(dataDichotomous, "\t");

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
    FILE* fData = fopen("Complexity/Data.txt", "w");
    if (fData == NULL) {
        printf("Can't open file : %s\n", strerror(errno));
        return;
    }
    fprintf(fData, "%s\n", dataLinear);
    fprintf(fData, "%s\n", dataDichotomous);
    fclose(fData);
    free(dataLinear);
    free(dataDichotomous);
    dataLinear =  NULL;
    dataDichotomous = NULL;
    printf("boubou");
}


// M.A. BENALLAL - "GNUPLOT, comment tracer une courbe en C" : https://www.youtube.com/watch?v=iWlYEScSgNc
void plotCurves(){

    char *gnuname = "'C:/Users/romai/CLionProjects/Agenda_Project/Complexity/plotingFile.txt'";
    char *gnupath = "'C:/Program Files/gnuplot/bin/wgnuplot'";
    char command[500];

    char buff[128];


    compareExecutionTime_Contact(10000);

    FILE *f_plot = fopen(gnuname, "w");
    if (f_plot != NULL){
        fprintf(f_plot, "set grid; \n");
        fprintf(f_plot, "set xrange[7:12]; \n");
        fprintf(f_plot, "set yrange[0:2]; \n");
        fprintf(f_plot, "set key box; \n");

        fprintf(f_plot, "set multiplot; \n");
        fprintf(f_plot, "set size 1,1; \n");
        fprintf(f_plot, "set origin 0.0,0.0; \n");

        fprintf(f_plot, "plot 'C:/Users/romai/CLionProjects/Agenda_Project/Complexity/Data.txt' u 1:2 title 'Recherche Linéaire' w lp lt rgb 'grey' lw 2 axes x1y1 ,  ");
        fprintf(f_plot, "     'C:/Users/romai/CLionProjects/Agenda_Project/Complexity/Data.txt' u 1:3 title 'Recherche Dichotomique' w lp lt rgb 'orange' lw 2 axes x1y1 ,  ");

        fprintf(f_plot, "unset multiplot; \n");
        fclose(f_plot);
        sprintf(command, "start %s -persist %s ", gnupath, gnuname);
        system(command);

    }

}