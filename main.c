#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"

struct builtin builtins[] = {
    {"cd", sh_cd},
    {"help", sh_help},
    {"exit", sh_exit},
    {NULL, NULL}
};

int main() {

    //main loop
    while(1) {
        //Reading user input and parsing it into command and arguments
        char buffer[50];
        printf("%s>> ",getcwd(NULL, 1024));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 
        // Process the input command
        char *command = strtok(buffer, " ");
        char *args[10];
        args[0] = strtok(NULL, " ");
        int i = 0;
        while (args[i] != NULL && i < 10) {
            args[++i] = strtok(NULL, " ");
        }
        
        //Matching the command with built-in commands
        int num_builtins = sizeof(builtins) / sizeof(struct builtin);
        for(int i = 0; i < num_builtins; i++) {
            if (strcmp(command, builtins[i].name) == 0) {
                builtins[i].function(args);
                break;
            }
        }
    }
    return 0;
}