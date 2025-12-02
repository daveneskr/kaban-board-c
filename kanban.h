
#ifndef KANBAN_H
#define KANBAN_H


// Assuming the following struct definitions exist:
typedef struct Item {
    char name[20];  // Assuming a fixed-size name field
    struct Item* next;
} Item;

typedef struct List {
    char name[20];  // Name of the list/list owner
    struct List* next;
    Item* first_item;
} List;

void initial_values();

void main_menu(void);

void displayBoard(void);

void loadBoard(void);

void edit_list(void);

void edit_item(List *list);
void add_item(List *list);
void delete_item(List *list);


void edit_board(void);

void edit_list_name(void);
void add_list(void);
void delete_list(void);

void clear_board(List** board);
void delete_all_items(Item **item);

void saveBoard(void);


#endif //KANBAN_H




