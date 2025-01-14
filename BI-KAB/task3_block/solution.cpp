#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cassert>
#include <cstring>

#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

struct crypto_config
{
	const char * m_crypto_function;
	unique_ptr<uint8_t[]> m_key;
	unique_ptr<uint8_t[]> m_IV;
	size_t m_key_len;
	size_t m_IV_len;
};

#endif /* _PROGTEST_ */

bool encrypt_data ( const string & in_filename, const string & out_filename, crypto_config & config )
{
    //Check possible volovinky

    const EVP_CIPHER * cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if(cipher == nullptr) return false;

    int key_len = EVP_CIPHER_key_length(cipher);
    if(!config.m_key || (int)config.m_key_len < key_len){
        unique_ptr<uint8_t []> new_key= make_unique<uint8_t []>(key_len);
        if(RAND_bytes(new_key.get(), key_len) != 1) return false;
        config.m_key = move(new_key);
        config.m_key_len = key_len;
    }

    int iv_len = EVP_CIPHER_iv_length(cipher);
    if(iv_len != 0 && ((int)config.m_IV_len < iv_len || !config.m_IV)){
        unique_ptr<uint8_t []> new_IV= make_unique<uint8_t []>(iv_len);
        if(RAND_bytes(new_IV.get(), iv_len) != 1) return false;
        config.m_IV = move(new_IV);
        config.m_IV_len = iv_len;
    }

    // Open and decrypt information

    ifstream in_file(in_filename, ios::binary);
    if(!in_file.is_open()) return false;

    ofstream out_file(out_filename, ios::binary);
    if(!out_file.is_open()) return false;

    const size_t header_size = 18;
    uint8_t header[header_size];

    if (!in_file.read(reinterpret_cast<char*>(header), sizeof(header)) || in_file.gcount() != header_size) return false;
    if (!out_file.write(reinterpret_cast<const char*>(header), header_size)) return false;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) return false;
    EVP_CIPHER_CTX_init(ctx);
    if (EVP_EncryptInit_ex(ctx, cipher, nullptr, config.m_key.get(), config.m_IV.get()) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    const size_t chunk_size = 500;
    uint8_t imported_data[chunk_size];
    uint8_t encrypted_data[chunk_size + EVP_MAX_BLOCK_LENGTH];
    int out_len = 0;

    while(true) {
        in_file.read(reinterpret_cast<char*>(imported_data), chunk_size);
        if(!in_file.good() && !in_file.eof()){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        const long bytes_read = in_file.gcount();
        if (bytes_read == 0) break;

        if (EVP_EncryptUpdate(ctx, encrypted_data, &out_len, imported_data, (int)bytes_read) != 1){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (!out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
    }

    if (EVP_EncryptFinal_ex(ctx, encrypted_data, &out_len) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    if (!out_file.write(reinterpret_cast<const char*>(encrypted_data), out_len)){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);

    out_file.close();
    in_file.close();
    return true;
}

bool decrypt_data ( const string & in_filename, const string & out_filename, crypto_config & config )
{
    //Check possible volovinky

    if(config.m_key == nullptr) return false;
    if(config.m_crypto_function == nullptr) return false;
    const EVP_CIPHER * cipher = EVP_get_cipherbyname(config.m_crypto_function);
    if(cipher == nullptr) return false;

    int key_len = EVP_CIPHER_key_length(cipher);
    if((int)config.m_key_len < key_len) return false;
    int iv_len = EVP_CIPHER_iv_length(cipher);
    if((int)config.m_IV_len < iv_len) return false;
    if(iv_len != 0 && !config.m_IV) return false;

    // Open and decrypt information

    ifstream in_file(in_filename, ios::binary);
    if(!in_file.is_open()) return false;

    ofstream out_file(out_filename, ios::binary);
    if(!out_file.is_open()) return false;

    const size_t header_size = 18;
    uint8_t header[header_size];
    if (!in_file.read(reinterpret_cast<char*>(header), sizeof(header)) || in_file.gcount() != header_size) return false;
    if (!out_file.write(reinterpret_cast<const char*>(header), header_size)) return false;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (ctx == nullptr) return false;
    EVP_CIPHER_CTX_init(ctx);
    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, config.m_key.get(), config.m_IV.get()) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    const size_t chunk_size = 500;
    uint8_t imported_data[chunk_size];
    uint8_t decrypted_data[chunk_size + EVP_MAX_BLOCK_LENGTH];
    int out_len = 0;

    while(true) {
        in_file.read(reinterpret_cast<char*>(imported_data), chunk_size);
        if(in_file.fail() && !in_file.eof()){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        const long bytes_read = in_file.gcount();
        if (bytes_read == 0) break;

        if (EVP_DecryptUpdate(ctx, decrypted_data, &out_len, imported_data, (int)bytes_read) != 1){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }

        if (!out_file.write(reinterpret_cast<const char*>(decrypted_data), out_len)){
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
    }

    if (EVP_DecryptFinal_ex(ctx, decrypted_data, &out_len) != 1){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    if (!out_file.write(reinterpret_cast<const char*>(decrypted_data), out_len)){
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);

    out_file.close();
    in_file.close();
    return true;
}


#ifndef __PROGTEST__

bool compare_files ( const char * name1, const char * name2 )
{
    ifstream file1(name1, ios::binary);
    string contents1((istreambuf_iterator<char>(file1)), istreambuf_iterator<char>());
    ifstream file2(name2, ios::binary);
    string contents2((istreambuf_iterator<char>(file2)), istreambuf_iterator<char>());
    if (contents1 != contents2)
    {
        return false;
    }
    return true;
}

int main ( void )
{
	crypto_config config {nullptr, nullptr, nullptr, 0, 0};

	// ECB mode
	config.m_crypto_function = "AES-128-ECB";
	config.m_key = make_unique<uint8_t[]>(16);
 	memset(config.m_key.get(), 0, 16);
	config.m_key_len = 16;

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config));
    assert( compare_files ("out_file.TGA", "homer-simpson_enc_ecb.TGA"));

	assert( decrypt_data  ("homer-simpson_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_ecb.TGA") );

	assert( decrypt_data  ("UCM8_enc_ecb.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_1_enc_ecb.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_2_enc_ecb.TGA") );

	assert( decrypt_data ("image_3_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_3_dec_ecb.TGA") );

	assert( decrypt_data ("image_4_enc_ecb.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_4_dec_ecb.TGA") );

	// CBC mode
	config.m_crypto_function = "AES-128-CBC";
	config.m_IV = make_unique<uint8_t[]>(16);
	config.m_IV_len = 16;
	memset(config.m_IV.get(), 0, 16);

	assert( encrypt_data  ("UCM8.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8_enc_cbc.TGA") );

	assert( decrypt_data  ("UCM8_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "UCM8.TGA") );

	assert( encrypt_data  ("homer-simpson.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson_enc_cbc.TGA") );

	assert( decrypt_data  ("homer-simpson_enc_cbc.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "homer-simpson.TGA") );

	assert( encrypt_data  ("image_1.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_5_enc_cbc.TGA") );

	assert( encrypt_data  ("image_2.TGA", "out_file.TGA", config) &&
			compare_files ("out_file.TGA", "ref_6_enc_cbc.TGA") );

	assert( decrypt_data ("image_7_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_7_dec_cbc.TGA") );

	assert( decrypt_data ("image_8_enc_cbc.TGA", "out_file.TGA", config)  &&
		    compare_files("out_file.TGA", "ref_8_dec_cbc.TGA") );
	return 0;
}

#endif /* _PROGTEST_ */
