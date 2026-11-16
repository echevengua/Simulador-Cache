#include <stdio.h>
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

void liberaMemoriaCache(Cache *cache){
    for(int i = 0;i < cache->cache_dl1->assoc;i++){
        for(int j = 0;j < cache->cache_dl1->nsets;j++){
            for(int k = 0;k < cache->cache_dl1->bsize;k++){
                free(cache->cache_dl1->via[i].linha[j].bloco.conteudo[k]);
            }
            free(cache->cache_dl1->via[i].linha[j].bloco.conteudo);
        }
        free(cache->cache_dl1->via[i].linha);
    }
    free(cache->cache_dl1->via);
    free(cache->cache_dl1);

    for(int i = 0;i < cache->cache_il1->assoc;i++){
        for(int j = 0;j < cache->cache_il1->nsets;j++){
            for(int k = 0;k < cache->cache_il1->bsize;k++){
                free(cache->cache_il1->via[i].linha[j].bloco.conteudo[k]);
            }
            free(cache->cache_il1->via[i].linha[j].bloco.conteudo);
        }
        free(cache->cache_il1->via[i].linha);
    }
    free(cache->cache_il1->via);
    free(cache->cache_il1);

    free(cache);
}

float divisao(float a, float b){
    if(b == 0){
        return 0;
    }else{
        return a/b;
    }
}

void imprimeDadosCache(Cache *cache){
    printf("┌────────────────────────────────────────────────────────────────────┐\n");
    printf("│NUMERO ACESSOS TOTAIS: %-10u                                   │\n", cache->numAcessos);
    printf("│NUMERO HIT: %-10u                                              │\n", cache->numHit);
    printf("│NUMERO MISSES TOTAIS: %-10u                                    │\n", cache->numColdMiss + cache->numMissTotal);
    printf("│NUMERO MISSES COMPULSORIOS: %-10u                              │\n", cache->numColdMiss);
    printf("│NUMERO DE MISS CONFLITO/CAPACIDADE: %-10u                      │\n", cache->numMissTotal);
    printf("│MISS RATE: %.4f                                                   │\n", divisao((float)(cache->numColdMiss + cache->numMissTotal),(float)cache->numAcessos));
    printf("│HIT RATE: %.4f                                                    │\n", divisao((float)cache->numHit,(float)cache->numAcessos));
    printf("├────────────────────────────────────────────────────────────────────┤\n");
    printf("│NUMERO ACESSOS CACHE_DL1: %-10u                                │\n", cache->cache_dl1->numAcessos);
    printf("│NUMERO HIT CACHE_DL1: %-10u                                    │\n", cache->cache_dl1->numHit);
    printf("│NUMERO MISSES TOTAIS CACHE_IL1: %-10u                          │\n", cache->cache_dl1->numColdMiss + cache->cache_dl1->numMissTotal);
    printf("│NUMERO MISSES COMPULSORIOS CACHE_DL1: %-10u                    │\n", cache->cache_dl1->numColdMiss);
    printf("│NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_DL1: %-10u            │\n", cache->cache_dl1->numMissTotal);
    printf("│MISS RATE CACHE_DL1: %.4f                                         │\n", divisao((float)(cache->cache_dl1->numColdMiss + cache->cache_dl1->numMissTotal),
                                                                                                (float)cache->cache_dl1->numAcessos));
    printf("│HIT RATE CACHE_DL1: %.4f                                          │\n", divisao((float)cache->cache_dl1->numHit,(float)cache->cache_dl1->numAcessos));
    printf("├────────────────────────────────────────────────────────────────────┤\n");
    printf("│NUMERO ACESSOS CACHE_IL1: %-10u                                │\n", cache->cache_il1->numAcessos);
    printf("│NUMERO HIT CACHE_IL1: %-10u                                    │\n", cache->cache_il1->numHit);
    printf("│NUMERO MISSES TOTAIS CACHE_IL1: %-10u                          │\n", cache->cache_il1->numColdMiss + cache->cache_il1->numMissTotal);
    printf("│NUMERO MISSES COMPULSORIOS CACHE_IL1: %-10u                    │\n", cache->cache_il1->numColdMiss);
    printf("│NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_IL1: %-10u            │\n", cache->cache_il1->numMissTotal);
    printf("│MISS RATE CACHE_IL1: %.4f                                         │\n", divisao((float)(cache->cache_il1->numColdMiss + cache->cache_il1->numMissTotal),
                                                                                          (float)cache->cache_il1->numAcessos));
    printf("│HIT RATE CACHE_IL1: %.4f                                          │\n", divisao((float)cache->cache_il1->numHit,(float)cache->cache_il1->numAcessos));
    printf("└────────────────────────────────────────────────────────────────────┘\n");
}

void salvaDadosCache(Cache *cache){
    FILE *fp;
    bool minBloco = true;

    fp = fopen("saida.txt","w");

    if(fp == NULL){
        printf("Erro ao criar arquivo \"saida.txt\"\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp,"╔════════════════════════════════════════════════════════════════════╗\n");
    fprintf(fp,"║NUMERO ACESSOS: %-10u                                          ║ \n", cache->numAcessos);
    fprintf(fp,"║                                                                    ║ \n");
    fprintf(fp,"║NUMERO HIT: %-10u                                              ║ \n", cache->numHit);
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS: %-10u                              ║ \n", cache->numColdMiss);
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE: %-10u                      ║ \n", cache->numMissTotal);
    fprintf(fp,"║MISS RATE: %.4f                                                   ║ \n", divisao((float)(cache->numColdMiss + cache->numMissTotal),(float)cache->numAcessos));
    fprintf(fp,"║HIT RATE: %.4f                                                    ║ \n", divisao((float)cache->numHit,(float)cache->numAcessos));
    fprintf(fp,"║                                                                    ║\n");
    fprintf(fp,"║NUMERO HIT CACHE_IL1: %-10u                                    ║ \n", cache->cache_il1->numHit);
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS CACHE_IL1: %-10u                    ║ \n", cache->cache_il1->numColdMiss);
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_IL1: %-10u            ║ \n", cache->cache_il1->numMissTotal);
    fprintf(fp,"║MISS RATE CACHE_IL1: %.4f                                         ║ \n", divisao((float)(cache->cache_il1->numColdMiss + cache->cache_il1->numMissTotal),
                                                                                          (float)cache->cache_il1->numAcessos));
    fprintf(fp,"║HIT RATE CACHE_IL1: %.4f                                          ║ \n", divisao((float)cache->cache_il1->numHit,(float)cache->cache_il1->numAcessos));
    fprintf(fp,"║                                                                    ║ \n");
    fprintf(fp,"║NUMERO HIT CACHE_DL1: %-10u                                    ║ \n", cache->cache_dl1->numHit);
    fprintf(fp,"║NUMERO MISSES COMPULSORIOS CACHE_DL1: %-10u                    ║ \n", cache->cache_dl1->numColdMiss);
    fprintf(fp,"║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_DL1: %-10u            ║ \n", cache->cache_dl1->numMissTotal);
    fprintf(fp,"║MISS RATE CACHE_DL1: %.4f                                         ║ \n", divisao((float)(cache->cache_dl1->numColdMiss + cache->cache_dl1->numMissTotal),
                                                                                                (float)cache->cache_dl1->numAcessos));
    fprintf(fp,"║HIT RATE CACHE_DL1: %.4f                                          ║ \n", divisao((float)cache->cache_dl1->numHit,(float)cache->cache_dl1->numAcessos));
    fprintf(fp,"╚════════════════════════════════════════════════════════════════════╝\n");

    fprintf(fp,"       ╔══════════════════════════════════════════════════╗\n");
    fprintf(fp,"       ║                    CACHE_DL1                     ║\n");
    
    for(int i = 0;i < cache->cache_dl1->assoc;i++){
        if(i == 0){
            fprintf(fp,"       ╠══════════════════════════════════════════════════╣\n");
        }else{
            fprintf(fp,"       ╔══════════════════════════════════════════════════╗\n");
        }
        fprintf(fp,"       ║                    VIA %d                         ║\n",i+1);
        fprintf(fp,"       ╠═══╦════════════════════════════╦═════════════════╣\n");
        fprintf(fp,"INDICE ║VAL║            TAG             ║   INFORMACAO    ║\n");
        fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");

        for(int j = 0;j < cache->cache_dl1->nsets;j++){
            fprintf(fp,"0x%05x║ %d ║             %-10u     ║%-17s║\n",j,cache->cache_dl1->via[i].linha[j].validade,cache->cache_dl1->via[i].linha[j].tag,
                                                                cache->cache_dl1->via[i].linha[j].bloco.conteudo[0]);

            fprintf(fp,"       ╠═══╩════════════════════════════╬═════════════════╣\n");
            minBloco = true;

            for(int k = 1;k < cache->cache_dl1->bsize;k++){
                minBloco = false;

                fprintf(fp,"       ║ ------------------------------ ║%-17s║\n",cache->cache_dl1->via[i].linha[j].bloco.conteudo[k]);

                if(!(k + 1 < cache->cache_dl1->bsize) && !(j + 1 < cache->cache_dl1->nsets)){
                    fprintf(fp,"       ╚════════════════════════════════╩═════════════════╝\n");
                }else if(!(k + 1 < cache->cache_dl1->bsize)){
                    fprintf(fp,"       ╠═══╦════════════════════════════╬═════════════════╣\n");
                }else{
                    fprintf(fp,"       ╠════════════════════════════════╬═════════════════╣\n");
                }
            }
            if(minBloco && !(j + 1 < cache->cache_dl1->nsets)){
                fprintf(fp,"       ╚═══╩════════════════════════════╩═════════════════╝\n");
            }else if(minBloco){
                fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");
            }
        }
    }

    fprintf(fp,"       ╔══════════════════════════════════════════════════╗\n");
    fprintf(fp,"       ║                    CACHE_IL1                     ║\n");
    
    for(int i = 0;i < cache->cache_il1->assoc;i++){
        if(i == 0){
            fprintf(fp,"       ╠══════════════════════════════════════════════════╣\n");
        }else{
            fprintf(fp,"       ╔══════════════════════════════════════════════════╗\n");
        }
        fprintf(fp,"       ║                    VIA %d                         ║\n",i+1);
        fprintf(fp,"       ╠═══╦════════════════════════════╦═════════════════╣\n");
        fprintf(fp,"INDICE ║VAL║            TAG             ║   INFORMACAO    ║\n");
        fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");

        for(int j = 0;j < cache->cache_il1->nsets;j++){
            fprintf(fp,"0x%05x║ %d ║             %-10u     ║%-17s║\n",j,cache->cache_il1->via[i].linha[j].validade,cache->cache_il1->via[i].linha[j].tag,
                                                                cache->cache_il1->via[i].linha[j].bloco.conteudo[0]);
            
            fprintf(fp,"       ╠═══╩════════════════════════════╬═════════════════╣\n");
            minBloco = true;

            for(int k = 1;k < cache->cache_il1->bsize;k++){
                minBloco = false;
                
                fprintf(fp,"       ║ ------------------------------ ║%-17s║\n",cache->cache_il1->via[i].linha[j].bloco.conteudo[k]);

                if(!(k + 1 < cache->cache_il1->bsize) && !(j + 1 < cache->cache_il1->nsets)){
                    fprintf(fp,"       ╚════════════════════════════════╩═════════════════╝\n");
                }else if(!(k + 1 < cache->cache_il1->bsize)){
                    fprintf(fp,"       ╠═══╦════════════════════════════╬═════════════════╣\n");
                }else{
                    fprintf(fp,"       ╠════════════════════════════════╬═════════════════╣\n");
                }
            }
            if(minBloco && !(j + 1 < cache->cache_il1->nsets)){
                fprintf(fp,"       ╚════════════════════════════════╩═════════════════╝\n");
            }else if(minBloco){
                fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");
            }
        }
    }

    fclose(fp);
}