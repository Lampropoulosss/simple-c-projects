#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fileUtils.h"

#define UIMAX "18446744073709551615"
#define MAX_DESC_LENGTH 200

void addItem(todoItem **head);
void displayItems(todoItem *head);
void markCompleted(todoItem *head);
bool isInputValid(const char *input);
void clearCompleted(todoItem **head);
void updateItem(todoItem *head);

int main()
{
    todoItem *head = NULL;
    char input[2];

    fileLoad(&head);

    do
    {
        input[0] = '0';
        printf("TODO APPLICATION\n\n");
        printf("========================================\n");
        printf("1) Add new item.\n2) Display all items.\n3) Mark an item as completed.\n4) Clear completed.\n5) Update item.\n6) Exit.\n");
        printf("========================================\n\n");

        while (input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4' && input[0] != '5' && input[0] != '6')
        {
            printf("Select an option: ");
            scanf("%1s", input);
            cleanInputBuffer();
        };

        switch (input[0])
        {
        case '1':
            addItem(&head);
            break;
        case '2':
            displayItems(head);
            break;
        case '3':
            markCompleted(head);
            break;
        case '4':
            clearCompleted(&head);
            break;
        case '5':
            updateItem(head);
            break;
        case '6':
            fileSave(head);
            break;
        }
    } while (input[0] != '6');
};

void addItem(todoItem **head)
{
    if (*head == NULL)
    {
        todoItem *newItem = malloc(sizeof(todoItem));
        printf("Give a description: ");
        fgets(newItem->description, sizeof(newItem->description), stdin); // Null-terminates the string automatically.

        unsigned long int len = strcspn(newItem->description, "\n"); // Counts the bytes untill the new-line character(without counting the new-line character).
        newItem->description[len] = '\0';                            // If length less than the max, replaces the new-line character with the null terminator.

        newItem->id = 1;
        newItem->status = 0;
        newItem->_next = NULL;
        *head = newItem;
    }
    else
    {
        // Finding the last node in the list
        todoItem *current = *head;
        todoItem *previous = NULL;

        while (current != NULL)
        {
            previous = current;
            current = current->_next;
        }

        // Creating a new node and linking it to the list
        todoItem *newItem = malloc(sizeof(todoItem));
        printf("Give a description: ");
        fgets(newItem->description, sizeof(newItem->description), stdin);

        unsigned long int len = strcspn(newItem->description, "\n");
        newItem->description[len] = '\0';

        newItem->id = (previous->id + 1);
        newItem->status = 0;
        newItem->_next = NULL;
        previous->_next = newItem;
    }
};

void displayItems(todoItem *head)
{
    if (head == NULL)
    {
        printf("There are no TODO items to display.\n\n");
    }
    else
    {
        todoItem *current = head;

        while (current != NULL)
        {
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("Item: %lu)\n", current->id);
            printf("Description: %s\n", current->description);
            printf("Status: %s\n", current->status == 0 ? "Pending" : "Completed");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

            current = current->_next;
        }
    }
};

bool isInputValid(const char *input)
{
    // Check if the input is empty or contains non-digit characters
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] < '0' || input[i] > '9')
        {
            return false;
        }
    }
    // Check for potential overflow
    if (strlen(input) > 20 || (strlen(input) == 20 && strcmp(input, UIMAX) > 0))
    {
        return false;
    }
    return true;
};

void markCompleted(todoItem *head)
{
    unsigned long int id;
    char input[21]; // 20 characters for input + 1 for null terminator

    printf("Give the ID of the item: ");
    scanf("%20s", input);
    cleanInputBuffer();

    if (isInputValid(input))
    {
        id = strtoul(input, NULL, 10);
        todoItem *current = head;
        todoItem *previous = NULL;

        while (current != NULL && current->id != id)
        {
            previous = current;
            current = current->_next;
        }

        if (current != NULL)
        {
            current->status = 1;
        }
        else
        {
            printf("Could not find the item with the ID: %lu\n\n", id);
        }
    }
    else
    {
        printf("Invalid input. Please enter a valid ID.\n\n");
    }
};

void clearCompleted(todoItem **head)
{
    todoItem *current = *head;
    todoItem *previous = NULL;

    while (current != NULL)
    {
        if (current->status == 1)
        {
            // Check if the first node is completed
            if (previous == NULL)
            {
                *head = current->_next;
                free(current);
                current = *head;
            }
            else
            {
                previous->_next = current->_next;
                free(current);
                current = previous->_next;
            }
        }
        else
        {
            previous = current;
            current = current->_next;
        }
    }

    printf("Finished deleted completed TODO items.\n\n");
};

void updateItem(todoItem *head)
{
    unsigned long int id;

    char input[21]; // 20 characters for input + 1 for null terminator
    char description[MAX_DESC_LENGTH];

    todoItem *current = head;
    todoItem *previous = NULL;

    printf("Give the ID of the item: ");
    scanf("%20s", input);
    cleanInputBuffer();

    if (isInputValid(input))
    {
        id = strtoul(input, NULL, 10);

        while (current != NULL && current->id != id)
        {
            previous = current;
            current = current->_next;
        }

        if (current != NULL)
        {
            printf("Give a new description: ");
            fgets(current->description, sizeof(current->description), stdin); // Null-terminates the string automatically.

            unsigned long int len = strcspn(current->description, "\n"); // Counts the bytes untill the new-line character(without counting the new-line character).
            current->description[len] = '\0';                            // If length less than the max, replaces the new-line character with the null terminator.
        }
        else
        {
            fprintf(stderr, "Could not find an item with the ID: %lu\n", id);
        }
    }
};