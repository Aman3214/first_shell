#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

struct builtin{
    char *name;
    int (*function)(char **args);
};

int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);
int sh_pwd(char **args);
int sh_echo(char **args);
int launch_external(char **args);
#endif