#include<stdio.h>
#include <math.h>

int main (void){

        long long int pozice = 0, soustava = 0;
        long long int vysledekDEC = 0;
        long long int velikostcisla = 0, presnapozice = 0, pridavekcisla = 0;
        long long int prvotni = 0, posledni = 0, Maxpozic = 0;
        long long int proEOF = 0;

        printf("Pozice a soustava:\n");

        while((proEOF = scanf("%lld %lld", &pozice, &soustava)) != EOF){
           if(proEOF != 2 || soustava < 2 || soustava > 36 || pozice < 0){
            printf("Nespravny vstup.\n");
            return 1;
            }

            if(pozice == 0){
                velikostcisla = 1;
            }

            if(pozice >=0 && pozice <=(soustava-1)){
                char finalnivysledek[100];
                char vysledek = '0';
                switch(pozice){
                    case(0):vysledek = '0';break;
                    case(1):vysledek = '1';break;
                    case(2):vysledek = '2';break;
                    case(3):vysledek = '3';break;
                    case(4):vysledek = '4';break;
                    case(5):vysledek = '5';break;
                    case(6):vysledek = '6';break;
                    case(7):vysledek = '7';break;
                    case(8):vysledek = '8';break;
                    case(9):vysledek = '9';break;
                    case(10):vysledek = 'a';break;
                    case(11):vysledek = 'b';break;
                    case(12):vysledek = 'c';break;
                    case(13):vysledek = 'd';break;
                    case(14):vysledek = 'e';break;
                    case(15):vysledek = 'f';break;
                    case(16):vysledek = 'g';break;
                    case(17):vysledek = 'h';break;
                    case(18):vysledek = 'i';break;
                    case(19):vysledek = 'j';break;
                    case(20):vysledek = 'k';break;
                    case(21):vysledek = 'l';break;
                    case(22):vysledek = 'm';break;
                    case(23):vysledek = 'n';break;
                    case(24):vysledek = 'o';break;
                    case(25):vysledek = 'p';break;
                    case(26):vysledek = 'q';break;
                    case(27):vysledek = 'r';break;
                    case(28):vysledek = 's';break;
                    case(29):vysledek = 't';break;
                    case(30):vysledek = 'u';break;
                    case(31):vysledek = 'v';break;
                    case(32):vysledek = 'w';break;
                    case(33):vysledek = 'x';break;
                    case(34):vysledek = 'y';break;
                    case(35):vysledek = 'z';break;
                }
                finalnivysledek[0] = vysledek;

                printf("%c\n", finalnivysledek[0]);
                printf("^\n");
            }
            else{
                for(long long int x = 1; 1 < 2; x++){
                    posledni = posledni + (((soustava-1)*((long long int)pow(soustava,x-1)))*x);
                    if(pozice <= posledni){
                        velikostcisla = x;
                        break;
                    }
                }
                for(long long int i = 1; i < velikostcisla; i++){
                    prvotni = prvotni + (((soustava-1)*((long long int)pow(soustava,i-1)))*i);
                }

               // printf("\nCIFER: %lld\n", velikostcisla);

                Maxpozic = (long long int)pow(soustava,velikostcisla-1);
               // printf("RADOVE: %d\n", Maxpozic);

               // printf("Prvotni: %lld\n", prvotni);
               // printf("Posledni: %d\n", posledni);

                pozice =  pozice - (prvotni+1);
                presnapozice = pozice;
                presnapozice = presnapozice % velikostcisla;

                pridavekcisla = (pozice / velikostcisla);

            // printf("\nSoustava: %d\nPomocny pridavekF: %d\nFinalni pridavek: %d\n", soustava, presnapozice, pridavekcisla);

            vysledekDEC = Maxpozic + pridavekcisla;
            // printf("Mezivysledek v 10 soustave: %u\n", vysledekDEC);

            long long int mezivypocty = vysledekDEC, zbytek = 0, k = 0;
            char finalnivysledek[2000];
            char vysledek = '0';

            while(mezivypocty != 0){

                zbytek = mezivypocty % soustava;
                mezivypocty = mezivypocty / soustava;


               // vysledek = (zbytek * (pow(10,k))) + vysledek;
                switch(zbytek){
                    case(0):vysledek = '0';break;
                    case(1):vysledek = '1';break;
                    case(2):vysledek = '2';break;
                    case(3):vysledek = '3';break;
                    case(4):vysledek = '4';break;
                    case(5):vysledek = '5';break;
                    case(6):vysledek = '6';break;
                    case(7):vysledek = '7';break;
                    case(8):vysledek = '8';break;
                    case(9):vysledek = '9';break;
                    case(10):vysledek = 'a';break;
                    case(11):vysledek = 'b';break;
                    case(12):vysledek = 'c';break;
                    case(13):vysledek = 'd';break;
                    case(14):vysledek = 'e';break;
                    case(15):vysledek = 'f';break;
                    case(16):vysledek = 'g';break;
                    case(17):vysledek = 'h';break;
                    case(18):vysledek = 'i';break;
                    case(19):vysledek = 'j';break;
                    case(20):vysledek = 'k';break;
                    case(21):vysledek = 'l';break;
                    case(22):vysledek = 'm';break;
                    case(23):vysledek = 'n';break;
                    case(24):vysledek = 'o';break;
                    case(25):vysledek = 'p';break;
                    case(26):vysledek = 'q';break;
                    case(27):vysledek = 'r';break;
                    case(28):vysledek = 's';break;
                    case(29):vysledek = 't';break;
                    case(30):vysledek = 'u';break;
                    case(31):vysledek = 'v';break;
                    case(32):vysledek = 'w';break;
                    case(33):vysledek = 'x';break;
                    case(34):vysledek = 'y';break;
                    case(35):vysledek = 'z';break;
                }

               finalnivysledek[velikostcisla-k-1] = vysledek;
                k++;
            }


            for(long long int n = 0; n < velikostcisla; n++){
                printf("%c", finalnivysledek[n]);
            }
            printf("\n");

            for(long long int z = 0; z < presnapozice; z++){
                printf(" ");
            }
            printf("^\n");
            }
        pozice = 0, soustava = 0,vysledekDEC = 0, velikostcisla = 0, presnapozice = 0, pridavekcisla = 0, prvotni = 0, posledni = 0, Maxpozic = 0,proEOF = 0;
        }


    return 0;
}
