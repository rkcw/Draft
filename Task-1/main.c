//You do not have to change anything in this file. You will not be required to submit this file
#include "Task1.h"
#include <stdio.h>
#include <stdlib.h>

enum tasks{
    QUIT,
    PASSWORD,
    EMAIL,
    REGEX,
    BULK_CHECK,
    FUNC_COUNT,
};



int main(){
    while(1){
        printf("Task 1 Functions:\n");
        for(int i = 1; i < FUNC_COUNT; i++) printf("%d:\n",i);
        printf("0: Quit\n");
        int valid = 0;
        for(;valid==0;){
            printf("Please choose an option: ");
            int in=0;
            scanf("%d", &in);
            getchar();
            switch (in){
                case PASSWORD:
                    valid = 1;
                    char password[500];
                    printf("Please enter a password to check: ");
                    scanf("%[^\n]", password);
                    getchar();
                    int is_valid = password_check(password);
                    if(is_valid) {
                        printf("Password is valid.\n");
                    } else {
                        printf("Password is invalid.\n");
                    }
                    break;
                case EMAIL:
                    valid = 1;
                    char email[500];
                    printf("Please enter an email to check: ");
                    scanf("%[^\n]", email);
                    getchar();
                    email_check(email);
                    break;
                case REGEX:
                    valid = 1;
                    char rex[500];
                    char* list[500];
                    printf("Please enter a regex: ");
                    scanf("%[^\n]", regex);
                    getchar();
                    regex(list, rex);
                    break;
                case BULK_CHECK:
                    valid = 1;
                    break;
                case QUIT:
                    printf("Terminating program...");
                    return 0;
                default:
                    printf("Not a valid option, please choose again.\n");
                    break;
            }
        }
    }
}