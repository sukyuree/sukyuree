/**********************************************************************
 * Copyright (c) 2020
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "types.h"

#define MAX_NR_TOKENS 32    /* Maximum number of tokens in a command */
#define MAX_TOKEN_LEN 64    /* Maximum length of single token */
#define MAX_COMMAND    256        /* Maximum length of command string */

/***********************************************************************
 * parse_command
 *
 * DESCRIPTION
 *    Parse @command, put each command token into @tokens[], and
 *    set @nr_tokens with the number of tokens.
 *
 * A command token is defined as a string without any whitespace (i.e., *space*
 * and *tab* in this programming assignment). Suppose @command as follow;
 *
 *   @command = "  Hello world   Ajou   University!!  "
 *
 * Then, @nr_tokens = 4 and @tokens should be
 *
 *   tokens[0] = "Hello"
 *   tokens[1] = "world"
 *   tokens[2] = "Ajou"
 *   tokens[3] = "University!!"
 *
 * Another exmaple is;
 *   command = "ls  -al   /home/operating_system /hw0  "
 *
 * then, nr_tokens = 4, and tokens is
 *   tokens[0] = "ls"
 *   tokens[1] = "-al"
 *   tokens[2] = "/home/operating_system"
 *   tokens[3] = "/hw0"
 *
 *
 * @command can be expressed with double quotation mark(") to quote a string
 * that contains whitespaces. Suppose following @command;
 *
 *   @command = "  We will get over with the "COVID 19" virus   "
 *
 * Then, @nr_tokens = 8, and @tokens are
 *
 *   tokens[0] = "We"
 *   tokens[1] = "will"
 *   tokens[2] = "get"
 *   tokens[3] = "over"
 *   tokens[4] = "with"
 *   tokens[5] = "the"
 *   tokens[6] = "COVID 19"
 *   tokens[7] = "virus"
 *
 * Note that tokens[6] does not contain the quotation marks.
 * Also, one @command can have multiple quoted strings, but they will not be
 * nested
 * (i.e., quote another string in a quote)
 *
 *   This "is a possible" case for a command --> This, is a possible, case, for,
 *                                                 a, command
 *   "This " is "what I told you" --> This, is, what I told you
 *
 * RETURN VALUE
 *    Return 0 after filling in @nr_tokens and @tokens[] properly
 *
 */
static int howlong(char *command){
    int i=0;
    while(1){
        i++;
        if(command[i]=='\0') break;
    }
    return i;
}

static int parse_command(char *command, int* nr_tokens, char *tokens[])
{
    int i = howlong(command);
    int count;
    int t=0;
    char **temp=(char**)malloc(sizeof(char*)*MAX_NR_TOKENS);
    for(int j=0;j<MAX_NR_TOKENS;j++){
        temp[j] = (char*)malloc(sizeof(char)*MAX_TOKEN_LEN);
    }
    for(count =0; count<i;count++){
        if(command[count]=='\"'){
            count++;
            while(command[count]!='\"'){
                temp[*nr_tokens][t]=command[count];
                t++;
                count++;
            }
            if(command[count]=='\"'){
                tokens[*nr_tokens]=temp[*nr_tokens];
                (*nr_tokens)++;
                t=0;
            }
        }
        else if(isspace(command[count])==0&&command[count]!='\"'){
            temp[*nr_tokens][t]=command[count];
            t++;
            if((isspace(command[count])==0&&isspace(command[count+1])!=0)||command[count+1]=='\"'){
                tokens[*nr_tokens]=temp[*nr_tokens];
                (*nr_tokens)++;
                t=0;
            }
        }
    }
    return 0;
}


/***********************************************************************
 * The main function of this program.
 * SHOULD NOT CHANGE THE CODE BELOW THIS LINE
 */
int main(int argc, char *argv[])
{
    char line[MAX_COMMAND] = { '\0' };
    FILE *input = stdin;

    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "No input file %s\n", argv[1]);
            return -EINVAL;
        }
    }

    while (fgets(line, sizeof(line), input)) {
        char *tokens[MAX_NR_TOKENS] = { NULL };
        int nr_tokens= 0;

        parse_command(line, &nr_tokens, tokens);

        fprintf(stderr, "nr_tokens = %d\n", nr_tokens);
        for (int i = 0; i < nr_tokens; i++) {
            fprintf(stderr, "tokens[%d] = %s\n", i, tokens[i]);
        }
        printf("\n");
    }

    if (input != stdin) fclose(input);
    return 0;
}
