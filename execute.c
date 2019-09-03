#include "execute.h"
#include "builtin.h"
#include <unistd.h>


void execute_pipeline(pipeline apipe) {
   
    if (builtin_index(apipe) != BUILTIN_UNKNOWN){
      
        builtin_run(apipe);
   
    } else {

            scommand aux;
            pipeline apipe2;
            bstring execute;
            int stdin;
            int stdout;
            int pid;
            int pipes[2][n-1];

            echo | sort  | uniq

            if (pipeline_length(apipe) == 1){

                 first = pipline_front(apipe);
                 execute = (const bstring) first->list->data;
                 scommand_pop_front(aux);
                 pid = fork();
                 if (pid == 0){
                      // mirar los pipes a ver si tengo un lugar donde leer
                      // Si hay un proceso antes
                      //      -> leo del proceso anterior pipes[i-1][0]
                      // Si hay un proceso despues
                      //      -> escribo en pipes[i][1]

                       execvp(execute, aux);
                       exit(1);
                 
                 } else {
                    
                    if (pipeline_get_wait())
                       wait();
                 
                 } 
                 

            } else {

                 int a[2];
                 pipe(a);
                 pid = fork();
                 if (pid == 0){
                    // Soy el hijo
                    // ejecutar el comando
                 }
            }

    }
    
    
    
    
    
    
    
    
    assert(apipe == NULL);
    int pid;
    scommand aux;
    pipeline pipeaux;
    const char *command;
    aux = scommand_front(apipe->list->data);
    pipeaux = apipe;
    if (pipeline_length(apipe) == 1){
         pid = fork();
         if (pid == 0) {
            command = aux->list->data;
            scommand_pop_front(aux);
            execvp(command, aux);
        } else {
            wait();
        }     
    } else if (pipeline_length(apipe) == 2) {
       
             int a[2];
             pipe(a);
             pid = fork();
             if(apipe->wait == true){
                 if(pid == 0){
                     command = aux->list->data;
                     scommand_pop_front(aux);
                     execvp(command, aux);
                 } else {
                     pipeline_pop_front(pipeaux);
                     command = pipeaux->list->data;
                 }


        }
    }
         
         
        
            