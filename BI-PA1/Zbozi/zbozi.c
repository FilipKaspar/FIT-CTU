#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 99

typedef struct {
    char name[NAME_LENGTH + 1];
    int amount;
} Specification;

int compareAmount(const void *a, const void *b){
    return ((const Specification*)b)->amount - ((const Specification*)a)->amount;
}

void printingResults(Specification **items,size_t items_size,size_t listedItems,int printingItems){
    int totalAmount = 0;

    qsort(*items, items_size, sizeof(**items), compareAmount);

    /*printf("------QSORT------\n");
    for(size_t y = 0; y < items_size; y++){
        printf("\nName: %s\nAccesed: %d\n", (*items+y)->name, (*items+y)->amount);
    }*/

    int from = 1,inRow;
    size_t to = 0, x = 0;
    while(x < listedItems){
        if(to >= items_size){
            break;
        }
        inRow = 1;
        for(size_t z = 1; z+x < items_size; z++){
            if((*items+x)->amount == (*items+x+z)->amount){
                inRow++;
            }else{
                break;
            }
        }
        to += inRow;
        for(int z = 0; z < inRow; z++){
            if(printingItems){
                if(inRow != 1){
                    printf("%d.-%ld. %s, %dx\n", from, to, (*items+x+z)->name, (*items+x+z)->amount);
                }
                else{
                    printf("%d. %s, %dx\n", from, (*items+x+z)->name, (*items+x+z)->amount);
                }
            }
            totalAmount += (*items+x+z)->amount;
        }
    from = to+1;
    x += inRow;
    }
    printf("Nejprodavanejsi zbozi: prodano %d kusu\n", totalAmount);
}

int main (void){
    // For Structure Specification
    Specification *items = NULL;
    size_t items_size = 0, items_capacity = 0;

    int listedItems = 0;

    char checkSizeName[NAME_LENGTH + 2];
    int check = 0;

    printf("Pocet sledovanych:\n");
    if(scanf("%d", &listedItems) != 1 || listedItems < 1){
        printf("Nespravny vstup.\n");
        free(items);
        return 1;
    }

    printf("Pozadavky:\n");

    char action;
    while(1){
        //printf("------------Dalsi while------------\n");
        if(scanf(" %c", &action) == EOF){
            //printf("EOF\n");
            break;
        }
        //printf("---------Proslo\n");

        switch(action){
            case '+':{
               int first = 1;
                    check = (scanf("%100s", checkSizeName));
                    if(strlen(checkSizeName) >= NAME_LENGTH + 1 || checkSizeName[0] == '\0' || check != 1){
                        printf("Nespravny vstup.\n");
                        free(items);
                        return 1;
                    } // radky od check = scanf po tento komentar jsou pouzity z ukazovaneho kodu na cviceni 8 od cvicicich Radek Husek a Ondrej Wrzecionko
                    // printf("Sizeof *items: %ld\n", sizeof(*items));

                    if(items_size >= items_capacity){
                        items_capacity = items_capacity * 2 + 4;
                        // printf("!REALLOC! New items capacity: %ld\n", items_capacity);
                        items = (Specification*)realloc(items, items_capacity * sizeof(*items));
                    }

                    for(size_t x = 0; x < items_size; x++){
                        if((first = (strcmp(checkSizeName, (items+x)->name))) == 0){
                            (items+x)->amount++;
                            break;
                        }
                    }
                    if(first){
                        strcpy((items + items_size)->name , checkSizeName);
                        (items + items_size)->amount = 1;
                        items_size++;
                    }
                break;
            }
            case '#':{
                printingResults(&items,items_size,listedItems,1);
                break;
            }
            case '?':{
                printingResults(&items,items_size,listedItems,0);
                break;
            }
            default:{
                printf("Nespravny vstup.\n");
                free(items);
                return 1;
            }
        }
        // printf("VALUE OF ITEMS_SIZE: %d\n", items_size);
        /*for(size_t y = 0; y < items_size; y++){
            printf("\nName: %s\nAccesed: %d\n", (items+y)->name, (items+y)->amount);

        }*/

    }

    free(items);
    return 0;
}

