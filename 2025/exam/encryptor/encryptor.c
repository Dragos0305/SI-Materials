#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LEN 4
#define FLAG_SIZE 68

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void encrypt_flag(const char* key)
{
    char flag[FLAG_SIZE];
    memset(flag, 0, FLAG_SIZE);

    FILE * fp = fopen("flag.txt", "r");

    if(fp == NULL) {
        printf("Nice! Now try your exploit on the remote instance\n");
    }
    fread(flag, sizeof(char), FLAG_SIZE, fp);
    printf("Here is the encrypted flag\n");
    for (size_t i = 0; i < FLAG_SIZE; i++)
    {
        printf("%02x", flag[i] ^ key[i % KEY_LEN]);
    }
    printf("\nSee you in another life!");
    
}


int main(int argc, char const *argv[])
{

    setup();
    char key[KEY_LEN];
    char buffer[32];
    memset(key, 0, KEY_LEN);

    FILE *f = fopen("/dev/urandom", "rb");
    if (f == NULL) {
        printf("[-]Failed to open /dev/urandom\nExit...\n");
        exit(EXIT_FAILURE);
    }

    size_t r = fread(key, 1, KEY_LEN, f);
    fclose(f);

    printf("Hello, old friend! I already know that you get the flag because another colleague give it to you, so I decided to encrypt the flag for you.\n");
    printf("Any other words at the end?\n");
    scanf("%s", buffer);
    encrypt_flag(key);
    
    

    return 0;
}
