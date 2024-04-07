// A collector of arbitrary objects.
struct collector;

// collector_init(obj_size, obj_print) initializes an empty collector that
//   stores an arbitrary objects number of objects, each of size obj_size.
///  The function pointer *obj_print refers to a function that can print the
//   stored objects.
// example: collector_init(sizeof(struct posn), print_posn) creates a collector
//          for storing struct posn.
// effects: allocates memory [client must call collector_collapse]
// time:    O(1)
struct collector *collector_init(int obj_size, void (*obj_print)(const void *));

// collector_insert(col, obj) inserts the object *obj to the back of the
//   collector *col.
// effects:  mutates *col
// time:     O(o), where o is the object size
void collector_insert(struct collector *col, const void *obj);

// collector_collapse(col) releases all resources used by the collector *col,
//   and returns a pointer to an array containing all objects from *col.
// effects: invalidates *col
//          allocates memory [client must free the array and its content]
// time: O(n * o), where n is the number of objects and o is their size
void *collector_collapse(struct collector *col, int *len);

// collector_is_empty(col) returns true if collector *col is empty, and false
//   otherwise.
// time: O(1)
bool collector_is_empty(struct collector *col);

// collector_print(col) prints all the objects in collector *col.
// Note: This function is intended to be used primarily for debugging.
// time: O(n * o), where n is the number of objects and o is their size
void collector_print(const struct collector *col);
