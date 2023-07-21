#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

int main()
{
    printf("Give the amount of characters you'd like: ");
    unsigned long long passLength;

    if (scanf("%llu", &passLength) != 1)
    {
        fprintf(stderr, "Error: The input is invalid. The password length is being set to default(8 characters).\n");
        passLength = 8;
    }

    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Error: Could not create pipe.\n");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid == -1)
    {
        fprintf(stderr, "Error: Could not create child process.\n");
        close(fd[0]);
        close(fd[1]);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // Child Process
        close(fd[0]); // Close read pipe.

        const char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$\%^&*_-+=|~.,:;!?";
        char *password = malloc(sizeof(char) * (passLength + 1));
        srand(time(NULL));

        if (password == NULL)
        {
            fprintf(stderr, "Error: Could not allocate memory for the password.");
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < passLength; i++)
        {
            password[i] = charset[rand() % 82];
        }
        password[passLength] = '\0';

        // The 'password' variable is a pointer so no need for passing it by reference
        write(fd[1], password, sizeof(char) * (passLength + 1));

        close(fd[1]);
        free(password);
    }
    else
    {
        // Parent Process
        close(fd[1]); // Close read pipe.

        char *password = malloc(sizeof(char) * (passLength + 1));

        if (password == NULL)
        {
            fprintf(stderr, "Error: Could not allocate memory for the password.");
            exit(EXIT_FAILURE);
        }

        // The 'password' variable is a pointer so no need for passing it by reference
        int status = read(fd[0], password, sizeof(char) * (passLength + 1));

        if (status == 0)
        {
            fprintf(stderr, "Error: Read end of the pipe closed unexpectedly.\n");
            free(password);
            close(fd[0]);

            exit(EXIT_FAILURE);
        }
        else if (status == -1)
        {
            fprintf(stderr, "Error: Could not read from pipe.\n");
            free(password);
            close(fd[0]);

            exit(EXIT_FAILURE);
        }

        printf("PASSWORD: %s\n", password);

        free(password);
        close(fd[0]);
    }

    return 0;
}
