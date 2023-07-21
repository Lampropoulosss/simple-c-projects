#include "fileUtils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void cleanInputBuffer()
{
    long long int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Discard characters
    }
};

void fileSave(todoItem *head)
{
    FILE *file = fopen("todo_list.csv", "w");

    if (file == NULL)
    {
        printf("Failed to save the file!\n\n");
    }
    else
    {
        fprintf(file, "Description,Status,ID\n");

        todoItem *current = head;

        while (current != NULL)
        {
            // Write the fields of each node separated by commas
            fprintf(file, "%s,%d,%lu\n", current->description, current->status, current->id);

            current = current->_next;
        }

        fclose(file);
        printf("TODO list saved.\n\n");
    }
};

void fileLoad(todoItem **head)
{
    FILE *file = fopen("todo_list.csv", "r");

    if (file == NULL)
    {
        printf("Failed to load the TODO list. If this is the first time you run the app, ignore this\n\n");
    }
    else
    {
        char line[256];
        fgets(line, sizeof(line), file); // Read and discard the header line

        while (fgets(line, sizeof(line), file))
        {
            // Parse the line to extract the fields
            char *description = strtok(line, ",");
            char *statusStr = strtok(NULL, ",");
            char *idStr = strtok(NULL, ",");

            if (description != NULL && statusStr != NULL && idStr != NULL)
            {
                // Create a new todoItem node
                todoItem *newItem = malloc(sizeof(todoItem));
                strncpy(newItem->description, description, MAX_DESC_LENGTH);
                newItem->status = atoi(statusStr);
                newItem->id = strtoul(idStr, NULL, 10);
                newItem->_next = NULL;

                // Link the new node to the list
                if (*head == NULL)
                {
                    *head = newItem;
                }
                else
                {
                    todoItem *current = *head;
                    while (current->_next != NULL)
                    {
                        current = current->_next;
                    }
                    current->_next = newItem;
                }
            }
        }

        fclose(file);
        printf("TODO list loaded.\n\n");
    }
};