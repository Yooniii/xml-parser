// An XML-tree.
struct xtree;

// tree_create(tokens, len) creates an XML-tree that contains all strings from
//   the array tokens of length len.
// effects: allocates memory [client must call tree_destroy]
struct xtree *tree_create(char **tokens, int len);

// tree_destroy(tr) releases all resources used by XML tree *tr.
// effects: invalidates *tr
void tree_destroy(struct xtree *tr);

// tree_print(tr) prints out the content of XML tree *tr.
// effects: writes to output
void tree_print(struct xtree *tr);
