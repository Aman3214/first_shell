#include "shell.h"
#define MAX_ARGS 128
#define BUFFER_SIZE 1024

int main() {

    //main loop
    while(1) {
        //Reading user input and parsing it into command and arguments
        char buffer[BUFFER_SIZE];
        printf("%s>> ",getcwd(NULL, 1024));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0; 
        // Process the input command
        char *args[MAX_ARGS];
        args[0] = strtok(buffer, " ");
        int i = 0;
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        args[i] = NULL; // Ensure explicit NULL termination for execvp
        
        //using AST to handle command execution with support for &&, ||, and | operators
        ASTNode *root = parse_input(args);
        if (root) {
            execute_ast(root);
            free_ast(root);
        }
    }
    return 0;
}