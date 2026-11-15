#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "misc.h"

float divisao(float a, float b){
    if(b == 0){
        return 0;
    }else{
        return a/b;
    }
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

    fclose(fp);
}

void imprimeDadosCache(Cache *cache){
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║NUMERO ACESSOS: %-10u                                          ║ \n", cache->numAcessos);
    printf("║                                                                    ║ \n");
    printf("║NUMERO HIT: %-10u                                              ║ \n", cache->numHit);
    printf("║NUMERO MISSES COMPULSORIOS: %-10u                              ║ \n", cache->numColdMiss);
    printf("║NUMERO DE MISS CONFLITO/CAPACIDADE: %-10u                      ║ \n", cache->numMissTotal);
    printf("║MISS RATE: %.4f                                                   ║ \n", divisao((float)(cache->numColdMiss + cache->numMissTotal),(float)cache->numAcessos));
    printf("║HIT RATE: %.4f                                                    ║ \n", divisao((float)cache->numHit,(float)cache->numAcessos));
    printf("║                                                                    ║\n");
    printf("║NUMERO HIT CACHE_IL1: %-10u                                    ║ \n", cache->cache_il1->numHit);
    printf("║NUMERO MISSES COMPULSORIOS CACHE_IL1: %-10u                    ║ \n", cache->cache_il1->numColdMiss);
    printf("║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_IL1: %-10u            ║ \n", cache->cache_il1->numMissTotal);
    printf("║MISS RATE CACHE_IL1: %.4f                                         ║ \n", divisao((float)(cache->cache_il1->numColdMiss + cache->cache_il1->numMissTotal),
                                                                                          (float)cache->cache_il1->numAcessos));
    printf("║HIT RATE CACHE_IL1: %.4f                                          ║ \n", divisao((float)cache->cache_il1->numHit,(float)cache->cache_il1->numAcessos));
    printf("║                                                                    ║ \n");
    printf("║NUMERO HIT CACHE_DL1: %-10u                                    ║ \n", cache->cache_dl1->numHit);
    printf("║NUMERO MISSES COMPULSORIOS CACHE_DL1: %-10u                    ║ \n", cache->cache_dl1->numColdMiss);
    printf("║NUMERO DE MISS CONFLITO/CAPACIDADE CACHE_DL1: %-10u            ║ \n", cache->cache_dl1->numMissTotal);
    printf("║MISS RATE CACHE_DL1: %.4f                                         ║ \n", divisao((float)(cache->cache_dl1->numColdMiss + cache->cache_dl1->numMissTotal),
                                                                                                (float)cache->cache_dl1->numAcessos));
    printf("║HIT RATE CACHE_DL1: %.4f                                          ║ \n", divisao((float)cache->cache_dl1->numHit,(float)cache->cache_dl1->numAcessos));
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}