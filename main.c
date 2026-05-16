#include "shell.h"

struct builtin builtins[] = {
    {"cd", sh_cd},
    {"help", sh_help},
    {"exit", sh_exit},
    {"pwd", sh_pwd},
    {"echo", sh_echo},
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
        char *args[10];
        args[0] = strtok(buffer, " ");
        int i = 0;
        while (args[i] != NULL && i < 9) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL; // Ensure explicit NULL termination for execvp

        // Protect against empty commands
        if (args[0] == NULL) {
            continue;
        }
        
        //Matching the command with built-in commands
        int found = 0;
        for(int i = 0; builtins[i].name != NULL; i++) {
            if (strcmp(args[0], builtins[i].name) == 0) {
                printf("Executing built-in command: %s\n", args[0]);
                builtins[i].function(args);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Command not in builtins: %s\n", args[0]);
            fflush(stdout);
            launch_external(args);
        }
    }
    return 0;
}