#include <stdio.h> 
#include <signal.h> 
#include <stdlib.h>
#include <string.h>

#define FLAG_SIZE 68

void win() {

    char flag[FLAG_SIZE + 1];
    memset(flag, 0, FLAG_SIZE + 1);
    FILE* fp = fopen("flag.txt", "r");
    
    if (fp == NULL) {
        printf("[+] Nice! Now try exploit on the remote instance\n");
    }

    fread(flag, sizeof(char), FLAG_SIZE, fp);
    printf("%s\n", flag);

}

void make_a_wish() {

    char buffer[128];
    printf("[+] Now make a wish: \n");
    scanf("%s", buffer);
}

void sig_handler(int sig) {

    printf("[+] Welcome in forbbiden realm, warrior\n");
    printf("[+] This is your gift: %p\n", win);
    make_a_wish();
}

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    signal(SIGSEGV, sig_handler); 
}

void start_challenge() {

    char buffer[32];
    printf("Say magic words: ");
    scanf("%s", buffer);
}

int main(int argc, char const *argv[])
{
    setup();
    start_challenge();
    
    return 0;
}
