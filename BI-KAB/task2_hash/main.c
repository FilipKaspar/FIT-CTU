#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

#endif /* __PROGTEST__ */

int checkZeroes(const unsigned char *hash, int bits, int hash_size){
    int x = 0;
    for (int k = 0; k < hash_size; k++) {
        for (int i = 7; i >= 0; i--) {
            int tmp = ((hash[k] >> i) & 1);
            if(x < bits) {
                if(tmp == 1) return 0;
            }
            else{
                return 1;
            }
            x++;
        }
    }

    return 1;
}

int createHash(unsigned char *buffer, unsigned char **hash, int bits, const EVP_MD * type, int hash_size, int buffer_size){
    EVP_MD_CTX * ctx;
    unsigned int length;

    if (!type) {
        return 0;
    }

    ctx = EVP_MD_CTX_new();
    if (ctx == NULL)
        return 0;

    /* Hash the text */
    if (!EVP_DigestInit_ex(ctx, type, NULL))
        return 0;

    if (!EVP_DigestUpdate(ctx, buffer, buffer_size))
        return 0;

    if (!EVP_DigestFinal_ex(ctx, *hash, &length))
        return 0;

    EVP_MD_CTX_free(ctx);
    return checkZeroes(*hash, bits, hash_size);
}

int generateText(unsigned char *buffer, const int buffer_size){
    if(RAND_bytes(buffer, buffer_size) == 0) return 0;
    // Může např. když špatně inicializuje generátor náhodnejch čísel atd. v takovém případě vrátí 0
    // Pokud proběhl úspěšně vrátí 1
    return 1;
}

char * convertToHex(const unsigned char * text, int size){
    const char hexa[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    char *res = (char*) malloc(((2 * size) + 1) * sizeof(char));

    for (int i = 0; i < size; i++) {
        const unsigned char byte = text[i];
        size_t half1 = byte >> 4;
        size_t half2 = byte & 0xf;

        res[i*2] = hexa[half1];
        res[i*2+1] = hexa[half2];
    }
    res[2*size] = '\0';

    return res;
}

int findHashEx (int bits, char ** message, char ** hash, const char * hashFunction) {
    /* TODO or use dummy implementation */
    const EVP_MD * type = EVP_get_digestbyname(hashFunction);
    // Pokud tomu dám dobrou hash funkci tak to vytvoří strukturu, která bude odpovídat dané hash funkci
    // Pokud tomu dám špatnou/neplatnou hash funkci tak to vrátí nullptr
    if(type == NULL) return 0;
    const int hash_size = EVP_MD_size(type);
    // Zde už bude type správný tudíž by nám to korektně mělo vrátit size hashe

    if(bits < 0 || bits > hash_size) return 0;


    OpenSSL_add_all_digests();
    const int buffer_size = 64; // Text length
    if(buffer_size == 0) return 0;
    unsigned char buffer[buffer_size] = {};

    unsigned char *hashs = (unsigned char*) malloc(hash_size*sizeof(unsigned char));
    // Ano malloc může selhat, např. když OS už nemá dostatek paměti na přidělení. Pokud tak nastane malloc vrátí nullptr.
    if(hashs == NULL){
        free(hashs);
        return 0;
    }

    if(generateText(buffer, buffer_size) == 0){
        free(hashs);
        return 0;
    }

    while(!createHash(buffer, &hashs, bits, type, hash_size, buffer_size)){
        buffer[rand() % buffer_size]++;
        //Rand zaračuje číslo od 0 do alespoň 32767. Pseudo náhodná čísla záleží na počátečním hodnotě seedu
        //Pokud by vadila hodnota seedu, můžeme použít funkci srand(). Pro naše účely si ale myslím stačí rand
        //Ohledně buffer_size, nemusí tam ani být -1. Ovšem kdybychom modulily 0 tak nejspíš dostaneme error
        //Protože bychom dělili nulou, ovšem bez toho -1 bychom tedy museli mít zprávu délky nula, což je už samo o sobě
        //divné. Nicméně můžeme ošetřit podmínkou.
    }

    *hash = convertToHex(hashs, hash_size);
    *message = convertToHex(buffer, buffer_size);


    //cout << *hash << endl;

    free(hashs);
    return 1;
}

int findHash (int bits, char ** message, char ** hash) {
    /* TODO: Your code here */
    return findHashEx(bits, message, hash, "sha512");
}

#ifndef __PROGTEST__

int checkHash(int bits, char * hexString) {
    for(int i = 0; i < bits; i++){
        if(hexString[i] == 1) return 0;
    }
    return 1;
}

int main (void) {
    //checkMsg();
    char * message, * hash;

    /*clock_t start, end;

    start = clock();*/

    // Calculating total time taken by the program.
    assert(findHash(8, &message, &hash) == 1);
    /*end = clock();

    // Calculating total time taken by the program.
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << fixed
         << time_taken << setprecision(5);
    cout << " sec " << endl;*/

    assert(message && hash && checkHash(0, hash));
    free(message);
    free(hash);
    assert(findHash(1, &message, &hash) == 1);
    assert(message && hash && checkHash(1, hash));
    free(message);
    free(hash);
    assert(findHash(2, &message, &hash) == 1);
    assert(message && hash && checkHash(2, hash));
    free(message);
    free(hash);
    assert(findHash(16, &message, &hash) == 1);
    assert(message && hash && checkHash(16, hash));
    free(message);
    free(hash);
    assert(findHash(-1, &message, &hash) == 0);
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

