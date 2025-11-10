#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void gadgets() {
    asm(
    /* Simple pop rdi; ret */
    ".global pop_rdi_ret\n"
    "pop_rdi_ret:\n"
    "    pop %rdi\n"
    "    ret\n"

    /* pop rsi; pop r15; ret  (two-pop gadget) */
    ".global pop_rsi_pop_r15_ret\n"
    "pop_rsi_pop_r15_ret:\n"
    "    pop %rsi\n"
    "    pop %r15\n"
    "    ret\n"
    );
}

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
}

void win(int arg_1) {
    printf("Calling function with 1 parameter: %d", arg_1);
}

void win2( int arg_1,  int arg_2) {


    printf("Calling function with 2 parameters: %d %d\n", arg_1, arg_2);
    if(arg_1 == 0x31 && arg_2 == 0xC0DE )
        printf("[+]Congrats!\n");
}
void start() {

    char buffer[32];
    printf("[+]Call win function, if you can: ");
    scanf("%s", buffer);
}

int main(int argc, char const *argv[])
{
    gadgets();
    setup();
    start();
    return 0;
}
