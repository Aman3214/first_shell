#include "shell.h"

struct builtin builtins[] = {
    {"cd", sh_cd},
    {"help", sh_help},
    {"exit", sh_exit},
    {"pwd", sh_pwd},
    {"echo", sh_echo},
    {NULL, NULL}
};

int execute_command(char **args) {
    for(int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            return builtins[i].function(args);
        }
    }
    return launch_external(args);
}
int main() {

    //main loop
    while(1) {
        //Reading user input and parsing it into command and arguments
        char buffer[1024];
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
        //processing && and || case for multiple commands
        i=0;
        int start_idx = 0;
        int execute_next = 1;
        while(args[i] != NULL) {
            
            if(strcmp(args[i], "&&") == 0) {
                args[i] = NULL; 
                if (execute_next) {
                    if(execute_command(&args[start_idx]) != 0) {
                        execute_next = 0; 
                    }
                }
                start_idx = i + 1;
                i = 0;
                continue;
            }

            else if(strcmp(args[i], "||") == 0) {
                args[i] = NULL; 
                if (execute_next) {
                    if(execute_command(&args[start_idx]) == 0) {
                        execute_next = 0; 
                    }
                }
                start_idx = i + 1;
                i = 0;
                continue;
            }
            i++;
        }
        if (execute_next && args[start_idx] != NULL) {
            execute_command(&args[start_idx]);
        }
    }
    return 0;
}