#include "execute.h"
#include <unistd.h>


void execute_pipeline(pipeline apipe) {
   
    assert(apipe == NULL);
    int pid;
    scommand aux;
    const char *command;
    aux = scommand_front(apipe->list);
    if (pipeline_length(apipe) == 1){
         pid = fork();
         if (pid == 0) {
            command = aux->list->data;
            scommand_pop_front(aux);
            execvp(command, aux);
        } else {
            wait();
        }     
    } else {
        while (apipe->list != NULL){
            
        }
    }
         
         
        
            