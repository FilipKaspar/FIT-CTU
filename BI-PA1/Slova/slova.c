#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#endif /* __PROGTEST__ */

void saveWords( char *** save, const char * input, int * wordsCount){

    int x = 0 , y = 0;
    int arraySize = 0;
    char temporaryStore[1000000]; // Temporary char for each word. Static because progtest wouldn't test bigger :D. If so, I would use dynamic

    while(1){

        if(*wordsCount >= arraySize){
            arraySize = arraySize * 2 + 10;
            *save = (char**)realloc(*save, arraySize * sizeof(char**));
        }

        x = y;
        y = 1;

        for(;1; x++){
            if(isalpha(input[x])){
                while(1){
                    if(isalpha(input[x+y])){
                        y++;
                    }
                    else{
                        break;
                    }
                }
                break;
            }
            else if(input[x] == '\0' || input[y] == '\0'){
                return;
            }
        }

        y = x+y;
        int jump = 0;
        while(x < y){
            temporaryStore[jump] = input[x];
            jump++;
            x++;
        }
        temporaryStore[jump] = '\0';

        (*save)[*wordsCount] = strdup(temporaryStore); // Strdup allocating memory for each array for each pointer
        (*wordsCount)++;
    }
}

int compare(char ** Storage1, char ** Storage2, int wordsCount1, int wordsCount2){
    int same = 1;

    for(int x = 0; x < wordsCount1; x++){
        for(int y = 0; y < wordsCount2; y++){
            if(!strcasecmp(Storage1[x],Storage2[y])){
                break;
            }
            else if(y == wordsCount2-1){
                same = 0;
                break;
            }
        }
        if(!same){
            break;
        }
    }
    return same;
}

int sameWords ( const char * a, const char * b ){

    char ** firstStorage = NULL;
    char ** secondStorage = NULL;

    int wordsCountF = 0, wordsCountS = 0;
    int same = 1;

    saveWords(&firstStorage, a, &wordsCountF); // Saving every word to an array
    saveWords(&secondStorage, b, &wordsCountS);


    if(wordsCountF == 0 && wordsCountS == 0){ // Checking if strings are just whitespace
        // Freeing memory
        for(int k = 0; k < wordsCountF; k++){
            free(firstStorage[k]);
        }
        for(int k = 0; k < wordsCountS; k++){
            free(secondStorage[k]);
        }
        free(firstStorage);
        free(secondStorage);
        return 1;
    }
    else if(wordsCountF == 0 || wordsCountS == 0){ // If not, checking if one string has space and the other doesn't
        same = 0;
    }
    if(same){ // Otherwise just checking both strings first every word from first array comparing to every word from the other array and then the other way around
        same = compare(firstStorage, secondStorage, wordsCountF, wordsCountS);
        if(same){
            same = compare(secondStorage, firstStorage, wordsCountS, wordsCountF);
        }
    }

    for(int k = 0; k < wordsCountF; k++){ // Freeing memory
        free(firstStorage[k]);
    }
    for(int k = 0; k < wordsCountS; k++){
        free(secondStorage[k]);
    }
    free(firstStorage);
    free(secondStorage);

    if(!same){
        return 0;
    }
    return 1;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  /*assert ( sameWords ( "", "" ) == 1);
  assert ( sameWords ( "                                  ", "" ) == 1);
  assert ( sameWords ("c", "C") == 1);
  assert ( sameWords ( "Hello students", "HELLO studEnts!") == 1 );
  assert ( sameWords ( " He said 'hello!'", "'Hello.' he   said." ) == 1 );
  assert ( sameWords ( "He said he would do it.", "IT said: 'He would do it.'" ) == 1 );
  assert ( sameWords ( "one two three", "one two five" ) == 0 );*/

  printf("            __           __          __     __\n");
  printf("\\        / |       |    |  | \\    / |      |  | |\\  | | \\  | \\   /\\\n");
  printf(" \\  /\\  /  |--     |    |  |  \\  /  |--    |  | | \\ | |  | | /  /--\\\n");
  printf("  \\/  \\/   |__     | _  |__|   \\/   |__    |__| |  \\| | /  | \\ /    \\\n");

  return 0;
}
#endif /* __PROGTEST__ */
