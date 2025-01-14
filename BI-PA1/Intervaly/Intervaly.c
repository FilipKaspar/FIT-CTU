#include<stdio.h>

int main(void){

    int h,m,s,ms;
    int h1,m1,s1,ms1;
    char xms[5],xms1[5];
    int cms, cms1,vysledek;
    int hod,min,sek,mils;

    printf("Zadejte cas t1:\n");
    if((scanf(" %d : %d : %d , %s", &h, &m, &s, xms)) != 4
       || h < 0 || m < 0 || s < 0
       || h > 23 || m > 59 || s > 59 ){

        printf("Nespravny vstup.\n");
        return 1;
    }

    sscanf(xms, "%d", &ms);
    printf("%d", ms);
    if(ms > 999 || ms < 0){

        printf("Nespravny vstup.\n");
        return 1;

    }

    printf("Zadejte cas t2:\n");
    if((scanf(" %d : %d : %d , %s", &h1, &m1, &s1, xms1)) != 4
       || h1 < 0 || m1 < 0 || s1 < 0
       || h1 < h || m1 > 59 || s1 > 59 || h1 > 23
       || (h1 == h && m1 < m) || (h1 == h && m1 == m && s1 < s)){

        printf("Nespravny vstup.\n");
        return 1;

    }

    sscanf(xms1, "%d", &ms1);

    if(ms1 < 0 || ms1 > 999 || (h1 == h && m1 == m && s1 == s && ms1 < ms)){

        printf("Nespravny vstup.\n");
        return 1;

    }
   // printf("\n%s,   %s\n", xms, xms1);




   // printf("\n%s,   %s\n", xms, xms1);



   // printf("Ahoj: %d, %d\n", ms,ms1);

    if(xms[0] != '0'){

        if(ms < 10){
            ms = ms *100;
        }else if(ms < 100){
            ms = ms * 10;
        }


    }
    if( xms1[0] != '0'){

        if(ms1 < 10){
            ms1 = ms1 *100;
        }else if(ms1 < 100){
            ms1 = ms1 * 10;
        }

    }

    //printf("Ahoj: %d, %d\n", ms,ms1);

    //Na ms prvniho

    cms = (h*60*60*1000)+(m*60*1000)+(s*1000)+ms;

    //Na ms druheho

    cms1 = (h1*60*60*1000)+(m1*60*1000)+(s1*1000)+ms1;

    //Odecet

    vysledek = cms1-cms;

    //Zpatky na cas

    mils = vysledek%1000;
    sek = (vysledek - mils)/1000;
    while(sek > 60){

        sek-= 60;

    }
    if(sek == 60){
            sek = 0;
    }
    min = (vysledek - mils)/1000/60;
    while(min > 60){

        min-= 60;

    }
    if(min == 60){

        min = 0;

    }
    hod = (vysledek - mils)/1000/60/60;

    printf("Doba: %2d:%.2d:%.2d,%.3d\n", hod,min,sek,mils);










    return 0;
}
