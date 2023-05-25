#include<stdio.h>
#include<stdlib.h>
#include"hashmap.h"

HashMap* HashMap_Create() {
	return (HashMap*)calloc(1, sizeof(HashMap));
}
int hash(char* key)
{
	int h = 0, g;
	while (*key)
	{
		h = (h << 4) + *key++;
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &= ~g;
	}
	return h % N;
}
V HashMap_put(HashMap* map, K key, V val) {
	int idx = hash(key);
	Entry* curr = map->table[idx];
	while (curr != NULL) { 
		if (strcmp(curr->key, key) == 0) {
			V oldval = curr->val;
			curr->val = val;
			return oldval;
		}
		curr = curr->next;
	}
	
	Entry* entry = (Entry*)malloc(sizeof(Entry)); // sdaf
	entry->key = key;
	entry->val = val;
	entry->next = map->table[idx];
	map->table[idx] = entry;
	return -1;
}
int HashMap_remove(HashMap* map, K key) {
	int idx = hash(key);
	Entry* curr = map->table[idx];
	Entry* prev = NULL;
	while (curr != NULL) {
		
		if (strcmp(curr->key, key) == 0) {
			int oldval;
			Entry* oldentry = curr;
			if (prev == NULL) {
				oldval = curr->val;
				map->table[idx] = curr->next;

			}
			else {
				
				prev->next = curr->next;
				oldval = curr->val;
			}
			free(oldentry);
			return oldval;

		}
		prev = curr;
		curr = curr->next;
	}
	return -1;

	

}