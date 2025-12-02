#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "kanban.h"
#include "useful.h"
#include "user_input.h"

List *board;

void main_menu(void)
{
    unsigned int choice; // used to store user's choice

    do // continue displaying main menu
    {
        puts("\nMenu:\n"
            "1. Display board\n"
            "2. Load board from a file\n"
            "3. Edit list\n"
            "4. Edit board\n"
            "5. Save board to file\n"
            "6. Quit");

        printf("Enter choice: ");
        choice = scan_choice(1, 6); // get user input

        switch (choice)
        {
        case 1:
            displayBoard(); // displays the board, with all the lists and their items
            break;
        case 2:
            loadBoard();// loads a board from a .dat file
            break;
        case 3:
            edit_list(); // lets user edit list's items
            break;
        case 4:
            edit_board(); //  lets user edit boards
            break;
        case 5:
            saveBoard(); // saves current board into a .dat file
            break;
        }
     } while (choice != 6); //Keep running program if it doesn't = 6
                            //if choice == 6, exit program
}

void initial_values()
{

    // Detailed list and item creation
    List* list1 = malloc(sizeof(List));
    strcpy(list1->name, "Abey");

    List* list2 = malloc(sizeof(List));
    strcpy(list2->name, "Dante");

    List* list3 = malloc(sizeof(List));
    strcpy(list3->name, "Tim");

    List* list4 = malloc(sizeof(List));
    strcpy(list4->name, "Nick");

    // Create items for list1
    Item* item1_1 = malloc(sizeof(Item));
    strcpy(item1_1->name, "Oculus Pro");

    Item* item1_2 = malloc(sizeof(Item));
    strcpy(item1_2->name, "Oculus Quest 1");

    // Create items for list2
    Item* item2_1 = malloc(sizeof(Item));
    strcpy(item2_1->name, "Oculus Quest 1");

    Item* item2_2 = malloc(sizeof(Item));
    strcpy(item2_2->name, "3070 RTX");

    // Create items for list3
    Item* item3_1 = malloc(sizeof(Item));
    strcpy(item3_1->name, "Oculus Quest 2");

    // Create items for list4
    Item* item4_1 = malloc(sizeof(Item));
    strcpy(item4_1->name, "3070 RTX");

    // Link items within lists
    // List 1 items
    item1_1->next = item1_2;
    item1_2->next = NULL;
    list1->first_item = item1_1;

    // List 2 items
    item2_1->next = item2_2;
    item2_2->next = NULL;
    list2->first_item = item2_1;

    // List 3 items
    item3_1->next = NULL;
    list3->first_item = item3_1;

    // List 4 items
    item4_1->next = NULL;
    list4->first_item = item4_1;

    // Link lists together sequentially
    list1->next = list2;
    list2->next = list3;
    list3->next = list4;
    list4->next = NULL;

    // Set the board to start at list1
    board = list1;
}

/***** DISPLAY BOARD ********/

// Function to print all lists and their items
void displayBoard(void)
{
    // check if board is empty
    if (is_empty(board)) { // if yes, inform user
        puts("Board is empty! Nothing to print.");
        return; // exit early
    }

    List* current = board; // make current point to first list

    // move through the linked list
    while (current != NULL) {
        // print name of each list
        printf("%s\n", current->name);

        // set currentItem to the first item of the list
        Item* currentItem = current->first_item;

        // move through the linked list
        while (currentItem != NULL) {
            // print each item indented
            printf(" - %s\n", currentItem->name);
            // move to next item
            currentItem = currentItem->next;
        }

        // move to next list
        current = current->next;
    }
}

/***** LOAD BOARD ********/

//loadBoard loads last saved board from file  
void loadBoard(void)
{
    char filename[30];
    //max 30 characters
    strcpy(filename, get_string(30, "Enter filename: "));

    //strrchr checks for the last occurence of a character
    char *file_type = strrchr(filename, '.');
    if (file_type == NULL || strcmp(file_type, ".dat") != 0) // ensure the file is a .dat file
    {
        printf("Error! Not a file type of .dat");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        puts("Error could not open file");
        return;

    }

    // Clear existing board first
    clear_board(&board);

    char line[100];
     List *lastList = NULL; // Pointer to keep track of the last list added

    // Read file line by line
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character from end of line
        line[strcspn(line, "\n")] = 0;

        // If line doesn't start with a space, it marks a new list
        if (line[0] != ' ')
        {
            // Allocate memory for a new list
            List *newList = malloc(sizeof(List));
            if (newList == NULL)
            {
                puts("Error: Memory allocation failed!");
                fclose(file); // Close file before exiting
                return;
            }

            // Copy the list name onto the line, then initalise pointers
            strcpy(newList->name, line);
            newList->first_item = NULL;
            newList->next = NULL;

            // If board is empty, start with newList
            if (board == NULL)
            {
                board = newList;
            }
            else
            {
                lastList->next = newList; // Link the new list to the last one
            }

            lastList = newList; // Update lastList to point to the new one
        }
        // If second character is '-', it's an item (starts with " - item")
        else if (line[1] == '-')
        {
            // If there's no list yet, it's an error
            if (lastList == NULL)
            {
                puts("ERROR: Found item before any list.");
                fclose(file);
                return;
            }

            // Allocate memory for a new item
            Item *newItem = malloc(sizeof(Item));
            if (newItem == NULL)
            {
                puts("Memory allocation failed!");
                fclose(file);
                return;
            }
            newItem->next = NULL;

            // Copy item name, skipping the " - " (3 characters)
            strcpy(newItem->name, line + 3);

            // Add item to the end of the current list's items
            if (lastList->first_item == NULL)
            {
                lastList->first_item = newItem; // First item in the list
            }
            else
            {
                // Go to the end of the item list and append
                Item *currItem = lastList->first_item;
                while (currItem->next != NULL)
                {
                    currItem = currItem->next;
                }
                currItem->next = newItem; //Add new item at the end
            }
        }
    }
    fclose(file);
    puts("Board loaded successfully.\n");
}

/***** EDIT BOARD ********/

/* Function that lets the user edit the items of a list
 */
void edit_list(void)
{
    // prompts user to give a list name that is going to be edited
    char *name = get_string(20, "Enter the list to edit: ");
    bool found = false; // used to check is nodes where found
    List *current = board; // set current to the first node

    // loop through list, looking for a match
    while (current != NULL)
    {
        // check if name of list matches name
        if (strcmp(current->name, name) == 0)
        {
            found = true; // mark as found
            break; // exit loop if match found
        }
        // move to next node
        current = current->next;
    }

    if (!found) { // if not found, list does not exist
        puts("No list with such name found.");
        return;
    }

    unsigned int choice; // used to save user's choice

    do // loop until user chooses to stop editing the list
    {
        // display user's options
        puts("\nOptions:\n"
        "1. Edit an item\n"
        "2. Add a new item\n"
        "3. Delete an item\n"
        "4. Return to main menu");

        printf("Enter choice: ");
        choice = scan_choice(1, 4); // scan user's choice

        switch (choice)
        {
        case 1:
            edit_item(current); // lets user edit item names
            break;
        case 2:
            add_item(current); // lets user add item at the end of the list
            break;
        case 3:
            delete_item(current); // lets user delete an item
            break;
        }
    } while (choice != 4); // if choice == 4, exit function
}

/* Function to rename an item of a list
 * - List *list: The list of which the items you want to edit
 */
void edit_item(List *list)
{
    // check if list empty
    if (list->first_item != NULL) {
        // if not, continue

        // get the name of the item to edit from user
        char *name = get_string(20, "Enter the name of the item to edit: ");
        bool found = false; // used to flag when the right item is found
        Item *current = list->first_item; // set current to the first item

        // loop through list, looking for a match
        while (current != NULL)
        {
            // check if name of list matches name
            if (strcmp(current->name, name) == 0) {
                // rename the item
                strcpy(current->name, get_string(20, "Enter the new name: "));
                found = true; // mark as found
                break; // exit loop if match found
            }
            // move to next node
            current = current->next;
        }

        if (!found) { // if nothing found inform user
            puts("No list with such name found.");
        }
    }
    else { // if list is empty{
        puts("List has no items.");
    }
}

/* Function that lets user add an item to the end of the list
 * - List *list: The list where to add the item
 */
void add_item(List *list)
{
    // make a new node
    Item *newItem = malloc(sizeof(Item));
    if (newItem == NULL) { // test for correct mem alloc
        puts("Memory allocation failed!");
        return;
    }
    newItem->next = NULL; // initialize the next pointer
    // get name of the item
    strcpy(newItem->name, get_string(20,"Enter the new name of the item: "));

    // check if list empty
    if (list->first_item == NULL) {
        list->first_item = newItem; // if yes, set the item entered as first
        return; // exit function early
    }

    // set current to the first item
    Item* current = list->first_item;

    // move to the end of the list
    while (current->next != NULL)
    {
        current = current->next;
    }
    // once at end, link new node
    current->next = newItem;
}

/* Function that deletes an item from the linked list
 * - List *list: The list of which the items you want to delete
 */
void delete_item(List *list)
{
    // check if list empty
    if (list->first_item != NULL)
    {
        // if not, continue

        // get item name from user
        char *name = get_string(20, "Enter the name of the item to delete: ");
        bool found = false; // used to flag when the right item is found

        Item *current = list->first_item; // set current to the first item
        Item *prev = NULL; // set up prev pointer

        // if the first item is a match, enter if-statement
        if (strcmp(list->first_item->name, name) == 0)
        {
            // link the second item to the list
            list->first_item = list->first_item->next;
            // free current item
            free(current);
            found = true; // mark as found
        }

        // if not found yet, loop through list, looking for a match
        while (!found && current != NULL)
        {
            // check if name of list matches name
            if (strcmp(current->name, name) == 0)
            {
                // link previous with next
                prev->next = current->next;
                free(current); // free current item
                found = true; // mark as found
                break; // exit loop if match found
            }
            // move to next node
            prev = current;
            current = current->next;
        }

        if (!found) { // if still not found, item is not in the list
            puts("No list with such name found.");
            return;
        }
    }
    else { // if list is empty
        puts("List has no items.");
        return;
    }
    puts("Item deleted."); // inform user of a successful deletion
}

/***** EDIT BOARD ********/

/* Function that lets user edit the board
 */
void edit_board(void)
{
    unsigned int choice; // used to store user input

    do // loop until, exited (aka. 4 is entered)
    {
        // display options
        puts("\nOptions:\n"
        "1. Edit the name of a list\n"
        "2. Add a new list\n"
        "3. Delete a list\n"
        "4. Return to main menu");

        printf("Enter choice: ");
        choice = scan_choice(1, 4);

        switch (choice)
        {
        case 1:
            edit_list_name(); // edits list specified by user
            break;
        case 2:
            add_list(); // adds a list at the end of linked list
            break;
        case 3:
            delete_list(); // deletes list specified by user
            break;
        }
    } while (choice != 4); // if choice == 4, exit function
}

/* Function that lets user edit a list's name
 */
void edit_list_name(void)
{
    char *choice = get_string(20, "Enter the list name to change: ");
    bool found = false;
    List *current = board;

    // loop through list, looking for a match
    while (current != NULL)
    {
        // check if name of list matches choice
        if (strcmp(current->name, choice) == 0)
        {
            strcpy(current->name, get_string(20, "Enter the new name: "));
            found = true;
            break; // exit loop if match found
        }
        // move to next node
        current = current->next;
    }

    if (!found) { // if still not found, list doesn't exits
        puts("No list with such name found."); // inform user
    }
}

/* Lets user add a new list
 */
void add_list(void)
{
    // make a new node
    List *newList = malloc(sizeof(List));
    if (newList == NULL) { // test for correct mem alloc
        puts("Memory allocation failed!");
        return;
    }
    newList->next = NULL; // initialize the next pointer
    // get name of the list
    strcpy(newList->name, get_string(20, "Enter the new name of the list: "));

    // check if board empty
    if (board == NULL) { // if yes, set board to new
        board = newList;
        return; // exit early
    }

    // set current to the beginning of the list
    List* current = board;
    // move to the end of the list
    while (current->next != NULL)
    {
        current = current->next;
    } // once at end, link new node
    current->next = newList;
}

/* Function that lets user delete a list and all its items
 */
void delete_list(void)
{
    // check if board is empty
    if (is_empty(board)) { // if yes, inform user
        puts("Board is empty! Nothing to delete.");
        return; //exit early
    }

    // get the list name to delete from user
    char *choice = get_string(20, "Enter the name of the list to delete: ");
    List *current = board; // set current to the beginning of the linked list
    List *prev = NULL; // set up prev pointer
    bool found = false; // used to flag if list found

    // check if the first list is a match
    if (strcmp(choice, current->name) == 0) { // if yes
        delete_all_items(&current->first_item); // delete all items in the list
        board = current->next; // move head to next node
        free(current); // delete the list
        found = true; // mark list as found
        return; // exit early
    }

    // move through the linked list, looking for a match
    while (!found && current != NULL)
    {
        // if mathc found
        if (strcmp(current->name, choice) == 0) {
            delete_all_items(&current->first_item); // delete all items in the list

            prev->next = current->next; // link the prev with next
            free(current); // delete list
            return;
        }

        // move through list
        prev = current;
        current = current->next;
    }
    puts("No such list found."); // if the list doesn't exist, inform user
}

// Clear the entire board structure
void clear_board(List** board)
{
    while (*board != NULL) {
        List* temp = *board;
        delete_all_items(&(*board)->first_item);
        *board = (*board)->next;
        free(temp);
    }
}

/* Function that deletes all the items in the list
 * - Item **item: Memory address of the first item
 */
void delete_all_items(Item **item)
{
    // if there are no items, exit
    if (*item == NULL) {
        return;
    }
    // set up pointers
    Item *current = *item;
    Item *next = NULL;

    // move through the list and free each item
    while (current->next != NULL)
    {
        next = current->next; // save next item
        free(current); // free memory
        current = next; // set next as current for the next iteration
    }
    // set the original pointer to NULL to indicate no items
    *item = NULL;
}

/***** CLEAR BOARD ********/

// Opens a file to write into it
void saveBoard(void)
{
    char filename[30]; // string to store filename

    // get file name from user
    strcpy(filename, get_string(30, "Enter filename: "));

    // check if file is .dat
    char *file_type = strrchr(filename, '.');
    if (file_type == NULL || strcmp(file_type, ".dat") != 0)
    { // if not, inform and exit function
        printf("Error! Not a file type of .dat");
        return;
    }

    // open file in "write" mode
    FILE *file = fopen(filename,"w");
    if (file == NULL) // check if filed opened successfully
    {
        printf("Error opening file");
        return;
    }

    List *currList = board; // set up current pointer

    // loop through the list
    while (currList != NULL)
    {
        // write list name into file
        fprintf(file, "%s\n" , currList->name);
        Item *currItem = currList->first_item; // set currItem to current lists' first item
        while(currItem != NULL) // if not null loop through the list of items
        {
            fprintf(file, " - %s\n", currItem->name); // print each name with ident
            currItem = currItem->next; // move to next item
        }

        currList = currList->next; // move to next time
    }
    fclose(file); // close file for safety
    printf("Saved file successfully\n"); // inform user of a successful save
}
