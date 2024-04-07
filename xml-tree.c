
#include "xml-tree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "collector.h"
//#include "cs136-trace.h"
#include <string.h>


// Indicates that an XML-node is a text node, i.e., a leaf.
const int TEXT_NODE = -1;

// The label for the root node of the XML tree.
const char *ROOT_TAG =  "<%%root%%>";

// === XML-Tree Nodes =========================================================

// A node in an XML-tree.
struct xnode {
    const char *label;       // token stored in this node
    struct xnode **children; // array of children
    int children_len;        // length of children array, TEXT_NODE if leaf
};

// node_print(v) prints out the xnode *v.
// effects: writes to output
void node_print(const void *v) {
    assert(v);
    const struct xnode *xn = v;
    printf("[%s: %d]\n", xn->label, xn->children_len);
}

// node_make(lbl, tokens, len) creates a subtree from *tokens with lbl as the
//   label for the root node. The labels for nodes in the subtree are generated
//   by processing strings in *tokens, an array of length *len. The function
//   advances *tokens and decrements *len by the number of tokens processed.
// effects: allocates memory [client must call node_destroy]
//          mutates **tokens
//          mutates *len
struct xnode *node_make(const char *label, char ***tokens, int *len) {
    assert(label);
    assert(len);
    assert(tokens);

    struct xnode *root = malloc(sizeof(struct xnode));
    root->label = malloc(strlen(label) + 1);
    strcpy((char *) root->label, label);
    struct collector *col = collector_init(sizeof(struct xnode *), node_print);

    while (*len > 0) {
        struct xnode *to_add = malloc(sizeof(struct xnode));
        to_add->label = malloc(strlen(**tokens) + 1);
        strcpy((char *) to_add->label, **tokens);
        //printf("%s", to_add->label);

        if (strncmp(**tokens, "<", 1) == 0 && strncmp(**tokens + 1, "/", 1) != 0) { // Markup tag
            char *start_tag_name = malloc(strlen(**tokens) + 1);
            strcpy(start_tag_name, (**tokens + 1));
            char *end_tag = malloc(strlen(start_tag_name) + strlen("</") + 1);
            strcpy(end_tag, "</");
            strcat(end_tag, start_tag_name);
            printf("%s \n%s\n", **tokens, end_tag);

            // loop to process nested markup tags

            // while (strcmp(**tokens, end_tag) != 0) {
                //collector_insert(col, node_make(**tokens, tokens, len));
               // printf("%s", **tokens);
            //     (*tokens)++;
            //     (*len)--;
            // }
            
            to_add->children = NULL;
            to_add->children_len = TEXT_NODE;
            collector_insert(col, to_add);
            free(start_tag_name);
            free(end_tag);
            
        } else if (strncmp(**tokens + 1, "/", 1) != 0){
            printf("CONTENT: %s\n", **tokens);
            to_add->children = NULL;
            to_add->children_len = TEXT_NODE;
            collector_insert(col, to_add);
        }

        free((void *)to_add->label);
        free(to_add);
        (*tokens)++;
        (*len)--;
    }

    int length = 0;
    struct xnode **temp_arr = collector_collapse(col, &length);
    root->children = malloc(sizeof(struct xnode *) * length);
    root->children = temp_arr;
    //memcpy(root->children, temp_arr, (sizeof(struct xnode *) * length));
    root->children_len = length;
    printf("%d \n", root->children_len);

    // testing to see if the data was copied properly
    // but get heap buffer overflow here
    for (int i = 0; i < root->children_len; ++i) {
        printf("%d", temp_arr[i]->children_len);
    }

    free(temp_arr);
    // free(root->children);
    // free((void *) root->label);
    // free(root);
    return root;
}



// node_destroy(xn) releases all resources use by XML-node *xn, including its
//   children.
// effects: invalidates *xn
void node_destroy(struct xnode *xn) {
    assert(xn);

    // if (xn->children == NULL) { // get heap buffer overflow here
    //     free((void * ) xn->label);
    //     //free(xn->children);
    // } else if (xn->children) {

    //     struct xnode *current = *(xn->children);

    //     while (current) {
    //         struct xnode *to_destroy = current;
    //         current = *(xn->children)++;
    //         node_destroy(to_destroy);
    //     }
    //     free(xn->children);
    //     free((void * ) xn->label);
    // }
    // free(xn); 
}


// === XML-Tree ===============================================================

// See documentation in tree.h.
struct xtree {
    struct xnode *root; // root of the XML-tree
};

// See documentation in tree.h.
struct xtree *tree_create(char **tokens, int len) {
    assert(tokens);
    struct xtree *tr = malloc(sizeof(struct xtree));  
    assert(tr); 
    if (len == 0) {
         tr->root = NULL;
    } else {
        tr->root = node_make(ROOT_TAG, &tokens, &len);
    }
    return tr;
}

// See documentation in tree.h.
void tree_destroy(struct xtree *tr) {
    assert(tr);
    if (tr->root != NULL) {
        node_destroy(tr->root);
    }
    free(tr);
}

void tree_print_wrker(struct xnode *xn) {
    assert(xn);
    printf("%s", xn->label);

    // if (xn->children_len == TEXT_NODE) { // content
    //     printf("\t %s", xn->label);
    // } else {
    //     printf("%s", xn->label);
    //     for (int i = 0; i < xn->children_len; ++i) {
    //         tree_print_wrker(xn->children[i]);
    //     }
    // }
}

// See documentation in tree.h.
void tree_print(struct xtree *tr) {
    assert(tr);
    if (tr->root != NULL) {
        printf("%s", (tr->root->children[0]->label));
        // for (int i = 0; i < tr->root->children_len; ++i) {
        //     tree_print_wrker(tr->root->children[i]);
        // }
    } else {
        printf("%s\n</%s\n", ROOT_TAG, ROOT_TAG + 1);
    }
}
