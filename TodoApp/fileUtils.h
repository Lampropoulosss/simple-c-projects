#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#define MAX_DESC_LENGTH 200

typedef struct todoItem
{
    char description[MAX_DESC_LENGTH];
    int status;
    unsigned long int id;
    struct todoItem *_next;
} todoItem;

void cleanInputBuffer();
void fileSave(todoItem *head);
void fileLoad(todoItem **head);

#endif