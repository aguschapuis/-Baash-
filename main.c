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
       char buff[256];      
       pipeline pipeline;
       Parser parser;
      //  bstring preword;
      //  unsigned char *word;
       while(1) {
            printf("~/Home/baash $\t");
            scanf("%s", buff);
            input = (fopen ("input.txt", "w+"));            
            fprintf(input, "%s" , (const char *)buff);
            parser = parser_new(input);
            pipeline = parse_pipeline(parser);

            // preword = pipeline_to_string(pipeline);
            // word = preword->data;
            // printf("EL PIPELINE ES: %s\n",word);

            execute_pipeline(pipeline);
            fclose(input);
            if (remove("input.txt") == -1) {
                  printf("El archivo no se pudo borrar\n");               
            }
            parser_destroy(parser);
       }
       exit(0);     
}


