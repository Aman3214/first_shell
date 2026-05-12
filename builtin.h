#ifndef BUILTIN_H
#define BUILTIN_H

struct builtin{
    char *name;
    int (*function)(char **args);
};

int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);

#endif