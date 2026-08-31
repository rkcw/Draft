//You do not have to change anything in this file. You will not be required to submit this file.
//Compile (from the Task-1 folder) with your Task1.c:
//   gcc main.c Task1.c -o main
//(To run the reference solution instead, swap Task1.c for Task1sol.c.)
#include "Task1.h"
#include <stdio.h>
#include <stdlib.h>

enum tasks{
    QUIT,
    PASSWORD,
    EMAIL,
    REGEX,
    FUNC_COUNT,
};



//Removes a trailing '\r' (left over when the input file uses CRLF line endings),
//so scanf("%[^\n]", ...) does not accidentally store it inside the string.
void trim_cr(char* s){
    int len=0;
    while(s[len]!='\0') len++;
    if(len>0 && s[len-1]=='\r') s[len-1]='\0';
}

int main(){
    while(1){
        printf("Task 1 Functions:\n");
        printf("1: Password Check\n");
        printf("2: Email Check\n");
        printf("3: Regex\n");
        printf("0: Quit\n");
        int valid=0;
        for(;valid==0;){
            printf("Please choose an option: ");
            int in=0;
            scanf("%d", &in);
            getchar();
            switch (in){
                case PASSWORD:
                    valid=1;
                    char password[500];
                    printf("Please enter a password to check: ");
                    scanf("%[^\n]", password);
                    getchar();
                    trim_cr(password);
                    int is_valid=password_check(password);
                    if(is_valid) {
                        printf("Password is valid.\n");
                    } else {
                        printf("Password is invalid.\n");
                    }
                    break;
                case EMAIL:
                    valid=1;
                    char email[500];
                    printf("Please enter an email to check: ");
                    scanf("%[^\n]", email);
                    getchar();
                    trim_cr(email);
                    email_check(email);
                    break;
                case REGEX: {
                    valid=1;
                    char words[500][64];
                    char* list[501];
                    int count=0;
                    printf("Please enter the number of words: ");
                    scanf("%d", &count);
                    getchar();
                    printf("Please enter the words (one per line):\n");
                    for(int i=0;i<count;i++){
                        scanf("%63s", words[i]);
                    }
                    getchar();
                    for(int i=0;i<count;i++){
                        list[i]=words[i];
                    }
                    list[count]=NULL;
                    char rex[500];
                    printf("Please enter a regex: ");
                    scanf("%[^\n]", rex);
                    getchar();
                    trim_cr(rex);
                    regex(list, rex);
                    break;
                }
                case QUIT:
                    printf("Terminating program...\n");
                    return 0;
                default:
                    printf("Not a valid option, please choose again.\n");
                    break;
            }
        }
    }
}