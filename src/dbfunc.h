#define TEXT_LEN 63 /* Maximale Textlänge */
#define DATABASE_SIZE 5

struct Data_s
{
    char name[TEXT_LEN];
    char brand[TEXT_LEN];
    char invNr[TEXT_LEN];
    long year;
};

typedef struct Data_s Data_t;

void initItems(Data_t *items, long len);

void printItem(Data_t *item);

void printDB(Data_t *items, long len);

long readValue(char *name);

void readString(char *name, char *text, long len);

long addItem(Data_t *items, long len);

long deleteItem(Data_t *items, long len);

long saveDB(Data_t *items, long len, char *outputFile);

long loadDB(Data_t *items, long len, char *inputFile);

long searchFile(char *inputFile, char *key, long ignoreCase);

void caseToLower(char *str, long len);

long sortItems(Data_t *items, long len, long which);

long addFromFile(Data_t *items, long len, char *inputFile, char *key);
