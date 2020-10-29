#include "commom.h"
#include "lru_cache.h"

void update_expired_entry(struct cache_entry* entry) {
  
}

struct lru_linked_list* cache_create(int max_size, int hash_table_size) {
  struct lru_linked_list* new_cache = (struct lru_linked_list*)malloc( sizeof(struct lru_linked_list) );
  new_cache->max_size = max_size;
  new_cache->current_size = 0;
  new_cache->head = NULL;
  new_cache->tail = NULL;
  //(struct hash_entry*)malloc(sizeof(struct hash_entry) * hash_table_size);
  return new_cache;
}


void remove_tail(struct lru_linked_list* cache) {

  struct cache_entry* tmp_ptr = cache->tail->prev;
  cache->tail = tmp_ptr;
  delete_from_hashtable(cache, tmp_ptr->next->file_name);
  free(tmp_ptr->next->resource_ptr);
  free(tmp_ptr->next);

  cache->tail->next = NULL;
  cache->current_size--;
}


void put_new_entry(struct lru_linked_list* cache, char* resource_ptr, char* file_name, int file_size) {

  struct cache_entry* tmp_ptr;

  if (cache->head != NULL) {

    tmp_ptr = cache->head;
    cache->head = (struct cache_entry*)malloc( sizeof(struct cache_entry) ); 

    cache->head->file_size = file_size;
    cache->head->resource_ptr = resource_ptr;
    strcpy(cache->head->file_name, file_name);
    cache->current_size++;
    insertToHashTable(cache, cache->head);

    if (cache->current_size > cache->max_size)
      remove_tail(cache);

    cache->head->next = tmp_ptr;
    cache->head->prev = NULL;

    tmp_ptr->prev = cache->head;

  } else {  // only one node, is head also is tail 

    cache->head = (struct cache_entry*)malloc( sizeof(struct cache_entry) ); 
    cache->tail = cache->head;
    cache->head->file_size = file_size;
    cache->head->resource_ptr = resource_ptr;
    strcpy(cache->head->file_name, file_name);
    cache->current_size++;
    insertToHashTable(cache, cache->head);

  }
}

void move_to_head(struct lru_linked_list* cache, struct cache_entry* entry) {
  struct cache_entry* tmp_entry;
  if ( cache->head == entry) {

    return;
  }

  else if ( cache->tail == entry) {

    entry->prev->next = NULL;
    cache->tail = entry->prev;
    
    entry->prev = NULL;
    entry->next = cache->head;
    cache->head = entry;    

  } 

  else if ( (cache->tail != entry)  && (cache->head != entry) ) {
 
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    
    entry->prev = NULL;
    entry->next = cache->head;
    cache->head = entry;
  }

}

struct cache_entry* get_entry(struct lru_linked_list* lru_linked_list0, char* file_name) {
  
  struct hash_entry* hash_table0 = lru_linked_list0->hash_table;
  int hash_val = hash(file_name);
  int index = hash_val;

  while (hash_table0[index].entry != NULL) {

    if (strcmp(file_name, hash_table0[index].entry->file_name)==0 ) {

    printf("file get from hash table[%d] \n", index);
      return hash_table0[index].entry;
    }
     
    else {
      if (index == (HASH_TABLE_SIZE-1) ) index = 0;
        index++;
    }
  }
  return NULL;
};

int delete_from_hashtable(struct lru_linked_list* lru_linked_list0, char* file_name) {

  struct hash_entry* hash_table0 = lru_linked_list0->hash_table;
  int hash_val = hash(file_name);
  int index = hash_val;

  while (hash_table0[index].entry != NULL) {

    if (strcmp(file_name, hash_table0[index].entry->file_name)==0 ) {

      hash_table0[index].entry = NULL;
      return 0;
    }
     
    else {
      if (index == (HASH_TABLE_SIZE-1) ) index = 0;
        index++;
    }
  }
  return -1;
}

int hash(const char* filename) {
  int i;
  int hash_val = 0;
  int len = strlen(filename);
  for (i=0; i<len; i++)
    hash_val = hash_val + filename[i];
  return hash_val;
}


int insertToHashTable(struct lru_linked_list* lru_linked_list0, struct cache_entry* entry_ptr) {
  
  struct hash_entry* hash_table0 = lru_linked_list0->hash_table;
  int hash_val = hash(entry_ptr->file_name);
  int index = hash_val;
  if (hash_table0[index].entry == NULL) {
    hash_table0[index].entry = entry_ptr;
    //printf("get into hash table[%d] \n", index);
  } else {

    if (index == (HASH_TABLE_SIZE-1) ) index = -1;
    index++;

    while (hash_table0[index].entry != NULL) {
      if (index == (HASH_TABLE_SIZE-1) )
        index = -1;
      index++;
    
    }
    hash_table0[index].entry = entry_ptr;
    //printf("get into hash table[%d] \n", index);
  }
}

int fileGetIn(struct lru_linked_list* cache0, char* file_name,int file_size, char* resource_ptr) {
   put_new_entry(cache0, resource_ptr, file_name, file_size);
  return 0;
}

