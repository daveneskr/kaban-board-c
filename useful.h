//
// Created by David Neškrabal on 27.03.2025.
//

#ifndef USEFUL_H
#define USEFUL_H

#include <stdbool.h>
#include "kanban.h"

/* Function used in do-while guards to display
 * error messages
 * - const char *message: string that will be put to stdin
 * Return: always true
 */
bool error_message(const char *message);

/* Function to clear buffer
 */
void clear_stdin(void);

int is_empty(List *head);

#endif //USEFUL_H
