#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

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
} Conjunto;

typedef struct {
    Conjunto *conjunto;
    u16 nsets;
    u16 bsize;
    u16 assoc;
    u32 numHit;
    u32 numMissTotal;
    u32 numColdMiss;
    u32 numAcessos;
} Cache;

Cache* inicializaCache(u16 nsets, u16 bsize, u16 assoc){
    Cache *cache = malloc(sizeof(Cache));

    cache->nsets = nsets;
    cache->bsize = bsize;
    cache->assoc = assoc;

    cache->numHit = 0;
    cache->numMissTotal = 0;
    cache->numColdMiss = 0;
    cache->numAcessos = 0;

    cache->conjunto = malloc(sizeof(Conjunto) * assoc);

    for(int i = 0;i < assoc;i++){
        cache->conjunto[i].linha = malloc(sizeof(Linha) * nsets);

        for(int j = 0;j < nsets;j++){
            cache->conjunto[i].linha[j].bloco.conteudo = malloc(sizeof(char*) * bsize);
            cache->conjunto[i].linha[j].validade = 0;

            for(int k = 0;k < bsize;k++){
                cache->conjunto[i].linha[j].bloco.conteudo[k] = malloc(sizeof(char) * 20);
                strcpy(cache->conjunto[i].linha[j].bloco.conteudo[k],"[lixo de memoria]");
            }
        }
    }

    return cache;
}

void salvaDadosCache(Cache *cache){
    FILE *fp;
    bool minBloco = true;

    fp = fopen("saida.txt","w");

    if(fp == NULL){
        printf("Erro ao abrir arquivo \"saida.txt\"\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"╔════════════════════════════════════════════════════════════════════╗\n");
    fprintf(fp,"║NUMERO ACESSOS: %-10d                                          ║ \n", cache->numAcessos);
    fprintf(fp,"║                                                                    ║ \n");
    fprintf(fp,"║NUMERO HIT: %-10d                                              ║ \n", cache->numHit);
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS: %-10d                              ║ \n", cache->numColdMiss);
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE: %-10d                      ║ \n", cache->numMissTotal);
    fprintf(fp,"║MISS RATE: %.4f                                                   ║ \n", (float)(cache->numColdMiss + cache->numMissTotal) / (float)cache->numAcessos);
    fprintf(fp,"║HIT RATE: %.4f                                                    ║ \n", (float)cache->numHit / (float)cache->numAcessos);
    fprintf(fp,"║                                                                    ║\n");
    fprintf(fp,"║NUMERO HIT CACHE_IL1: %-10d                                    ║ \n", cache->numHit);
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS CACHE_IL1: %-10d                    ║ \n", cache->numColdMiss);
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_IL1: %-10d            ║ \n", cache->numMissTotal);
    fprintf(fp,"║MISS RATE CACHE_IL1: %.4f                                         ║ \n", (float)(cache->numColdMiss + cache->numMissTotal) / (float)cache->numAcessos);
    fprintf(fp,"║HIT RATE CACHE_IL1: %.4f                                          ║ \n", (float)cache->numHit / (float)cache->numAcessos);
    fprintf(fp,"║                                                                    ║ \n");
    fprintf(fp,"║NUMERO HIT CACHE_DL1: -                                             ║ \n");
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS CACHE_DL1: -                             ║ \n");
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_DL1: -                     ║ \n");
    fprintf(fp,"║MISS RATE CACHE_DL1: -                                              ║ \n");
    fprintf(fp,"║HIT RATE CACHE_DL1: -                                               ║ \n");
    fprintf(fp,"╚════════════════════════════════════════════════════════════════════╝\n");

    for(int i = 0;i < cache->assoc;i++){
        fprintf(fp,"       ╔══════════════════════════════════════════════════╗\n");
        fprintf(fp,"       ║                    CONJUNTO %d                    ║\n",i+1);
        fprintf(fp,"       ╠═══╦════════════════════════════╦═════════════════╣\n");
        fprintf(fp,"INDICE ║VAL║            TAG             ║   INFORMACAO    ║\n");
        fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");

        for(int j = 0;j < cache->nsets;j++){
            fprintf(fp,"0x%05x║ %d ║             %-10d     ║%-17s║\n",j,cache->conjunto[i].linha[j].validade,cache->conjunto[i].linha[j].tag,
                                                                cache->conjunto[i].linha[j].bloco.conteudo[0]);

            for(int k = 1;k < cache->bsize;k++){
                minBloco = false;

                fprintf(fp,"       ╩═══╩════════════════════════════╬═════════════════╣\n");
                fprintf(fp,"       ║ -------------------------------------- ║%-17s║\n",cache->conjunto[i].linha[j].bloco.conteudo[i]);

                if(!(k + 1 < cache->bsize) && !(j + 1 < cache->nsets)){
                    fprintf(fp,"       ╚═══════════════════════════════════════════════════╝\n");
                }else if(!(k + 1 < cache->bsize)){
                    fprintf(fp,"        ╠═══╦════════════════════════════╦═════════════════╣\n");
                }else{
                    fprintf(fp,"        ╠═══╬════════════════════════════╬═════════════════╣\n");
                }
            }
            if(minBloco && !(j + 1 < cache->nsets)){
                fprintf(fp,"       ╚═══╩════════════════════════════╩═════════════════╝\n");
            }else{
                fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");
            }
        }
    }
}

int main(int argc, char *argv[]){

    Cache *cache_dl1;

    u8 numBitsTag;
    u8 numBitsOffset;
    u8 numBitsIndice;

    u16 tag;
    u16 offset;
    u16 indice;
    u16 nsets_il1;
    u16 bsize_il1;
    u16 assoc_il1;
    // u16 nsets_dl1;
    // u16 bsize_dl1;
    // u16 assoc_dl1;
    u16 numRandom;

    u32 address;

    char fileName[20];
    char ptr[32];
    char string[20];

    FILE *fp; 

    nsets_il1 = (u16)strtoul(argv[1],NULL,10);
    bsize_il1 = (u16)strtoul(argv[2],NULL,10);
    assoc_il1 = (u16)strtoul(argv[3],NULL,10);

    numBitsIndice = (u8)log2(nsets_il1);
    numBitsOffset = (u8)log2(bsize_il1);
    numBitsTag = 32 - (numBitsIndice + numBitsOffset);

    strcpy(fileName,argv[4]);

    fp = fopen(fileName,"r");

    if(fp == NULL){
        printf("Arquivo %s não existe\n", fileName);
        exit(EXIT_FAILURE);
    }

    cache_dl1 = inicializaCache(nsets_il1,bsize_il1,assoc_il1);

    while(fgets(ptr,32,fp) != NULL){
        cache_dl1->numAcessos++;

        address = strtoul(ptr,NULL,10);

        indice = (address << numBitsTag) >> (numBitsTag + numBitsOffset);
        offset = address << (numBitsTag + numBitsIndice) >> (numBitsTag + numBitsIndice);
        tag = address >> (numBitsIndice + numBitsOffset);

        bool hit = false;

        for(int i = 0;i < assoc_il1;i++){
            if(cache_dl1->conjunto[i].linha[indice].validade == 1 && cache_dl1->conjunto[i].linha[indice].tag == tag){
                cache_dl1->numHit++;
                hit = true;
            }
        }

        if(!hit){
           numRandom = rand() % assoc_il1;

            if(cache_dl1->conjunto[numRandom].linha[indice].validade == 0){
                cache_dl1->numColdMiss++;
            }else{
                cache_dl1->numMissTotal++;
            }

            cache_dl1->conjunto[numRandom].linha[indice].validade = 1;
            cache_dl1->conjunto[numRandom].linha[indice].tag = tag;

            address = (address >> numBitsOffset) << numBitsOffset;

            for(int i = 0;i < bsize_il1;i++){
                sprintf(string,"MEM[%u]", address + i);
                strcpy(cache_dl1->conjunto[numRandom].linha[indice].bloco.conteudo[i],string);
            }
        }
    }

    fclose(fp);

    salvaDadosCache(cache_dl1);


    return 0;
}