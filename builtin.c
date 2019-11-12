#include "builtin.h"
#include <unistd.h>
#include <stdio.h>

#define CD_WRONG 2
#define EXIT_WRONG 3

/*Esta primer funcion determina cual de los 3 valores de cmd_id le corresponde al pipe*/

cmd_id builtin_index (const pipeline pipe){
    cmd_id index;
    if (bstricmp (scommand_front (pipeline_front (pipe)), bfromcstr ("cd")) == 0){
        index = BUILTIN_CHDIR;
        return index;
    }
    else if(bstrncmp (scommand_front (pipeline_front (pipe)), bfromcstr ("cd"), 2)==0){
        return CD_WRONG;
    }
    else if (bstricmp (scommand_front (pipeline_front (pipe)), bfromcstr ("exit")) == 0){
        index = BUILTIN_EXIT;
        return index;
    }
    else if(bstrncmp (scommand_front (pipeline_front (pipe)), bfromcstr ("exit"), 4) == 0){
        return EXIT_WRONG;
    }
    else {
        index = BUILTIN_UNKNOWN;
        return index;
    }

}

/*Esta determina cuando tiene que salir (_exit)*/

bool builtin_is_exit (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    return (index == BUILTIN_EXIT);

}


/*Corre el builtin , dependiendo cual sea su builtin_index */

void builtin_run (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    if(index != CD_WRONG && index != EXIT_WRONG){
        const_bstring frst_scomand;
        const char * dir;
        if(index==BUILTIN_CHDIR){
            scommand_pop_front(pipeline_front (pipe));
            if(scommand_is_empty(pipeline_front (pipe))){
                printf("---------- Must to give a direction after cd ----------\n");
            }
            else{
                frst_scomand = scommand_front(pipeline_front (pipe)); //primer scomand por eso el nombre 
                dir = (const char *)frst_scomand->data; //direccion a la que tiene que dirijirse chdir
                chdir(dir);
                pipeline_destroy(pipe);       
            }
        }
        else if (builtin_is_exit(pipe)){
            pipeline_destroy(pipe);
            _exit(1); //EXIT_SUCESS
        }
        else {
            _exit(1); //EXIT FAILURE
        }
    }
    else if(index == CD_WRONG){
        printf("---------- Incorrect comand. Try with 'cd' ----------\n");
    }
    else if(index == EXIT_WRONG){
        printf("---------- Incorrect comand. Try with 'exit' ----------\n");
    }
    else{
        printf("---------- Incorrect comand ----------\n");
    }
    
}