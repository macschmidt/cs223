struct coordinate {
	int xCoord;
	int yCoord;
	int zCoord;
};
typedef struct coordinate Coordinate;

// A single ant
struct ant {
    int name;
    Coordinate antPosition;
};
typedef struct ant Ant; // [struct ant] can now be called Ant

typedef struct dict *Dict;

/* create a new empty dictionary */
Dict dictCreate(void);

/* destroy a dictionary */
void dictDestroy(Dict);

/* insert a new key-value pair into an existing dictionary */
void dictInsert(Dict, Coordinate key, int value);

/* return the most recently inserted value associated with a key */
/* or 0 if no matching key is present */
const int dictSearch(Dict, Coordinate key);

/* delete the most recently inserted record with the given key */
/* if there is no such record, has no effect */
void dictDelete(Dict, Coordinate key);

// static unsigned long hashFunction(int x, int y, int z);

// static void grow(Dict d);