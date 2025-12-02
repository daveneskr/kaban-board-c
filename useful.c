//
// Created by David Neškrabal on 27.03.2025.
//
#include <stdio.h>
#include <stdbool.h>

#include "useful.h"

/* Function used in do-while guards to display
 * error messages
 * - const char *message: string that will be put to stdin
 * Return: always true
 */
bool error_message(const char *message)
{
    printf("%s", message); // print inputted message
    return true; // always return true
}

/* Function to clear buffer
 */
void clear_stdin(void)
{
    int c;
    do {
        // continue reading from stdin until '\n' is found
        c = getchar();
    } while (c != '\n');
}


// return 1 if the list is empty, 0 otherwise
int is_empty(List* head)
{
    return head == NULL;
}
