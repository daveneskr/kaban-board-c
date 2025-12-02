//
// Created by David Neškrabal on 27.03.2025.
//


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "user_input.h"
#include "useful.h"

int scan_choice(int from, int to)
{
    unsigned int choice;
    int test;
    // loop until valid input is given
    do {
        test = scanf("%u", &choice); // get user input
        clear_stdin(); // clear stdin from unscanned input
        // repeat loop if scanf failed to read an integer (test != 1)
        // or the response was not 1 or 2
        // and inform the user if input not valid
    } while ((test != 1 || choice < from || choice > to) && error_message("Invalid input!\nEnter again: "));

    return choice; // return valid choice
}

char* get_string(unsigned int const length, char* const question)
{
    if (length > 50)
    {
        puts("Length is greater than the largest allowed length");
        return NULL;
    }
    static char choice[50];
    bool tooLong = false;

    printf("%s", question);

    do
    {
        // read input
        if (fgets(choice, length, stdin) == NULL) {
            // Handle end of file or read error
            error_message("Error reading input!\n");
            choice[0] = '\0';
            return choice;
        }

        size_t len = strlen(choice);
        if (len > 0 && choice[len - 1] == '\n') { // check if the whole string was scanned
            choice[len - 1] = '\0'; // if yes, remove the newline char
        }
        else { // if not, string is too long
            tooLong = true;
            clear_stdin();
        }
    } while (choice[0] == '\0' && error_message("String cannot be empty!\nEnter again: ") ||
            tooLong && error_message("String too long!\n") );


    return choice;
}

