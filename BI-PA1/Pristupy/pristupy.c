#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int calculateUniqueAccess(int from, int to, int uniqueAccess, int storage[]){
    int repeatingId[100000] = {0};

    for(int z = from; z <= to; z++){
        if(repeatingId[storage[z]] != 0){
            uniqueAccess--;
        }else{
            repeatingId[storage[z]]++;
        }
    }

    return uniqueAccess;
}

int main (void){

    char indicator;
    int id;
    int from = 0, to = 0;
    int x = 0;
    static int storage[1000000];
    static int repeatingAccess[100000];


    printf("Pozadavky:\n");

    while(1){

        int proEOF = 0;
        // printf("Prvni v repeating array: %d\n", repeatingId[0]);
        if((proEOF = (scanf(" %c", &indicator))) != 1 || (indicator != '+' && indicator != '?')){
            if(proEOF == EOF){
                break;
            }
            printf("Nespravny vstup.\n");
            return 1;
        }

        if(indicator == '+'){
            if(scanf("%d", &id) != 1 || id < 0 || id > 99999 || x > 1000000){
                printf("Nespravny vstup.\n");
                return 1;
            }
            storage[x] = id;
            // printf("-----------------------Zadane cislo bylo:  %d\n", storage[x]);

            repeatingAccess[id]++;
            if(repeatingAccess[id] != 1){
                printf("> navsteva #%d\n", repeatingAccess[id]);
            }else{
                printf("> prvni navsteva\n");
            }
            x++;
            // printf("Input Indicator: %c\n", indicator);
            // printf("Input ID: %d\n", id);

        }else{
            if(scanf("%d %d", &from, &to) != 2 || from < 0 || from > to || to >= x){
                printf("Nespravny vstup.\n");
                return 1;
            }
            int totalRange = (to-from) + 1;
            int uniqueAccess = (to-from) + 1;

            uniqueAccess = calculateUniqueAccess(from, to, uniqueAccess, storage);

            printf("> %d / %d\n", uniqueAccess, totalRange);
        }
    }

    return 0;
}
