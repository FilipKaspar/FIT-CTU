#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#endif /* __PROGTEST__ */


int kontrolaDne(int month, bool leapyear){
    if(month == 2){
        if(leapyear == true){
            return 29;
        }else{
            return 28;
        }
    }else{
        if(month < 8){
            if(month % 2 == 0){
                return 30;
            }else{
                return 31;
            }

        }else{
            if(month % 2 == 0){
                return 31;
            }else{
                return 30;
            }
        }
    }
}

bool prestupniRok(int year){

    if(year % 4000 == 0){
        return false;
    }else if(year % 400 == 0){
        return true;
    }else if(year % 100 == 0){
        return false;
    }else if(year % 4 == 0){
        return true;
    }
    return false;

}

int kontrolaVstupuDnu(int d1, int d2, int maxDnu1, int maxDnu2){
    if(d1 < 1 || d1 > maxDnu1 ||
       d2 < 1 || d2 > maxDnu2){
           printf("\nSpatne zadane vstupy dnu.\n");
            return 0;
       }
       return 1;
}

int kontrolaVstupuBezDnu(int y1, int m1, int d1, int h1, int i1,
                         int y2, int m2, int d2, int h2, int i2, bool prestupniy1, bool prestupniy2){
    int maxDnu1 = 0, maxDnu2 = 0;

    // Kontrola Vstupu bez dnu
    if(y1 < 1600 || m1 < 1 || m1 > 12  || h1 < 0 || h1 > 23 || i1 < 0 || i1 > 59 ||
       y2 < 1600 || m2 < 1 || m2 > 12  || h2 < 0 || h2 > 23 || i2 < 0 || i2 > 59){
        printf("\nSpatne zadane vstupy! Dny mozna spravne.\n");
        return 0;
    }

    // Urceni dne
    maxDnu1 = kontrolaDne(m1, prestupniy1);
    maxDnu2 = kontrolaDne(m2, prestupniy2);
    //printf("Mesic: %d Max dnu: %d\nMesic: %d Max dnu: %d\n", m1 , maxDnu1, m2, maxDnu2);

    // Kontrola pouze vstupu dne
    if(d1 < 1 || d1 > maxDnu1 ||
       d2 < 1 || d2 > maxDnu2){
           printf("\nSpatne zadane vstupy dnu.\n\n");
            return 0;
       }

    if(y2 < y1){
        printf("\nDruhy interval je driv nez prvni.\n\n");
            return 0;
    }else if(y2 == y1 && m2 < m1){
        printf("\nDruhy interval je driv nez prvni.\n\n");
            return 0;
    }else if(y2 == y1 && m2 == m1 && d2 < d1){
        printf("\nDruhy interval je driv nez prvni.\n\n");
            return 0;
    }else if(y2 == y1 && m2 == m1 && d2 == d1 && h2 < h1){
        printf("\nDruhy interval je driv nez prvni.\n\n");
            return 0;
    }else if(y2 == y1 && m2 == m1 && d2 == d1 && h2 == h1 && i2 < i1){
        printf("\nDruhy interval je driv nez prvni.\n\n");
            return 0;
    }

    return 1;
}

int monthToDay(int m, bool prestupni){
    int month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int d = 0;

    if(prestupni){
        month[1] = 29;
    }

    for(int x  = 0; x < m-1; x++){
            d += month[x];
           // printf("Cyklu: %d   Celkem dnu: %d\n", x, d);
    }

    return d;
}

int dayCost(int hour, int min){

    // Prevedeni na zacatky hodin
    int prvniCisloHod = hour / 10;
    int druheCisloHod = hour % 10;

    int prvniCisloMin = hour / 10;
    int druheCisloMin = hour % 10;

    // Konstanty
    int khod = 12200;
    int kmin = 200;

    int tonumber[10] = {4, 5, 2, 3, 3, 1, 5, 4, 1, 2};

	int totalCena = 0;

    int hodinaTed = 0;
    int poziceHod = 0;
    prvniCisloHod = 0; druheCisloHod = 0;
    // Vypocet hodin
    while(hodinaTed < hour && hour != 0){
        if(hodinaTed != hour){
            totalCena += khod;
        }
        poziceHod += tonumber[prvniCisloHod];
        totalCena += tonumber[prvniCisloHod];
        prvniCisloHod++;
        if(prvniCisloHod % 10 == 0){
            totalCena+=tonumber[druheCisloHod];
            poziceHod += tonumber[druheCisloHod];
            druheCisloHod++;
            prvniCisloHod = 0;
        }
        hodinaTed++;
    }
    int minutaTed = 0;
    prvniCisloMin = 0;
    druheCisloMin = 0;
    // Vypocet Minut
    while(minutaTed < min){
        if(minutaTed != min){
            totalCena += kmin;
        }
        totalCena += tonumber[prvniCisloMin];
        prvniCisloMin++;
        if(prvniCisloMin % 10 == 0){
            totalCena+=tonumber[druheCisloMin];
            druheCisloMin++;
            prvniCisloMin = 0;
        }
        minutaTed++;
    }

    return totalCena;
}


long long int vypocetJoulu(int y1, int m1, int d1, int h1, int i1, int y2, int m2, int d2, int h2, int i2, bool prestupniy1, int prestupniy2){
    long long int interval1 = 0, interval2 = 0;
    int kden = 292886;
    long long int celkemDny = 0, rozdilVeDni = 0;
    long long int dnyVLetech = 0;

   // printf("h1prvni: %d  h2prvni: %d\nh1druhy: %d  h2druhy: %d\ni1prvni: %d  ni2prvni: %d\ni1druhy: %d  i2druhy: %d\n\n", h1prvni, h2prvni, h1druhy, h2druhy, i1prvni, i2prvni, i1druhy, i2druhy);

   // printf("\nJe 1 Rok prestupny? %d\nJe 2 Rok prestupny? %d\n\n", prestupniy1, prestupniy2);

    d1 += (monthToDay(m1,prestupniy1));
    d2 += (monthToDay(m2,prestupniy2));
   // printf("\nDny z mesicu1: %d\nDny z mesicu2: %d\n", d1, d2);

    for(int x = y1+1; x<y2; x++){
        if(prestupniRok(x)){
            dnyVLetech += 366;
        }else{
            dnyVLetech += 365;
        }
    }
    if(y1 == y2){
        interval1 = (d1*kden);
        interval2 = (d2*kden);
        celkemDny = interval2 - abs(interval1);
    }else{
        if(prestupniy1){
            interval1 = (366*kden)-(d1*kden);
        }else{
            interval1 = (365*kden)-(d1*kden);
        }
        interval2 = (d2*kden);
        celkemDny = interval2 + interval1;
    }



   // printf("------Days in Interval 1: %d\n", interval1/kden);
   // printf("------Days in Interval 2: %d\n", interval2/kden);

    int prvniIntervalOd0 = dayCost(h1, i1);
    int druhyIntervalOd0 = dayCost(h2, i2);
   // printf("Prvni Interval v 1 dni: %d\nDruhy Interval v 2 dni: %d\n", prvniIntervalOd0, druhyIntervalOd0);
   // printf("\nRozdil dnu: %d\n", celkemDny);

    if(prvniIntervalOd0 > druhyIntervalOd0){
        celkemDny += druhyIntervalOd0-prvniIntervalOd0;
    }else{
        celkemDny += abs(prvniIntervalOd0-druhyIntervalOd0);
    }

   // printf("\nRozdil dnu + dodatek za jeden den: %d\n", celkemDny);
    return celkemDny + rozdilVeDni + (dnyVLetech*kden);
}

int energyConsumption ( int y1, int m1, int d1, int h1, int i1,
                        int y2, int m2, int d2, int h2, int i2, long long int * consumption )
{
    bool prestupniy1 = false,  prestupniy2 = false;

   // y1 = 1700, y2 = 1904, m1 = 2, m2 = 12, d1 = 2, d2 = 29, h1 = 23, h2 = 23, i1 = 59, i2 = 2;

    // Prestupni roky
    prestupniy1 = prestupniRok(y1);
    prestupniy2 = prestupniRok(y2);

   // printf("Rok: %lld Prestupni: %lld\nRok: %d Prestupni: %lld\n", y1 ,prestupniy1, y2, prestupniy2);

    // Kontrola vstupu
    if(kontrolaVstupuBezDnu(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2, prestupniy1, prestupniy2) == 0)return 0;

    // Vypocet sekund z obou intervalu
    *consumption = vypocetJoulu(y1, m1, d1, h1, i1, y2, m2, d2, h2, i2, prestupniy1, prestupniy2);
   // printf("Vysledek Joulu: %lld\n\n", *consumption);

    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  long long int consumption;

assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                               2021, 10,  5, 11, 20, &consumption ) == 1
           && consumption == 81106033LL );

  return 0;
}
#endif /* __PROGTEST__ */
