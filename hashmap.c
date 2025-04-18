#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long indice = hash(key, map->capacity);
    long original = indice;
    while(map->buckets[indice] != NULL && map->buckets[indice]->key != NULL)
    {
        if(is_equal(map->buckets[indice]->key,key)) return;
        indice = (indice + 1) % map->capacity;
        if(indice == original)return;
    }
    map->buckets[indice]= createPair(key,value);
    map->size++;
    map->current = indice;
    

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long capacidadA  = map->capacity;
    Pair ** bucketA = map->buckets;

    long capacidadN  = capacidadA * 2;

    map->buckets = (Pair **) malloc(sizeof(Pair *) * capacidadN);
    map->capacity = capacidadN;
    map->size = 0;
    map->current = -1;

    for(long t = 0; t < capacidadN ; t++){
        map->buckets[t] = NULL;
    }

    for (long i = 0; i < capacidadA; i++) {
        if (bucketA[i] != NULL && bucketA[i]->key != NULL) {
            insertMap(map, bucketA[i]->key, bucketA[i]->value);
        }
    }

    free(bucketA);

}


HashMap * createMap(long capacity) {
    HashMap *mapa = (HashMap*)malloc(sizeof(HashMap));
    mapa->buckets = (Pair**)malloc(sizeof(Pair*)*capacity);
    for(long i = 0; i < capacity; i++)
    {
        mapa->buckets[i] = NULL;
    }
     mapa->capacity = capacity;
     mapa->size = 0;
     mapa->current = -1;

    return mapa;
}

void eraseMap(HashMap * map,  char * key) {    
    long indice = hash(key, map->capacity);
    long original = indice;

    while (map->buckets[indice] != NULL) {
        if (map->buckets[indice]->key != NULL && strcmp(map->buckets[indice]->key, key) == 0) {
            map->buckets[indice]->key = NULL;

            map->size--;
            return;
        }

        indice = (indice+1) % map->capacity;

        if (indice == original) break;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long indice = hash(key, map->capacity);
    long original = indice;

    while (map->buckets[indice] != NULL) {
        if (map->buckets[indice]->key != NULL && is_equal(map->buckets[indice]->key, key)) {
            map->current = indice;
            return map->buckets[indice];
        }
        indice = (indice + 1) % map->capacity;
        if (indice == original) break;
    }

    return NULL;

}

Pair * firstMap(HashMap * map) {
    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for (long i = map->current + 1; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
