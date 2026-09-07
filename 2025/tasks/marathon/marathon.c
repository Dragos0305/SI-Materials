#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define PASS_LEN 18
int km = 0;
char password[PASS_LEN];

static const char *CHARSET = "abcdefghijklmnopqrstuvwxyz"
                             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                             "0123456789"
                             "!@#$%^&*()-_=+[]{};:,.<>?/";

static int generate_password(size_t n) {
    unsigned char* buf = malloc(n);
    FILE *f = fopen("/dev/urandom", "rb");
    if (f) {
        size_t r = fread(buf, 1, n, f);
        fclose(f);
        
    }
    for (size_t i = 0; i < n; i++) {
        password[i] = CHARSET[(int)buf[i] % strlen(CHARSET)];
    }

}
void milestone_prize() {
    printf("Km %d: %c\n", km, password[km]);
    km++;
}


void marathon() {

    char buffer[32];
    printf("Some words before start?\n");
    scanf("%s", buffer);
    printf("Good luck!!\n");
}

void setup() {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    
}

void menu() 
{

    int choice;
    printf("Do you want to start the marathon or use the cheat code and receive your prize?\n");
    printf("Enter 1 for marathon or 2 for cheat code: ");
    scanf("%d", &choice);

    if(choice == 1)
        marathon();
    else if (choice == 2) {
        char user_password[PASS_LEN];
        char flag[69];
        memset(flag, 0, 69);

        printf("Enter password: ");
        scanf("%s", user_password);
        
        if (!strcmp(user_password, password)) {
            FILE* fp = fopen("flag.txt", "r");

            fread(flag, sizeof(char), 68, fp);
            printf("%s\n", flag);
        }
    }

}

int main(int argc, char const *argv[])
{
    setup();
    generate_password(PASS_LEN);
    menu();
    
    return 0;
}
