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
} Via;

typedef struct {
    Via *via;
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

Cache_il1* inicializaCache_il1(u16 nsets_il1, u16 bsize_il1, u16 assoc_il1){
    Cache_il1 *cache_il1 = malloc(sizeof(*cache_il1));

    cache_il1->nsets = nsets_il1;
    cache_il1->bsize = bsize_il1;
    cache_il1->assoc = assoc_il1;

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
        printf("Erro ao abrir arquivo \"saida.txt\"\n");
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
                    fprintf(fp,"       ╠═══╬════════════════════════════╬═════════════════╣\n");
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

int main(int argc, char *argv[]){

    Cache *cache;

    u8 numBitsTag;
    u8 numBitsOffset;
    u8 numBitsIndice;

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

    nsets_il1 = (u16)strtoul(argv[1],NULL,10);
    bsize_il1 = (u16)strtoul(argv[2],NULL,10);
    assoc_il1 = (u16)strtoul(argv[3],NULL,10);
    nsets_dl1 = (u16)strtoul(argv[4],NULL,10);
    bsize_dl1 = (u16)strtoul(argv[5],NULL,10);
    assoc_dl1 = (u16)strtoul(argv[6],NULL,10);

    numBitsIndice = (u8)log2(nsets_il1);
    numBitsOffset = (u8)log2(bsize_il1);
    numBitsTag = 32 - (numBitsIndice + numBitsOffset);

    strcpy(fileName,argv[7]);

    fp = fopen(fileName,"r");

    if(fp == NULL){
        printf("Arquivo %s não existe\n", fileName);
        exit(EXIT_FAILURE);
    }

    cache = inicializaCache(nsets_il1,bsize_il1,assoc_il1,nsets_dl1,bsize_dl1,assoc_dl1);

    while(fgets(ptr,32,fp) != NULL){
        cache->numAcessos++;

        address = strtoul(ptr,NULL,10);

        bool hit = false;

        if(address < 536870912){
            cache->cache_dl1->numAcessos++;

            indice = (address << numBitsTag) >> (numBitsTag + numBitsOffset);
            offset = address << (numBitsTag + numBitsIndice) >> (numBitsTag + numBitsIndice);
            tag = address >> (numBitsIndice + numBitsOffset);

            int viaLivre = -1;

            for(int i = 0;i < assoc_dl1;i++){
                if(cache->cache_dl1->via[i].linha[indice].validade == 1 && cache->cache_dl1->via[i].linha[indice].tag == tag){
                    cache->cache_dl1->numHit++;
                    cache->numHit++;
                    hit = true;
                }

                if(cache->cache_dl1->via[i].linha[indice].validade == 0 && viaLivre == -1){
                    viaLivre = i;
                }   
            }

            if(!hit){
                if(viaLivre == -1){
                    via = rand() % assoc_dl1;
                }else{
                    via = viaLivre;
                }

                if(cache->cache_dl1->via[via].linha[indice].validade == 0){
                    cache->cache_dl1->numColdMiss++;
                    cache->numColdMiss++;
                }else{
                    cache->cache_dl1->numMissTotal++;
                    cache->numMissTotal++;
                }

                cache->cache_dl1->via[via].linha[indice].validade = 1;
                cache->cache_dl1->via[via].linha[indice].tag = tag;

                address = (address >> numBitsOffset) << numBitsOffset;

                for(int i = 0;i < bsize_dl1;i++){
                    sprintf(string,"MEM[%u]", address + i);
                    strcpy(cache->cache_dl1->via[via].linha[indice].bloco.conteudo[i],string);
                }
            }
        }else{
            cache->cache_il1->numAcessos++;

            indice = (address << numBitsTag) >> (numBitsTag + numBitsOffset);
            offset = address << (numBitsTag + numBitsIndice) >> (numBitsTag + numBitsIndice);
            tag = address >> (numBitsIndice + numBitsOffset);

            int viaLivre = -1;

            for(int i = 0;i < assoc_il1;i++){
                if(cache->cache_il1->via[i].linha[indice].validade == 1 && cache->cache_il1->via[i].linha[indice].tag == tag){
                    cache->cache_il1->numHit++;
                    cache->numHit++;
                    hit = true;
                }

                if(cache->cache_il1->via[i].linha[indice].validade == 0 && viaLivre == -1 && !hit){
                    viaLivre = i;
                } 
            }

            if(!hit){
                if(viaLivre == -1){
                    via = rand() % assoc_il1;
                }else{
                    via = viaLivre;
                }

                if(cache->cache_il1->via[via].linha[indice].validade == 0){
                    cache->cache_il1->numColdMiss++;
                    cache->numColdMiss++;
                }else{
                    cache->cache_il1->numMissTotal++;
                    cache->numMissTotal++;
                }

                cache->cache_il1->via[via].linha[indice].validade = 1;
                cache->cache_il1->via[via].linha[indice].tag = tag;

                address = (address >> numBitsOffset) << numBitsOffset;

                for(int i = 0;i < bsize_il1;i++){
                    sprintf(string,"MEM[%u]", address + i);
                    strcpy(cache->cache_il1->via[via].linha[indice].bloco.conteudo[i],string);
                }
            }
        }
    }

    fclose(fp);

    salvaDadosCache(cache);

    return 0;
}