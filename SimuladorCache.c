#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "cache.h"

int main(int argc, char *argv[]){

    Cache *cache;

    if(argc != 8 && argc != 2){
        printf("ENTRADA DE PARAMETROS INCORRETA!\n");
        printf("./SimuladorCache <nome_arquivo>\n");
        printf("./SimuladorCache <nsets_il1> <bsize_il1> <assoc_il1> <nsets_dl1> <bsize_dl1> <assoc_dl1> <nome_arquivo>\n");

        exit(EXIT_FAILURE);
    }

    u16 nsets_il1;
    u16 bsize_il1;
    u16 assoc_il1;
    u16 nsets_dl1;
    u16 bsize_dl1;
    u16 assoc_dl1;
    u16 via;

    u32 tag;
    u32 offset;
    u32 indice;
    u32 address;

    char fileName[20];
    char ptr[32];
    char string[20];

    FILE *fp; 

    if(argc == 2){
        nsets_il1 = 64;
        bsize_il1 = 4;
        assoc_il1 = 1;
        nsets_dl1 = 64;
        bsize_dl1 = 4;
        assoc_dl1 = 1;

        strcpy(fileName,argv[1]);

        fp = fopen(fileName,"r");

        if(fp == NULL){
            printf("Arquivo %s não existe\n", fileName);
            exit(EXIT_FAILURE);
        }
    }else{
        nsets_il1 = (u16)strtoul(argv[1],NULL,10);
        bsize_il1 = (u16)strtoul(argv[2],NULL,10);
        assoc_il1 = (u16)strtoul(argv[3],NULL,10);
        nsets_dl1 = (u16)strtoul(argv[4],NULL,10);
        bsize_dl1 = (u16)strtoul(argv[5],NULL,10);
        assoc_dl1 = (u16)strtoul(argv[6],NULL,10);

        strcpy(fileName,argv[7]);

        fp = fopen(fileName,"r");

        if(fp == NULL){
            printf("Arquivo %s não existe\n", fileName);
            exit(EXIT_FAILURE);
        }
    }

    cache = inicializaCache(nsets_il1,bsize_il1,assoc_il1,nsets_dl1,bsize_dl1,assoc_dl1);

    while(fgets(ptr,32,fp) != NULL){
        cache->numAcessos++;

        address = strtoul(ptr,NULL,10);

        bool hit = false;

        if(address < 536870912){
            cache->cache_dl1->numAcessos++;

            indice = (address << cache->cache_dl1->numBitsTag) >> (cache->cache_dl1->numBitsTag + cache->cache_dl1->numBitsOffset);
            offset = address << (cache->cache_dl1->numBitsTag + cache->cache_dl1->numBitsIndice) >> (cache->cache_dl1->numBitsTag + cache->cache_dl1->numBitsIndice);
            tag = address >> (cache->cache_dl1->numBitsIndice + cache->cache_dl1->numBitsOffset);

            int viaLivre = -1;

            for(int i = 0;i < assoc_dl1;i++){
                if(cache->cache_dl1->via[i].linha[indice].validade == 1 && cache->cache_dl1->via[i].linha[indice].tag == tag){
                    cache->cache_dl1->numHit++;
                    cache->numHit++;
                    hit = true;

                    break;
                }

                if(cache->cache_dl1->via[i].linha[indice].validade == 0 && viaLivre == -1){
                    viaLivre = i;
                }   
            }

            if(!hit){
                if(viaLivre == -1){
                    via = rand() % assoc_dl1;

                    cache->cache_dl1->numMissTotal++;
                    cache->numMissTotal++;
                }else{
                    via = viaLivre;

                    cache->cache_dl1->numColdMiss++;
                    cache->numColdMiss++;
                }

                cache->cache_dl1->via[via].linha[indice].validade = 1;
                cache->cache_dl1->via[via].linha[indice].tag = tag;

                address = (address >> cache->cache_dl1->numBitsOffset) << cache->cache_dl1->numBitsOffset;

                for(int i = 0;i < bsize_dl1;i++){
                    sprintf(string,"MEM[%u]", address + i);
                    strcpy(cache->cache_dl1->via[via].linha[indice].bloco.conteudo[i],string);
                }
            }
        }else{
            cache->cache_il1->numAcessos++;

            indice = (address << cache->cache_il1->numBitsTag) >> (cache->cache_il1->numBitsTag + cache->cache_il1->numBitsOffset);
            offset = address << (cache->cache_il1->numBitsTag + cache->cache_il1->numBitsIndice) >> (cache->cache_il1->numBitsTag + cache->cache_il1->numBitsIndice);
            tag = address >> (cache->cache_il1->numBitsIndice + cache->cache_il1->numBitsOffset);

            int viaLivre = -1;

            for(int i = 0;i < assoc_il1;i++){
                if(cache->cache_il1->via[i].linha[indice].validade == 1 && cache->cache_il1->via[i].linha[indice].tag == tag){
                    cache->cache_il1->numHit++;
                    cache->numHit++;
                    hit = true;

                    break;
                }

                if(cache->cache_il1->via[i].linha[indice].validade == 0 && viaLivre == -1){
                    viaLivre = i;
                } 
            }

            if(!hit){
                if(viaLivre == -1){
                    via = rand() % assoc_il1;

                    cache->cache_il1->numMissTotal++;
                    cache->numMissTotal++;
                }else{
                    via = viaLivre;

                    cache->cache_il1->numColdMiss++;
                    cache->numColdMiss++;
                }

                cache->cache_il1->via[via].linha[indice].validade = 1;
                cache->cache_il1->via[via].linha[indice].tag = tag;

                address = (address >> cache->cache_il1->numBitsOffset) << cache->cache_il1->numBitsOffset;

                for(int i = 0;i < bsize_il1;i++){
                    sprintf(string,"MEM[%u]", address + i);
                    strcpy(cache->cache_il1->via[via].linha[indice].bloco.conteudo[i],string);
                }
            }
        }
    }

    fclose(fp);

    imprimeDadosCache(cache);

    liberaMemoriaCache(cache);

    return 0;
}