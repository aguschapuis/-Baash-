
#include "builtin.h"
#include "execute.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "tests/syscall_mock.h"

int exec_simple_command(pipeline pipeline){
   
   int pid;
   scommand first;
   char **argv;
   FILE *infile;
   FILE *outfile;

   first = pipeline_front(pipeline);
   
   if(first == NULL){
      printf("scommand NULL\n");
      return -1; 
   }
   argv = calloc(scommand_length(first)+1, sizeof(char *));      
   if (scommand_length(first) == 1) {
      argv[0] = (char *)scommand_front(first)->data;
      scommand_pop_front(first);
   } else {
      for(unsigned int i = 0; i <= scommand_length(first)+1 ; i++){
          argv[i] = (char *)scommand_front(first)->data;
          scommand_pop_front(first);
      }
   }
   pid = fork();
   if (pid == 0){
      if(first->in != NULL){
         infile = fopen((const char *)first->in->data, "r");
         dup2(infile->_fileno, 0);
         fclose(infile);
      }
      if (first->out != NULL){
         outfile = fopen((const char *)first->out->data, "w");
         dup2(outfile->_fileno , 1);
         fclose(outfile);
      }
      if(execvp(argv[0], argv) < 0){
         printf("%s : Incorrect command\n",argv[0]);
         return -1;
      }
   } else {
           if(pipeline_get_wait(pipeline)){
             wait(NULL);
           }
   }  
   return 0;
}

int exec_pipe_command(pipeline apipe){

       scommand first;
       scommand second;
       char **argv;
       FILE *infile;
       FILE *outfile;
       
       int fd[2];
       int pid1, pid2;

       if(pipe(fd) < 0){
          printf("Could not pipe\n");
          return -1;
       }

       first = pipeline_front(apipe);
       pipeline_pop_front(apipe);
       second = pipeline_front(apipe);
        
       pid1 = fork();        
       if (pid1 < 0){
         printf("Could not fork\n");
         return -1;
       }
       if (pid1 == 0){        //ejecuto el primer hijo
         argv = calloc(sizeof(char *), scommand_length(first));
         if (scommand_length(first) == 1) {
           argv[0] = (char *)scommand_front(first)->data;
           scommand_pop_front(first);
         } else {
           for(unsigned int i = 0; i <= scommand_length(first); i++){
              argv[i] = (char *)scommand_front(first)->data;
              scommand_pop_front(first);
            }  
         }
         if(first->in != NULL) {
            infile = fopen((const char *)first->in->data, "r");
            dup2(infile->_fileno, STDIN_FILENO);
            fclose(infile);
         }
         dup2(fd[1],STDOUT_FILENO);
         close(fd[1]);
         close(fd[0]);
         if (execvp(argv[0], argv) < 0){
           printf("Could not execute the command\n");
           return -1;
         }
         exit(0);
       } else {
          pid2 = fork();   
          if(pid2 < 0){
             printf("Could not fork\n");
             return -1;
          } 
          if (pid2 == 0){
             argv = calloc(sizeof(char *), scommand_length(second));
             if (scommand_length(second) == 1) {
               argv[0] = (char *)scommand_front(second)->data;
               scommand_pop_front(first);
             } else {
               for(unsigned int i = 0; i <= scommand_length(second); i++){
                 argv[i] = (char *)scommand_front(second)->data;
                 scommand_pop_front(second);
               }
             }
             if(second->out != NULL){
                outfile = fopen((const char *)first->out->data, "w");
                dup2(outfile->_fileno , STDOUT_FILENO);
                fclose(outfile);
             }   
			    dup2(fd[0], STDIN_FILENO); 
             close(fd[0]);		
             close(fd[1]); 
             if(execvp(argv[0], argv) < 0){
                printf("Could not execute the second command\n");
                return -1;
             } 
          } else {
             close(fd[0]);
             close(fd[1]);
             if(pipeline_get_wait(apipe))
                wait(NULL);
             wait(NULL);      
         }
        
       }
       return 0;
}

void extern_run (pipeline apipe) {

        int pipe_length;
        pipe_length = pipeline_length(apipe);
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
               extern_run(apipe);
         } else {
               builtin_run(apipe);
         }
}



