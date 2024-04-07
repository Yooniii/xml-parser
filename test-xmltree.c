#include <assert.h>
#include <stdlib.h>
#include "tokenizer.h"
#include "xml-tree.h"

#include <stdio.h>
int main(void) {
    int len = 0;
    char **tokens = tokenize_input(&len);
    
    struct xtree *tr = tree_create(tokens, len);

    // Pretty-print the tree.
    
    //tree_print(tr);
    tree_destroy(tr);

    // Clean up token array.
    for (int i = 0; i < len; ++i) {
        free(tokens[i]);
    }
    free(tokens);
}
