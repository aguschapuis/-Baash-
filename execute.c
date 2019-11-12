
#include "builtin.h"
#include "execute.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


int exec_simple_command(pipeline pipeline){
    
      int pid;
      char **argv;
      scommand first;
      printf("exec\n");
      first = pipeline_front(pipeline);
      if(first == NULL){
         printf("scommand NULL\n"); 
      }
      argv = (char **)scommand_to_string(first);  
      pid = fork();
      if (pid == 0){
         if(execvp(argv[0], argv) < 0){
             printf("Could not execute the command\n");
             return -1;
         }
       } else {
           wait(NULL);
       }
       return 0;
}

int exec_pipe_command(pipeline pipeline){

       scommand first;
       scommand second;
       char **argv;
       int fd[2];
       int pid1, pid2;

       if(pipe(fd) < 0){
          printf("Could not pipe\n");
          return -1;
       }

       first = pipeline_front(pipeline);
       pipeline_pop_front(pipeline);
       second = pipeline_front(pipeline);
        
       pid1 = fork();        
       if (pid1 < 0){
         printf("Could not fork\n");
         return -1;
       }
       if (pid1 == 0){        //ejecuto el primer hijo
         close(fd[1]);
         dup2(fd[1],1);
         close(fd[1]);
         argv = (char **)scommand_to_string(first);
         if (execvp(argv[0], argv) < 0){
           printf("Could not execute the first command\n");
           return -1;
         }
       } else {
          pid2 = fork();   
          if(pid2 < 0){
             printf("Could not fork\n");
             return -1;
          } 
          if (pid2 == 0){
             close(fd[1]); 
			    dup2(fd[0], 0); 
			    close(fd[0]);
             argv = (char **)scommand_to_string(second);
             if(execvp(argv[0], argv) < 0){
                printf("Could not execute the second command\n");
                return -1;
             } 
          } else {
             wait(NULL);
             wait(NULL);
          }


       }
       return 0;
}

void extern_run (pipeline apipe) {

        int pipe_length;
        printf("extern\n");
        pipe_length = pipeline_length(apipe);
        printf("%d\n" , pipe_length);
        if (pipe_length == 1){
           exec_simple_command(apipe);
        } else if (pipe_length > 1){
           exec_pipe_command(apipe);
        } else {
           return;
        }
  }


void execute_pipeline(pipeline apipe) {

        
        if (builtin_index(apipe) < 0) {
               printf("run extern\n");
               extern_run(apipe);
         } else {
               builtin_run(apipe);
         }
}



