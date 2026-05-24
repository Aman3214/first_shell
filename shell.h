#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

struct builtin{
    char *name;
    int (*function)(char **args);
};


int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);
int sh_pwd(char **args);
int sh_echo(char **args);
int launch_external_io(char **args, int input_fd, int output_fd);
int execute_command_io(char **args, int input_fd, int output_fd);

extern struct builtin builtins[];

typedef enum {
    NODE_COMMAND,
    NODE_AND,
    NODE_OR,
    NODE_PIPE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char **args; 
    char *input_file;
    char *output_file;
    int append_mode;

    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *parse_input(char **input);
int execute_ast_io(ASTNode *node, int input_fd, int output_fd);
int execute_ast(ASTNode *node);
void free_ast(ASTNode *node);
#endif