#include "shell.h"

//change directory function
int sh_cd(char **args) {
    printf("Changing directory to: %s\n", args[1]);
    if (args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
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
    printf("  pwd - Print the current working directory\n");
    printf("  echo [args] - Print the arguments to the terminal\n");
    printf("  exit - Exit the shell\n");
    return 1;
}

//exit function
int sh_exit(char **args) {
    exit(0);
    return 0;
}

//present working directory function
int sh_pwd(char **args) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd");
    }
    return 1;
}

//echo function
int sh_echo(char **args) {
    if(args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"echo\"\n");
        return 1;
    }
    else if(strcmp(args[1], "-n") == 0) {
        for (int i = 2; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        return 1;
    }
    else if(strcmp(args[1], "-e") == 0) {
        for(int i = 2; args[i] != NULL; i++) {
            char *arg = args[i];
            while (*arg) {
                if (*arg == '\\') {
                    arg++;
                    if (*arg == 'n') {
                        printf("\n");
                    } else if (*arg == 't') {
                        printf("\t");
                    } else {
                        printf("\\%c", *arg);
                    }
                } else {
                    putchar(*arg);
                }
                arg++;
            }
            printf(" ");
        }
        printf("\n");
        return 1;
    }
    else if(strcmp(args[1],"$USER") == 0) {
        char *user = getenv("USER");
        if (user != NULL) {
            printf("%s\n", user);
        } else {
            fprintf(stderr, "USER environment variable not set\n");
        }
        for (int i = 2; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
        return 1;
    }
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

int launch_external(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("fork");
    } else {
        // Parent process
        wait(NULL);
    }
    return 1;
}