#include <stdlib.h>
#include "./helper.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

char **inputToArray(char *command);
int validateCommand(char *command);

int main()
{
    welcome();
    int     doContinue  =   0;
    int     child_output;
    pid_t   child_pid;
    pid_t   wait_pid    = 0;

    while(1) {
        printf("\033[1;32m$ \033[0;37m");

        char *command = malloc(256 * sizeof(char));
        if (!command) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }

        if (fgets(command, 256, stdin) == NULL) {
            free(command);
            continue;
        }
        command[strcspn(command, "\n")] = '\0';

        doContinue = validateCommand(command);
        if (doContinue == 0) {
            free(command);
            continue; 
        } else if (doContinue == -1) {
            free(command);
            break; // exit gracefully
        }

        char **args = inputToArray(command);

        // Here magic happens
        child_pid = fork();
        if (child_pid == 0) {
            execvp(args[0], args);
            perror("failed to execute command");
            exit(EXIT_FAILURE);
        } else if (child_pid < 0) {
            perror("There was an error in forking");
            free(args);
            free(command);
            exit(EXIT_FAILURE);
        } else {
            wait_pid = waitpid(child_pid, &child_output, WUNTRACED);
        }

        free(args);
        free(command);
    }
    return 0;
}

char **inputToArray(char *command) {
    int bufsize = 64; 
    char **args = malloc(bufsize * sizeof(char *));
    if (!args) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    char *token;
    int position = 0;

    token = strtok(command, " ");
    while (token != NULL) {
        args[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            args = realloc(args, bufsize * sizeof(char *));
            if (!args) {
                perror("realloc failed");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " ");
    }
    args[position] = NULL;
    return args;
}

int validateCommand(char *command) {
    if (command == NULL || strlen(command) == 0) {
        return 0;
    } else if (command[0] == '\0') {
        return 0;
    } else if (strcmp(command, "exit") == 0) {
        return -1;
    } else if (strchr(command, ';') != NULL || 
               strchr(command, '&') != NULL || 
               strchr(command, '|') != NULL) {
        printf("Invalid command: Special characters like ';', '&', or '|' are not allowed.\n");
        return 0;
    } else {
        return 1; // valid command
    }
}