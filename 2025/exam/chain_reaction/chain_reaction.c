#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int visited_first, visited_second;

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


void last_chance() {

    unsigned char shellcode[64];
    if (visited_first && visited_second) {
        printf("[+]Now give me something juicy: ");
        read(0, shellcode, sizeof(shellcode));
        void (*function)();
        function = (void (*)())shellcode;
        (void)(*function)();

    }
}

void chain_1(int param_1) {

    if (param_1 == 0xdeadbeef) {
        printf("Hello! Bye\n");
        visited_first = 1;
    }

}

void chain_2(int param_1, int param_2) {

    if (param_1 == 0xcafebabe && param_2 == 0xdeadc0de) {
        printf("Hello! Bye\n");
        visited_second = 1;
    }
}


void start_chain_reaction() {

    char buffer[64];    
    printf("[+] You have only one chance to start chain reaction: ");
    scanf("%s", buffer);

}

int main(int argc, char const *argv[]) {

    setup();
    start_chain_reaction();
    return 0;
}