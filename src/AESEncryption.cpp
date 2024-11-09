#include "AESEncryption.h"
#include <openssl/evp.h>
#include <stdexcept>

std::string AESEncryption::encrypt(const std::string &data) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX.");
    }

    int len = 0;
    int ciphertext_len = 0;
    std::vector<unsigned char> ciphertext(data.size() + AES_BLOCK_SIZE);

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize AES encryption.");
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(data.c_str()), data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during AES encryption update.");
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during AES encryption finalization.");
    }
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return std::string(ciphertext.begin(), ciphertext.begin() + ciphertext_len);
}

std::string AESEncryption::decrypt(const std::string &data) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX.");
    }

    int len = 0;
    int plaintext_len = 0;
    std::vector<unsigned char> plaintext(data.size());

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize AES decryption.");
    }

    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, reinterpret_cast<const unsigned char*>(data.c_str()), data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during AES decryption update.");
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed during AES decryption finalization.");
    }
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.begin() + plaintext_len);
}
