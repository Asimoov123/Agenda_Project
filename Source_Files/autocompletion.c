#include "autocompletion.h"
#include <stdio.h>
#include <windows.h>


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
char **autocomplete(char input[], t_d_ContactList contactList) {
    // If list is empty
    if (contactList.heads[3] == NULL) {
        return NULL;
    }

    int matchCount = 0, i;
    char *ScannedName = Scan_name(input);

    // Init the matches array
    char **matches = malloc(1024 * sizeof(char *));
    for (i = 0; i < 1024; i++) {
        matches[i] = NULL;
    }

    t_d_contact *currCell = contactList.heads[3];
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
char *autocompleteResearch(t_d_ContactList contactList) {
    // Initialize input arrays
    char input[25], alphabeticalInput[25] = "\0", previousInput[25] = "\0";

    // Allocate memory for the result array
    char **result = malloc(1024 * sizeof(char *));
    for (int i = 0; i < 1024; i++) {
        result[i] = NULL;
    }

    int i = 0, j = 0, k = 0, c = 0, found = 0;

    // Main loop to read user input until Enter key (ASCII 13) is pressed || Esc key (ASCII 27)
    while ((input[i] = getch()) != 13 && input[i] != 27) {
        // Check if the input character is an alphabetical character or a space
        if (('A' <= input[i] && input[i] <= 'Z') || ('a' <= input[i] && input[i] <= 'z') || input[i] == ' ' ||
            input[i] == '_') {
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
                result = autocomplete(alphabeticalInput, contactList);
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
    if (input[i] == 27) {
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