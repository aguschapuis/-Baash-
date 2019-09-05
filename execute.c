#include "execute.h"
#include "builtin.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_ARGS 256

void execute_pipeline(pipeline apipe) {
   
    if (builtin_index(apipe) != BUILTIN_UNKNOWN){
      
        builtin_run((const pipeline)apipe);
   
    } else {

            const char *stdin = (const char *)scommand_get_redir_in(pipeline_front(apipe));
            const char *stdout = (const char *)scommand_get_redir_out(pipeline_front(apipe));
            int pid;
            const char *execute; 
            scommand aux;
            char * command;
            
            if (pipeline_length(apipe) == 1){      // el caso de que sea un solo comando, no necesito un pipe
 
                
                 char const* argv;
                 aux = pipeline_front(apipe);
                 command = (char *) scommand_front(aux);
                 scommand_pop_front(aux);
                 execute = (const char *) scommand_to_string(aux);  
                 argv = execute;               
                 pid = fork();
                 if (pid == 0){             // soy el hijo
                      
                       if (stdin != NULL) {           
                      
                              const char * in = (const char *) stdin;
                              int fd;
                              fd = open(in, O_RDWR ,S_IRWXU);      // si hay un stdin, utlizo open para abrir el archivo
                              if (fd == -1) {
                               
                                      printf("No se pudo abrir el archivo\n");
                                      _exit(1);                         
                               }                   
                             
                              if(dup2(0,fd) == -1){

                                      _exit(1);
                               }  
                         }                    
                         if (stdout != NULL){     
                               
                              int fd;
                              fd = open(stdout, O_RDWR ,S_IRWXU);
                               
                               if(dup2(1, fd) == -1){
 
                                      _exit(1);
                               }
                         } 
                         
                         if (execl(command, argv) == -1){
                     
                               _exit(1);
                         }
                         _exit(0);                  // termina el proceso hijo
                 
                 } else {
                    
                       if (pipeline_get_wait(apipe) == true){   // veo si el proceso padre tiene que esperar al hijo
                       wait(NULL);
                 
                     } 
                 } 

               } else {          // si tiene mas comandos, hay que utilizar al menos un pipe
                                      
              
                 for (unsigned int i = 0; i < pipeline_length(apipe); i++){

                          int pipes[2];
                          char* argv[MAX_ARGS];
                          pipeline pop;
                          pop = apipe;
                          aux = pipeline_front(apipe);
                          pipeline_pop_front(pop);
                          command = (char *)scommand_front(aux);
                          scommand_pop_front(aux);                   
                          execute = (const char *) scommand_to_string(aux);                        
                          pipe(pipes);
                          pid = fork();
                          if (pid == 0) { 

                              if (i == 0) {         // el primer command

                                        if (stdin != NULL) {

                                                  int fd;         
                                                  const char *in = (const char*)stdin;
                                                  fd = open(in, O_RDWR ,S_IRWXU);      // si hay un stdin, utlizo open para abrir el archivo
                                                 
                                                  if (fd == -1) {
                               
                                                           printf("No se pudo abrir el archivo\n");
                                                           _exit(1);
                         
                                                  }                         
                                                
                                                  if (dup2(0, fd) == -1){

                                                        _exit(1);      //cambio el standard in del proceso por el descriptor de fd
                                                  }                                                                        
                                             }

                                        if (close(pipes[0]) == -1) {
                                             
                                                  _exit(1); 
                                        }
                                             
                                        if (dup2(1,4) == -1){
                                                        
                                                  _exit(1);
                                        }

                                        if(execvp(command, argv) == -1){

                                                     _exit(1);
                                         } 

                              } else if (i == pipeline_length(apipe)-1){  // para el ultimo command
                                                                           
                                        if (stdout != NULL) {

                                                 int fd;
                                                 FILE *fichero;
                                                 fichero = fopen (stdout,"w");
                                                 fd = open(stdout, O_RDWR ,S_IRWXU); 
                                                 if (fd == -1){

                                                      _exit(1);
                                                  }
                                             
                                                  if (dup2 (1, fd)==-1){   // la salida va a stdout
                                               
                                                      _exit(1);
                                              
                                                  }      
                                             
                                                 if (execvp(command, argv) == -1){   //ejecuto el comando
                                              
                                                     _exit(1);
                                             
                                                  }

                                                 if (fclose(fichero) == -1){       //cierro el archivo stdout
                                                
                                                    printf ("No se pudo cerrar el archivo\n");
                                                    _exit(1);
 
                                                  }                                            

                                        } else {
                                       
                                                 if (dup2 (1,4) == -1){      // escribo de pipes[1] 
                                                  
                                                     _exit(1);
                                              
                                                  }                                        
                                                 
                                                  if (execvp(command, argv) == -1){
                                              
                                                     _exit(1);
                                             
                                                   } 
                                        }
                                         
                                                                                                                                                                                                                                          

                               }  else {   // para el resto de los comandos

                                        if (dup2(0,3) == -1){   // la entrada es la lectura del resultado del comando anterior
                                             _exit(1);
                                        }                    
                                        if(dup2(1,4) == -1){    // la salida va al siguiente comando
                                             _exit(1);
                                        }   
                                        if (execvp(command,argv) == -1) {    
                                             _exit(1);
                                        }                                                                               
                          }  

                          _exit(0); // el proceso hijo finaliza exitosament y se cierra                                   
                 
                     } else {

                                    if (dup2(0, 3) == -1){     // el padre lee el resultado del proceso hijo a traves 
                                                               // del pipe                                  
                                         _exit(1);
                                    }                                 
                                    if (pipeline_get_wait(apipe) == true){
                                
                                           wait(NULL);
                                    }
                                    
                     }
                                                 
                }    
                 
     }

    
    }
}
