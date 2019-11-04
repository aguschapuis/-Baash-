
#include "builtin.h"
#include "execute.h"
#include<sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int exec_simple_command(pipeline pipeline){
    

     int pid;
     char **execute;
     scommand first;
     printf("exec\n");
     first = pipeline_front(pipeline);
     execute = (char **)scommand_to_string(first);      
     printf("%s \n", execute[0]);       
     pid = fork();
     if (pid == 0){
        if(execvp(execute[0], execute) < 0){
            printf("Could not execute the command\n");
            return -1;
        }
      } else {
          wait(NULL);
        }
      return 0;
}
/*
int exec_pipe_command(char **argv, pipeline pipeline){

      scommand second;
       int fd[2];
       int pid1, pid2;

       if(pipe(fd) < 0){
          printf("Could not pipe\n");
          return -1;
       }
        
       pid1 = fork();
        
       if (pid1 < 0){
         printf("Could not fork\n");
         return -1;
        }

       if (pid1 == 0){
         

        }



}
*/
void extern_run (pipeline pipeline) {

        int length = pipeline_length(pipeline);
        printf("%d\n" , length);
        if (pipeline_length(pipeline) == 1){
           exec_simple_command(pipeline);
        } else if (pipeline_length(pipeline) > 1){
     //      exec_pipe_command(pipeline);
        } else {
           return;
        }
  }





void execute_pipeline(pipeline pipeline) {

        int aux = 1;
        if (aux) {
               extern_run(pipeline);
         } else {
               builtin_run(pipeline);
         }
}



