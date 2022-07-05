#include "dir_file.h"
#include "utils.h"

int open_file(char* fname, FILE** input) {
    if (access(fname, F_OK) == -1) {
        ERR_PRINT("The '%s' file does not exists\n", fname);
        return -1;
    }

    *input = fopen(fname, "r");
    if (input == NULL) {
        ERR_PRINT("Failed open '%s' file\n", fname);
        return -1;
   }

    return 1;
}

// This parse_str_to_list() split string to the tokens, and put the tokens in token_list.
// The value of return is the number of tokens.
int parse_str_to_list(const char* str, char** token_list) {
    /* Fill this function */
    int count = (int)strlen(str);
    int token=0;
    char *temp=(char*)malloc(sizeof(char)*(count));
    strncpy(temp,str,count-1);
    temp[count-1]='\0';
    char *ptr=strtok(temp,"/");
    while(ptr!=NULL){
	char *str_to_token = malloc(sizeof(char)*((int)strlen(ptr)));
        strcpy(str_to_token,ptr);
	token_list[token]=str_to_token;
        token++;
        ptr=strtok(NULL,"/");
    }
    return token;
}

void free_token_list(char** token_list, int num_token) {
    int index;

    for (index = 0; index < num_token; index++) {
        free(token_list[index]);
    }

    free(token_list);
}
