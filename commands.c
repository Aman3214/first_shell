#include<unistd.h>
#include <stdio.h>
#include "builtin.h"

//change directory function
int sh_cd(char **args) {
    printf("Changing directory to: %s\n", args[0]);
    if (args[0] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    } else {
        if (chdir(args[0]) != 0) {
            perror("cd");
        }
    }
    return 1;
}

//help function
int sh_help(char **args) {
    printf("Shell built-in commands:\n");
    printf("  cd [dir] - Change the current directory to 'dir'\n");
    printf("  help - Display this help message\n");
    printf("  exit - Exit the shell\n");
    return 1;
}

//exit function
int sh_exit(char **args) {
    return 0;
}