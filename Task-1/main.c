//You do not have to change anything in this file. You will not be required to submit this file
#include "Task1.h"
#include <stdio.h>
#include <stdlib.h>

#define quit 10

enum tasks{
    QUIT,
    FUNC_1,
    FUNC_2,
    FUNC_COUNT,
};



int main(){
    while(1){
        menu:
        printf("Task 1 Functions:\n");
        for(int i = 1; i < FUNC_COUNT; i++) printf("%d:\n",i);
        printf("0: Quit\n");
        printf("Please choose an option: ");
        int in=0;
        scanf("%d", &in);
        switch (in){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case QUIT:
                printf("Terminating program...");
                return 0;
            default:
                printf("Not a valid option, please choose again.\n");
                goto menu;
        }
    }
}