#include<stdio.h>

void zacatekkonec(int sirka){
    printf("+");
    for(int i = 0; i < sirka; i++){
        printf("-");
    }
    printf("+\n");
}

int vstupy (){
    int vstup = 0;
    if(scanf("%d", &vstup) != 1 || vstup <= 0)return -1;

    return vstup;
}

int main (void){

    int poli = 0, velikostpole = 0,mezercelkem;

    printf("Zadejte pocet poli:\n");
    poli = vstupy();
        if(poli == -1){
            printf("Nespravny vstup.\n");
            return 1;
        }


    printf("Zadejte velikost pole:\n");
    velikostpole = vstupy();
        if(velikostpole == -1){
            printf("Nespravny vstup.\n");
            return 1;
        }

    mezercelkem = poli * velikostpole;

    zacatekkonec(mezercelkem);

    int z = 0;
    for(int i = 0; i < poli; i++){
        for(int k = 0; k < velikostpole; k++){
            printf("|");
            if(i % 2 == 0){
                z = 0;
                for(int x = 0; x < mezercelkem; x++){
                    if(z < velikostpole){
                        printf(" ");

                    } else{
                        printf("X");
                        if(z == velikostpole + (velikostpole - 1)){
                            z = -1;
                        }
                    }
                    z++;
                }
            } else {
                z = velikostpole;
                for(int x = 0; x < mezercelkem; x++){
                    if(z < velikostpole){
                        printf(" ");

                    } else{
                        printf("X");
                        if(z == velikostpole + (velikostpole - 1)){
                            z = -1;
                        }
                    }
                    z++;
                }
            }
            printf("|\n");
        }

    }
    zacatekkonec(mezercelkem);



    return 0;
}
