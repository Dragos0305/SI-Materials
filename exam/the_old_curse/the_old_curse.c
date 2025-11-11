#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FLAG_SIZE 69

void print_ascii_art() {
    const char* ascii_art =
"                             /\\\n"
"                            /  \\\n"
"                           |    |\n"
"                         --:'''':--\n"
"                           :'_' :\n"
"                           _:\"\":\\___\n"
"            ' '      ____.' :::     '._\n"
"           . *=====<<=)           \\    :\n"
"            .  '      '-'-'\\_      /'._.'\n"
"                             \\====:_ \"\"\n"
"                            .'     \\\\\n"
"                           :       :\n"
"                          /   :    \\\n"
"                         :   .      '.\n"
"         ,. _        snd :  : :      :\n"
"      '-'    ).          :__:-:__.;--'\n"
"    (        '  )        '-'   '-'\n"
" ( -   .00.   - _\n"
"(    .'  _ )     )\n"
"'-  ()_.\\,\\,   -\n";

printf("%s", ascii_art);

}

void help() {
    printf("Hello! Good luck at your exam\n");
}

void curse() {
    char flag[FLAG_SIZE+1];
    memset(flag, 0, FLAG_SIZE+1);

    FILE * fp = fopen("flag.txt", "r");

    if(fp == NULL) {
        printf("Nice! Now try your exploit on the remote instance\n");
    }
    fread(flag, sizeof(char), FLAG_SIZE, fp);
    
    printf("\nYou break the curse\n");
    puts(flag);
}

void welcome_message() {
    printf("[+] Hello, stranger! It's time to demonstrate again what you learnt\n");
}

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void start_the_curse()  {

    char name[32];
    char spell[64];
    
    printf("[+] Your teacher wants to help you, so he gave me this %p\n", help);
    printf("[+] First of all, what is your name, junior?\n");

    scanf("%s", name);
    printf("[+] Oh, welcome ");
    printf(name);

    printf("\n[+] Now give me your most powerful spell to breaj the curse\n");
    scanf("%s", spell);

}

int main(int argc, char const *argv[])
{
    setup();
    print_ascii_art();
    welcome_message();
    start_the_curse();
    return 0;
}




