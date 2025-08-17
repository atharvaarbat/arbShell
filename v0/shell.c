#include <stdlib.h>
#include "./helper.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

char **inputToArray(char* command);

int main(){
    welcome();
    int status = 1;

    int child_output;
    pid_t child_pid;
    pid_t wait_pid = 0;
    do{
        printf("\033[1;32m$ \033[0;37m");
        char *command = malloc(256 * sizeof(char));
        fgets(command, 256, stdin);
        command[strcspn(command, "\n")] = '\0';
        
        char **args = inputToArray(command);
        
        child_pid = fork();
        if(child_pid == 0){
            execvp(args[0], args);
            perror("failed to execute command");
            exit(EXIT_FAILURE);
        }else if(child_pid < 0){
            perror("There was an error in forking");
            free(command);
            free(args);
            exit(EXIT_FAILURE);
        }else{
            wait_pid = waitpid(child_pid, &child_output, WUNTRACED);
        }
        free(command);
        free(args);
    }while(status);
    return 0;
}

char **inputToArray(char* command) {

    char **args = malloc(64 * sizeof(char*));
    char* seprator = " ";
    char *token;
    int position = 0;
    
    token = strtok(command, seprator);
    while (token != NULL) {
        args[position] = token;
        position++;

        if (position >= 64) {
            args = realloc(args, sizeof(char*) * 64);
        }
        token = strtok(NULL, " ");
    }
    args[position] = NULL;
    return args;

}