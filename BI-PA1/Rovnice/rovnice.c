#include<stdio.h>
#include<math.h>

int main(void){

    double cislo1, cislo2, vysledek;
    double vypocetLstrany = 0;
    char operace;

    printf("Zadejte rovnici:\n");

    if(scanf("%lf %c %lf = %lf", &cislo1, &operace, &cislo2, &vysledek) != 4){

         printf("Nespravny vstup.\n");
         return 1;

    }

    if(operace != '/' && operace != '*' && operace != '+' && operace != '-' ){

        printf("Nespravny vstup.\n");
        return 1;

    }

    switch(operace){

        case '/': {

            if(cislo2 == 0){

                printf("Nespravny vstup.\n");
                return 1;

            }

            vypocetLstrany = cislo1 / cislo2;

            if(fabs(vypocetLstrany-vysledek) < 1e-9*fabs(vypocetLstrany+vysledek )){



            }else{

                vypocetLstrany = (int)vypocetLstrany;

            }
            break;

        }
        case '*': {

            vypocetLstrany = cislo1 * cislo2;
            break;

        }
        case '+': {

            vypocetLstrany = cislo1 + cislo2;
            break;

        }
        case '-': {

            vypocetLstrany = cislo1 - cislo2;
            break;

        }

    }

    if(fabs(vypocetLstrany-vysledek) <= 1e-9*fabs(vypocetLstrany+vysledek)){

        printf("Rovnice je spravne.\n");

    }else{

        printf("%.13g != %g\n", vypocetLstrany, vysledek);

    }

    return 0;

}
