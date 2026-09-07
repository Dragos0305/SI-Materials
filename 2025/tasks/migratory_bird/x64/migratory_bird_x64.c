#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FLAG_SIZE 69

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void south_africa() {
    
    char flag[FLAG_SIZE];
    memset(flag, 0, FLAG_SIZE);

    FILE * fp = fopen("flag.txt", "r");

    if(fp == NULL) {
        printf("Nice! Now try your exploit on the remote instance\n");
    }

    printf("Welcome in South Africa, bird!\n");
    fread(flag, sizeof(char), FLAG_SIZE - 1, fp);
    printf("%s\n", flag);

}

void start_challenge() {


    char name[7];
    char joke[32];

    printf("Tell me your name: ");
    scanf("%s", name);
    printf(name);

    printf("\nNow tell me a joke: ");
    scanf("%s", joke);


}

int main(int argc, char const *argv[])
{
    setup();
    start_challenge();
    return 0;
}
