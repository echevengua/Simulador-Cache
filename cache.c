#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cache.h"

Cache_il1* inicializaCache_il1(u16 nsets_il1, u16 bsize_il1, u16 assoc_il1){
    Cache_il1 *cache_il1 = malloc(sizeof(*cache_il1));

    cache_il1->nsets = nsets_il1;
    cache_il1->bsize = bsize_il1;
    cache_il1->assoc = assoc_il1;

    cache_il1->numBitsIndice = (u8)log2(nsets_il1);
    cache_il1->numBitsOffset = (u8)log2(bsize_il1);
    cache_il1->numBitsTag = 32 - (cache_il1->numBitsIndice + cache_il1->numBitsOffset);

    cache_il1->numAcessos = 0;
    cache_il1->numHit = 0;
    cache_il1->numMissTotal = 0;
    cache_il1->numColdMiss = 0;
    
    cache_il1->via = malloc(sizeof(Via) * assoc_il1);

    for(int i = 0;i < assoc_il1;i++){
        cache_il1->via[i].linha = malloc(sizeof(Linha) * nsets_il1);

        for(int j = 0;j < nsets_il1;j++){
            cache_il1->via[i].linha[j].bloco.conteudo = malloc(sizeof(char*) * bsize_il1);
            cache_il1->via[i].linha[j].validade = 0;

            for(int k = 0;k < bsize_il1;k++){
                cache_il1->via[i].linha[j].bloco.conteudo[k] = malloc(sizeof(char) * 20);
                strcpy(cache_il1->via[i].linha[j].bloco.conteudo[k],"[lixo de memoria]");
            }
        }
    }

    return cache_il1;
}

Cache_dl1* inicializaCache_dl1(u16 nsets_dl1, u16 bsize_dl1, u16 assoc_dl1){
    Cache_dl1 *cache_dl1 = malloc(sizeof(*cache_dl1));

    cache_dl1->nsets = nsets_dl1;
    cache_dl1->bsize = bsize_dl1;
    cache_dl1->assoc = assoc_dl1;

    cache_dl1->numBitsIndice = (u8)log2(nsets_dl1);
    cache_dl1->numBitsOffset = (u8)log2(bsize_dl1);
    cache_dl1->numBitsTag = 32 - (cache_dl1->numBitsIndice + cache_dl1->numBitsOffset);

    cache_dl1->numAcessos = 0;
    cache_dl1->numHit = 0;
    cache_dl1->numMissTotal = 0;
    cache_dl1->numColdMiss = 0;
    
    cache_dl1->via = malloc(sizeof(Via) * assoc_dl1);

    for(int i = 0;i < assoc_dl1;i++){
        cache_dl1->via[i].linha = malloc(sizeof(Linha) * nsets_dl1);

        for(int j = 0;j < nsets_dl1;j++){
            cache_dl1->via[i].linha[j].bloco.conteudo = malloc(sizeof(char*) * bsize_dl1);
            cache_dl1->via[i].linha[j].validade = 0;

            for(int k = 0;k < bsize_dl1;k++){
                cache_dl1->via[i].linha[j].bloco.conteudo[k] = malloc(sizeof(char) * 20);
                strcpy(cache_dl1->via[i].linha[j].bloco.conteudo[k],"[lixo de memoria]");
            }
        }
    }

    return cache_dl1;
}

Cache* inicializaCache(u16 nsets_il1, u16 bsize_il1, u16 assoc_il1, u16 nsets_dl1, u16 bsize_dl1, u16 assoc_dl1){
    Cache *cache = malloc(sizeof(Cache));

    cache->cache_il1 = inicializaCache_il1(nsets_il1,bsize_il1,assoc_il1);
    cache->cache_dl1 = inicializaCache_dl1(nsets_dl1,bsize_dl1,assoc_dl1);

    cache->numHit = 0;
    cache->numMissTotal = 0;
    cache->numColdMiss = 0;
    cache->numAcessos = 0;

    return cache;
}