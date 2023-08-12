
typedef struct {
    char* key;
    char* value;
} item;
typedef struct {
    int base_size;
    int size;
    int count;
    item** arr;
} hash_table;

extern int PRIME_1;
extern int PRIME_2;
extern int START_BASE_SIZE;

static item* create_new_item(const char* new_key, const char* new_val);
static hash_table* create_sized_hash_table(const int base_size);
hash_table* create_hash_table();
static void delete_item(item* del_item);
void delete_hash_table(hash_table* del_ht);
void insert(hash_table* ht, const char* key, const char* value);
char* search(hash_table* ht, const char* key);
void delete(hash_table* ht, const char* key);
static void resize(hash_table* ht, const int base_size);
static void resize_up(hash_table* ht);
static void resize_down(hash_table* ht);