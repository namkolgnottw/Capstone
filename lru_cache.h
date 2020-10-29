#define HASH_TABLE_SIZE 200
////** data structure **////

struct cache_entry {
  char file_name[100];
  int file_type;
  int file_size;
  char* resource_ptr;  // file ptr in memory
  struct cache_entry* prev;
  struct cache_entry* next;
};

struct hash_entry {
  struct cache_entry* entry;
};

struct lru_linked_list {
  int current_size;
  int max_size;
  struct cache_entry* head;
  struct cache_entry* tail;
  struct hash_entry hash_table[HASH_TABLE_SIZE];
};


////** function interface **////

struct lru_linked_list* cache_create(int max_size, int hash_table_size);

void remove_tail(struct lru_linked_list* cache);

void put_new_entry(struct lru_linked_list* cache, char* resource_ptr, char* file_name, int file_size);

void move_to_head(struct lru_linked_list* cache, struct cache_entry* entry);

struct cache_entry* get_entry(struct lru_linked_list* lru_linked_list0, char* file_name);

int delete_from_hashtable(struct lru_linked_list* lru_linked_list0, char* file_name);

int hash(const char* filename);

int insertToHashTable(struct lru_linked_list* lru_linked_list0, struct cache_entry* entry_ptr);

int fileGetIn(struct lru_linked_list* cache0, char* file_name, int file_size, char* resource_ptr);

