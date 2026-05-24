#include "shell.h"

static ASTNode *create_node(NodeType type){
    ASTNode *node = calloc(1, sizeof(struct ASTNode));
    if(!node) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    return node;
}
static void extract_redirection(ASTNode *node) {
    for (int i = 0; node->args[i] != NULL; i++) {
        if (strcmp(node->args[i], ">") == 0 || strcmp(node->args[i], ">>") == 0) {
            node->append_mode = (strcmp(node->args[i], ">>") == 0)? 1 : 0;
            node->output_file = node->args[i + 1];
            node->args[i] = NULL; 
            break;
        } else if (strcmp(node->args[i], "<") == 0) {
            node->input_file = node->args[i + 1];
            node->args[i] = NULL; 
            break;
        }
    }
}

static ASTNode *parse_command_leaf(char **args) {
    if (args[0] == NULL) return NULL;
    ASTNode *leaf = create_node(NODE_COMMAND);
    leaf->args = args;  
    extract_redirection(leaf);
    return leaf;
}

static ASTNode *parse_pipeline_expression(char **args) {
    int i = 0;
    int split_index = -1;
    while(args[i]!= NULL) {
        if(strcmp(args[i], "|") == 0) {
            split_index = i;
        }
        i++;
    }
    if(split_index != -1) {
        ASTNode *node = create_node(NODE_PIPE);
        args[split_index] = NULL; 
        node->left = parse_pipeline_expression(args);
        node->right = parse_command_leaf(&args[split_index + 1]);
        return node;
    }
    return parse_command_leaf(args);
}

static ASTNode *parse_logical_expression(char **args) {
    int i = 0;
    int split_index = -1;
    NodeType op_type;
    while(args[i]!= NULL) {
        if(strcmp(args[i], "&&") == 0) {
            split_index = i;
            op_type = NODE_AND;
        } else if(strcmp(args[i], "||") == 0) {
            split_index = i;
            op_type = NODE_OR;
        }
        i++;
    }
    if(split_index != -1) {
        ASTNode *node = create_node(op_type);
        args[split_index] = NULL; 
        node->left = parse_logical_expression(args);
        node->right = parse_pipeline_expression(&args[split_index + 1]);
        return node;
    }
    return parse_pipeline_expression(args);
}

ASTNode *parse_input(char **args) {
    return parse_logical_expression(args);
}

void free_ast(ASTNode *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}