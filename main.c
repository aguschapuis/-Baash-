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
  char cwd[256];
  pipeline pipe;
  Parser input;
  input = parser_new(stdin);
  
   while(1) {
    getcwd(cwd,sizeof(cwd));
    printf("\033[1;32m");
    printf("%s ~baash $ ",cwd);
    printf("\033[0m");   
    pipe = parse_pipeline(input);
    if (pipe == NULL){
         continue;
    }
    execute_pipeline(pipe);
  }
  return 0;     
}


