#include "shell.h"

int execute_ast_io(ASTNode *node, int input_fd, int output_fd) {
    if (node == NULL) return 0;
    switch(node->type) {
        case NODE_COMMAND: {
            int in_fd = input_fd;
            int out_fd = output_fd;

            if(node->input_file){
                in_fd = open(node->input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror("open input file");
                    return -1;
                }
            }

            if(node->output_file){
                int flags = O_WRONLY | O_CREAT | (node->append_mode ? O_APPEND : O_TRUNC);
                out_fd = open(node->output_file, flags, 0644);
                if (out_fd < 0) {
                    perror("open output file");
                    if(in_fd != input_fd) close(in_fd);
                    return -1;
                }
            }

            int result = execute_command_io(node->args, in_fd, out_fd);
            if(node->input_file && in_fd != input_fd) close(in_fd);
            if(node->output_file && out_fd != output_fd) close(out_fd);

            return result;
        }

        case NODE_PIPE: {
            int pipe_fd[2] = {0,0};
            if(pipe(pipe_fd) == -1) {
                perror("pipe");
                return -1;
            }
            int left_status = execute_ast_io(node->left, input_fd, pipe_fd[1]);
            close(pipe_fd[1]);
            int right_status = execute_ast_io(node->right, pipe_fd[0], output_fd);
            close(pipe_fd[0]);
            return right_status;
        }

        case NODE_AND: {
            if (execute_ast_io(node->left, input_fd, output_fd) == 0) {
                return execute_ast_io(node->right, input_fd, output_fd);
            }
            return -1;
        }

        case NODE_OR: {
            if (execute_ast_io(node->left, input_fd, output_fd) != 0) {
                return execute_ast_io(node->right, input_fd, output_fd);
            }
            return 0;
        }
    }
    return 0;
}

int execute_ast(ASTNode *node) {
    return execute_ast_io(node, STDIN_FILENO, STDOUT_FILENO);
}