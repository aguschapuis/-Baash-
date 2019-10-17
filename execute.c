
#include "builtin.h"
#include "execute.h"
#include<sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void extern_run (pipeline pipeline) {

             const char * command;
             scommand aux;
             char * argv[256];
             int pid;
             char* aux2;
             aux = pipeline_front(pipeline);
             command = (const char *) scommand_front(aux);
             scommand_pop_front(aux);
             for (unsigned int i = 0; i < scommand_length(aux) - 1; i++) {
                     
                     aux2 = aux->list->data;
             printf("El error esta aca\n");
                     argv[i] = aux2;
                     aux2 = aux->list->next->data;
             }              
             pid = fork();
             if (pid == 0){
                     
                 if (execvp(command, argv) == -1){                 
                      exit(1);
                 }     
             } else {
                    
                 if (pipeline_get_wait(pipeline) == true){   // veo si el proceso padre tiene que esperar al hijo
                       wait(NULL);                
                     } 
             }
             exit(0);
}


void execute_pipeline(pipeline pipeline) {

        printf("Comienzo execute_pipeline\n");
      if (1) {
             printf("Is runing EXTERN--------------------------------------------\n");
               extern_run(pipeline);
               printf("FIN execute_pipeline\n");
       } else {
        printf("Is runing BUILTIN--------------------------------------------\n");
               builtin_run(pipeline);
       }
       exit(0);
}



