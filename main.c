#include "command.h"
#include "execute.h"
#include "builtin.h"
#include "parser.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main (void){

       FILE *input;
       char buff[1024];      
       pipeline pipeline;
       Parser parser;

       while(1) {
            printf("~/Home/baash $\t");
            fgets (buff, sizeof(buff), stdin);
            input = (fopen ("input.txt", "w+"));            
            fprintf(input, "%s" , (const char *)buff);
            parser = parser_new(input);
            pipeline = parse_pipeline(parser);
            execute_pipeline(pipeline);
            fclose(input);
            remove("input.txt");            
            parser_destroy(parser);
       }
       return 0;     
}


