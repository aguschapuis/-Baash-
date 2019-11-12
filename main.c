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
  printf("\033[0;31m");
  printf("%s ~baash $ ",cwd);
  printf("\033[0m");

  //fgets (buff, sizeof(buff), stdin);
  //stdin = (fopen ("input.txt", "w+"));            
  //fprintf(stdin, "%s" , (const char *)buff);     
  pipe = parse_pipeline(input);
  if (pipe == NULL){
       printf("Hubo un error de sintaxis\n");
       continue;
  }
  //   printf("Entro a execute\n");
    execute_pipeline(pipe);
  //   printf("Salio de execute\n");     
    //fclose(input);
    //remove("input.txt");            
    //parser_destroy(input);
  }
  return 0;     
}


