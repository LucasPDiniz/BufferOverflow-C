#include <stdio.h>
#include <string.h>

void secret() {
    printf("Secret Function\n");
}

int main(int argc, char **argv){
    char test[12];
    gets(test);
    printf("You send: %s\n", test);
    return(0);
}