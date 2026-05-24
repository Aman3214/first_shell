#include "shell.h"

struct builtin builtins[] = {
    {"cd", sh_cd},
    {"help", sh_help},
    {"exit", sh_exit},
    {"pwd", sh_pwd},
    {"echo", sh_echo},
    {NULL, NULL}
};

//change directory function
int sh_cd(char **args) {
    printf("Changing directory to: %s\n", args[1]);
    if (args[1] == NULL) {
        fprintf(stderr, "Expected argument to \"cd\"\n");
        return 1;
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
            return 1;
        }
    }
    return 0;
}

//help function
int sh_help(char **args) {
    printf("Shell built-in commands:\n");
    printf("  cd [dir] - Change the current directory to 'dir'\n");
    printf("  help - Display this help message\n");
    printf("  pwd - Print the current working directory\n");
    printf("  echo [args] - Print the arguments to the terminal\n");
    printf("  exit - Exit the shell\n");
    return 0;
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
        return 1;
    }
    return 0;
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
        return 0;
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
        return 0;
    }
    else if(strcmp(args[1],"$USER") == 0) {
        char *user = getenv("USER");
        if (user != NULL) {
            printf("%s\n", user);
        } else {
            fprintf(stderr, "USER environment variable not set\n");
            return 1;
        }
        for (int i = 2; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
        return 0;
    }
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s ", args[i]);
    }
    printf("\n");
    return 1;
}

// Execute command function
int execute_command_io(char **args,int input_fd, int output_fd) {
    for(int i = 0; builtins[i].name != NULL; i++) {
        if (strcmp(args[0], builtins[i].name) == 0) {
            if(input_fd != STDIN_FILENO || output_fd != STDOUT_FILENO) {
                pid_t pid = fork();
                if (pid == 0) {
                    if (input_fd != STDIN_FILENO) {
                        dup2(input_fd, STDIN_FILENO);
                        close(input_fd);
                    }
                    if (output_fd != STDOUT_FILENO) {
                        dup2(output_fd, STDOUT_FILENO);
                        close(output_fd);
                    }
                    exit(builtins[i].function(args));
                }
                int status;
                waitpid(pid, &status, 0);
                if (WIFEXITED(status)) {
                    return WEXITSTATUS(status);
                }
            }
            return builtins[i].function(args);
        }
    }
    return launch_external_io(args, input_fd, output_fd);
}
    


int launch_external_io(char **args, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork");
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    }
    return 1;

}