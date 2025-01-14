#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/pem.h>

using namespace std;

#endif /* __PROGTEST__ */

void sealDelete(EVP_CIPHER_CTX * ctx, EVP_PKEY * pubkey, const char * outFile, unsigned char ** key){
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(pubkey);
    remove(outFile);
    delete[] key;
}

bool seal( const char * inFile, const char * outFile, const char * publicKeyFile, const char * symmetricCipher)
{
    if (!inFile || !outFile || !publicKeyFile || !symmetricCipher) return false;

    const EVP_CIPHER * cipher = EVP_get_cipherbyname(symmetricCipher);
    if(cipher == nullptr) return false;

    FILE *fp = fopen(publicKeyFile, "r");
    if(!fp) return false;


    EVP_PKEY * pubkey = PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
    fclose(fp);
    if(!pubkey){
        EVP_PKEY_free(pubkey);
        return false;
    }

    // Open and decrypt information

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    ifstream in_file(inFile, ios::binary);
    ofstream out_file(outFile, ios::out | ios::binary);

    if(!ctx || !in_file.is_open() || !out_file.is_open()){
        remove(outFile);
        EVP_CIPHER_CTX_free(ctx);
        EVP_PKEY_free(pubkey);
        return false;
    }

    EVP_CIPHER_CTX_init(ctx);
    auto * key = new unsigned char[EVP_PKEY_size(pubkey)];
    int key_len = EVP_CIPHER_key_length(cipher);
    shared_ptr<uint8_t[]> iv(new uint8_t[EVP_CIPHER_iv_length(cipher)]);
    int nid = EVP_CIPHER_nid(cipher);
    if (EVP_SealInit(ctx, cipher, &key, &key_len, iv.get(), &pubkey, 1) != 1
        || !out_file.write(reinterpret_cast<char*>(&nid), sizeof(int))
        || !out_file.write(reinterpret_cast<char*>(&key_len), sizeof(int))
        || !out_file.write(reinterpret_cast<char *>(key), key_len)
        || !out_file.write(reinterpret_cast<char *>(iv.get()), EVP_CIPHER_iv_length(cipher))){
        sealDelete(ctx, pubkey, outFile, &key);
        return false;
    }

    const size_t chunk_size = 500;
    uint8_t imported_data[chunk_size];
    uint8_t encrypted_data[chunk_size + EVP_MAX_BLOCK_LENGTH];
    int out_len = 0;

    while(true) {
        in_file.read(reinterpret_cast<char*>(imported_data), chunk_size);
        if(!in_file.good() && !in_file.eof()){
            sealDelete(ctx, pubkey, outFile, &key);
            return false;
        }
        const long bytes_read = in_file.gcount();
        if (bytes_read == 0) break;

        if (EVP_SealUpdate(ctx, encrypted_data, &out_len, imported_data, (int)bytes_read) != 1
            || !out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
            sealDelete(ctx, pubkey, outFile, &key);
            return false;
        }
    }

    if (EVP_SealFinal(ctx, encrypted_data, &out_len) != 1
        || !out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
        sealDelete(ctx, pubkey, outFile, &key);
        return false;
    }
    delete[] key;

    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(pubkey);

    out_file.close();
    in_file.close();

    return true;
}

void openDelete(EVP_CIPHER_CTX * ctx, EVP_PKEY * privateKey, const char * outFile, unsigned char * key, unsigned char * iv){
    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(privateKey);
    remove(outFile);
    delete[] key;
    delete[] iv;
}

bool open( const char * inFile, const char * outFile, const char * privateKeyFile)
{
    if (!inFile || !outFile || !privateKeyFile) return false;

    FILE *fp = fopen(privateKeyFile, "r");
    if(!fp) return false;

    EVP_PKEY * privatekey;
    privatekey = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);

    ifstream in_file(inFile, ios::binary);
    ofstream out_file(outFile, ios::out | ios::binary);
    int nid;

    if(!in_file.is_open() || !out_file.is_open() || !privatekey || !in_file.read(reinterpret_cast<char *>(&nid), sizeof(int))){
        remove(outFile);
        EVP_PKEY_free(privatekey);
        return false;
    }

    const EVP_CIPHER * cipher = EVP_get_cipherbynid(nid);

    auto key = new unsigned char[EVP_PKEY_size(privatekey)];
    int key_len = -1;
    auto iv = new unsigned char [EVP_CIPHER_iv_length(cipher)];

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    if(!cipher
       || !in_file.read(reinterpret_cast<char *>(&key_len), sizeof(int))
       || key_len == -1
       || !in_file.read(reinterpret_cast<char *>(key), key_len)
       || !in_file.read(reinterpret_cast<char *>(iv), EVP_CIPHER_iv_length(cipher))){
        openDelete(ctx, privatekey, outFile, key, iv);
        return false;
    }

    // Open and decrypt information

    if (!ctx || EVP_OpenInit(ctx, cipher, reinterpret_cast<const unsigned char *>(key), key_len,
                     reinterpret_cast<const unsigned char *>(iv), privatekey) != 1){
        openDelete(ctx, privatekey, outFile, key, iv);
        return false;
    }

    const size_t chunk_size = 500;
    uint8_t imported_data[chunk_size];
    uint8_t encrypted_data[chunk_size + EVP_MAX_BLOCK_LENGTH];
    int out_len = 0;

    while(true) {
        in_file.read(reinterpret_cast<char*>(imported_data), chunk_size);
        if(!in_file.good() && !in_file.eof()){
            openDelete(ctx, privatekey, outFile, key, iv);
            return false;
        }
        const long bytes_read = in_file.gcount();
        if (bytes_read == 0) break;

        if (EVP_OpenUpdate(ctx, encrypted_data, &out_len, imported_data, (int)bytes_read) != 1
            || !out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
            openDelete(ctx, privatekey, outFile, key, iv);
            return false;
        }
    }

    if (EVP_OpenFinal(ctx, encrypted_data, &out_len) != 1
        || !out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
        openDelete(ctx, privatekey, outFile, key, iv);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    EVP_PKEY_free(privatekey);

    out_file.close();
    in_file.close();
    delete[] key;
    delete[] iv;
    return true;
}



#ifndef __PROGTEST__

int main ( void )
{
    assert( seal("fileToEncrypt", "sealed.bin", "PublicKey.pem", "aes-128-cbc") );
    assert( open("sealed.bin", "openedFileToEncrypt", "PrivateKey.pem") );

    assert( open("sealed_sample.bin", "opened_sample.txt", "PrivateKey.pem") );

    return 0;
}

#endif /* __PROGTEST__ */

