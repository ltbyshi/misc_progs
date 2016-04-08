#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <errno.h>
#include <string.h>

unsigned char key[32];
const char* message = "Crack me and crack me again!";
const int bits = 256;
const int key_length = 32;

void RealRandom(unsigned char* buf, size_t length)
{
    FILE* fp;
    fp = fopen("/dev/random", "rb");
    if(!fp)
    {
        perror("Error: cannot open /dev/random");
        exit(1);
    }
    fread(buf, length, 1, fp);
    fclose(fp);
}

void HexPrint(const unsigned char* s, size_t length)
{
    size_t i;
    for(i = 0; i < length; i ++)
        printf("%X", (unsigned int)s[i]);
};

int main()
{
    AES_KEY aes_key;
    unsigned char* encrypted;
    unsigned char* decrypted;
    size_t message_len;

    message_len = strlen(message);
    printf("Original message(%lu): %s\n", message_len, message);
    encrypted = (unsigned char*)malloc(message_len*2 + 1);
    decrypted = (unsigned char*)malloc(message_len + 1);
    RealRandom(key, key_length);
    printf("Encrypt key: ");
    HexPrint(key, key_length);
    printf("\n");

    AES_set_encrypt_key(key, bits, &aes_key);
    AES_encrypt((unsigned char*)message, encrypted, &aes_key);
    printf("Encrypted message(%lu): ", strlen((char*)encrypted));
    HexPrint(encrypted, strlen((char*)encrypted));
    printf("\n");
    
    AES_set_decrypt_key(key, bits, &aes_key);
    AES_decrypt(encrypted, decrypted, &aes_key);
    printf("Decrypted message(%lu): %s\n", strlen((char*)decrypted), (char*)decrypted);

    free(encrypted);
    free(decrypted);
    return 0;
}


