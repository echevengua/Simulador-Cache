#ifndef CACHE_H
#define CACHE_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {
    char **conteudo;
} Bloco;

typedef struct {
    bool validade;
    u32 tag;
    Bloco bloco;
} Linha;

typedef struct {
    Linha *linha;
} Via;

typedef struct {
    Via *via;
    u8 numBitsTag;
    u8 numBitsOffset;
    u8 numBitsIndice;
    u16 nsets;
    u16 bsize;
    u16 assoc;
    u32 numAcessos;
    u32 numHit;
    u32 numMissTotal;
    u32 numColdMiss;
} Cache_il1;

typedef struct {
    Via *via;
    u8 numBitsTag;
    u8 numBitsOffset;
    u8 numBitsIndice;
    u16 nsets;
    u16 bsize;
    u16 assoc;
    u32 numAcessos;
    u32 numHit;
    u32 numMissTotal;
    u32 numColdMiss;
} Cache_dl1;

typedef struct {
    u32 numAcessos;
    u32 numHit;
    u32 numMissTotal;
    u32 numColdMiss;
    Cache_il1 *cache_il1;
    Cache_dl1 *cache_dl1;
} Cache;

Cache_il1* inicializaCache_il1(u16 nsets_il1, u16 bsize_il1, u16 assoc_il1);
Cache_dl1* inicializaCache_dl1(u16 nsets_dl1, u16 bsize_dl1, u16 assoc_dl1);
Cache* inicializaCache(u16 nsets_il1, u16 bsize_il1, u16 assoc_il1, u16 nsets_dl1, u16 bsize_dl1, u16 assoc_dl1);

#endif