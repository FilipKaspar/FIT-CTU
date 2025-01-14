#include<stdio.h>
#include<math.h>


//Funkce pro input


int main(void){

    double a = 1,b = 1,c = 1, s;
    double area1, perimeter1, area2, perimeter2;
    char obrazec1, obrazec2;

    printf("Obrazec #1\n");

    scanf(" %c ", &obrazec1);

    switch(obrazec1){
        case'T':{
            if(scanf("%lf %lf %lf", &a, &b, &c) != 3 || a + b <= c || a + c <= b || c + b <= a
               || (fabs((a + b) - c) <= 1e-9*fabs(a + b + c))){
                printf("Nespravny vstup.\n");
                return 1;
            }
            s = (a + b + c) / (double)2;
            area1 = sqrt(s*((s-a)*(s-b)*(s-c)));
            perimeter1 = a + b + c;
            break;
        }
        case'R':{
            if(scanf("%lf %lf", &a, &b) != 2 || a <= 0 || b <= 0 || a == b){
                printf("Nespravny vstup.\n");
                return 1;
            }
            area1 = a * b;
            perimeter1 = (double)2*(a + b);
            break;
        }
        case'S':{
            if(scanf("%lf", &a) != 1 || a <= 0){
                printf("Nespravny vstup.\n");
                return 1;
            }
            area1 = a * a;
            perimeter1 = (double)4 * a;
            break;
        }
        default:{
            printf("Nespravny vstup.\n");
            return 1;
        }
    }

    printf("Obrazec #2\n");

    scanf(" %c ", &obrazec2);

    switch(obrazec2){
        case'T':{
            if(scanf("%lf %lf %lf", &a, &b, &c) != 3 || a + b <= c || a + c <= b || c + b <= a
               || (fabs((a + b) - c) <= 1e-9*fabs(a + b + c))){
                printf("Nespravny vstup.\n");
                return 1;
            }
            s = (a + b + c) / (double)2;
            area2 = sqrt(s*((s-a)*(s-b)*(s-c)));
            perimeter2 = a + b + c;

            if( obrazec1 == 'T'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: trojuhelnik #1 = trojuhelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: trojuhelnik #1 < trojuhelnik #2\n");

                }else{
                    printf("Obvod: trojuhelnik #1 > trojuhelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: trojuhelnik #1 = trojuhelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: trojuhelnik #1 < trojuhelnik #2\n");

                }else{
                    printf("Obsah: trojuhelnik #1 > trojuhelnik #2\n");

                }

            } else if (obrazec1 == 'R'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: obdelnik #1 = trojuhelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: obdelnik #1 < trojuhelnik #2\n");

                }else{
                    printf("Obvod: obdelnik #1 > trojuhelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: obdelnik #1 = trojuhelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: obdelnik #1 < trojuhelnik #2\n");

                }else{
                    printf("Obsah: obdelnik #1 > trojuhelnik #2\n");

                }

            }else{
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: ctverec #1 = trojuhelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: ctverec #1 < trojuhelnik #2\n");

                }else{
                    printf("Obvod: ctverec #1 > trojuhelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: ctverec #1 = trojuhelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: ctverec #1 < trojuhelnik #2\n");

                }else{
                    printf("Obsah: ctverec #1 > trojuhelnik #2\n");

                }


            }
            break;
        }
        case'R':{
            if(scanf("%lf %lf", &a, &b) != 2 || a <= 0 || b <= 0 || a == b){
                printf("Nespravny vstup.\n");
                return 1;
            }
            area2 = a * b;
            perimeter2 = (double)2*(a + b);

            if( obrazec1 == 'T'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: trojuhelnik #1 = obdelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: trojuhelnik #1 < obdelnik #2\n");

                }else{
                    printf("Obvod: trojuhelnik #1 > obdelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: trojuhelnik #1 = obdelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: trojuhelnik #1 < obdelnik #2\n");

                }else{
                    printf("Obsah: trojuhelnik #1 > obdelnik #2\n");

                }

            } else if (obrazec1 == 'R'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: obdelnik #1 = obdelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: obdelnik #1 < obdelnik #2\n");

                }else{
                    printf("Obvod: obdelnik #1 > obdelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: obdelnik #1 = obdelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: obdelnik #1 < obdelnik #2\n");

                }else{
                    printf("Obsah: obdelnik #1 > obdelnik #2\n");

                }

            }else{
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: ctverec #1 = obdelnik #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: ctverec #1 < obdelnik #2\n");

                }else{
                    printf("Obvod: ctverec #1 > obdelnik #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: ctverec #1 = obdelnik #2\n");

                } else if (area1 < area2){
                    printf("Obsah: ctverec #1 < obdelnik #2\n");

                }else{
                    printf("Obsah: ctverec #1 > obdelnik #2\n");

                }


            }

            break;
        }
        case'S':{
            if(scanf("%lf", &a) != 1 || a <= 0){
                printf("Nespravny vstup.\n");
                return 1;
            }

            area2 = a * a;
            perimeter2 = (double)4 * a;

            if( obrazec1 == 'T'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: trojuhelnik #1 = ctverec #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: trojuhelnik #1 < ctverec #2\n");

                }else{
                    printf("Obvod: trojuhelnik #1 > ctverec #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: trojuhelnik #1 = ctverec #2\n");

                } else if (area1 < area2){
                    printf("Obsah: trojuhelnik #1 < ctverec #2\n");

                }else{
                    printf("Obsah: trojuhelnik #1 > ctverec #2\n");

                }

            } else if (obrazec1 == 'R'){
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: obdelnik #1 = ctverec #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: obdelnik #1 < ctverec #2\n");

                }else{
                    printf("Obvod: obdelnik #1 > ctverec #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: obdelnik #1 = ctverec #2\n");

                } else if (area1 < area2){
                    printf("Obsah: obdelnik #1 < ctverec #2\n");

                }else{
                    printf("Obsah: obdelnik #1 > ctverec #2\n");

                }

            }else{
                if(fabs(perimeter1 - perimeter2) <= 1e-9*fabs(perimeter1 + perimeter2)){
                    printf("Obvod: ctverec #1 = ctverec #2\n");

                } else if (perimeter1 < perimeter2){
                    printf("Obvod: ctverec #1 < ctverec #2\n");

                }else{
                    printf("Obvod: ctverec #1 > ctverec #2\n");

                }

                if(fabs(area1 - area2) <= 1e-9*fabs(area1 + area2)){
                    printf("Obsah: ctverec #1 = ctverec #2\n");

                } else if (area1 < area2){
                    printf("Obsah: ctverec #1 < ctverec #2\n");

                }else{
                    printf("Obsah: ctverec #1 > ctverec #2\n");

                }


            }

            break;
        }
        default:{
            printf("Nespravny vstup.\n");
            return 1;
        }
    }



    return 0;
}
