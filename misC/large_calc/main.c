#include <stdio.h>
#include <stdlib.h>
#define RUN
#include "str_number.h"
#ifdef DEFS
//test definitions
#endif

int main(int argc, char** argv)
{

#ifdef TEST
    for (int i = 0; i <= 120; i++)
        printf("%c = %d, ",i, base_check(i,'f'));
    putchar('\n');
#endif
#ifdef RUN
    int q_num = 0;
    const int base = 16, size = 100;
    //char num[2][100];
    STR_INT** num_list;
    //char** num;
    if (q_num + argc-1 < 2)
    {
        num_list = (STR_INT**) malloc((2-argc+1)*sizeof(STR_INT*));
    }
    while (q_num + argc-1 < 2)
    {
        printf("please provide a number: ");
        *(num_list+q_num) = new_str_int(base,size,stdin);
        //*(num_list+q_num) = (char*) malloc(100*sizeof(char));
        //read(*(num+q_num),10,stdin);
        printf("number read: ");
        print_str_int(*(num_list+q_num),stdout);
        q_num++;
    }

    char* a;
    char* b;
    printf("Argument read check:\n");
    if (argc == 1){
        printf("no args provided on call, two prompted:\n");
        a = (*num_list)->value;
        b = (*(num_list+1))->value;
    }
    else if (argc == 2){
        printf("one arg provided on call, second number prompted:\n");
        a = *(argv+1);
        mark(a,base);
        b = (*num_list)->value;
    }
    else {
        printf("Both args provided on call:\n");
        a = *(argv+1);
        mark(a,base);
        b = *(argv+2);
        mark(b,base);
    }
    print_num(a,stdout);
    print_num(b,stdout);
    
    for (STR_INT** n = num_list; n < num_list+q_num; n++){
        free((void*) *n);
        //printf("freed a string\n");
    }
    free((void*) num_list);
    //printf("freed num\n");
#endif
}
