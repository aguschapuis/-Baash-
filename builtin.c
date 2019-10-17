#include "builtin.h"
#include <unistd.h>
#include <stdio.h>


/*Esta primer funcion determina cual de los 3 valores de cmd_id le corresponde al pipe*/

cmd_id builtin_index (const pipeline pipe){
    cmd_id index;
    if (bstricmp(scommand_front(pipeline_front(pipe)),bfromcstr("cd"))){
        index = BUILTIN_CHDIR;
    printf("El index es chdir\n");    
    }
    // else if(bstrncmp(scommand_front(pipeline_front(pipe)),bfromcstr("cd"),2)){
        // printf("Incorrect comand \n Try with 'cd' ");
        // _exit(1);
    // }
    else if (bstricmp(scommand_front(pipeline_front(pipe)), bfromcstr("exit"))){
        index = BUILTIN_EXIT;
        printf("El index es exit\n");
    }
    // else if(bstrncmp(scommand_front(pipeline_front(pipe)),bfromcstr("exit"),4)){
        // printf("Incorrect comand \n Try with 'exit' ");
        // _exit(1);
    // }
    else {
        index = BUILTIN_UNKNOWN;
        printf("El index es desconocidp\n");
    }
    return index;

}

/*Esta determina cuando tiene que salir (_exit)*/

bool builtin_is_exit (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    return (index==BUILTIN_EXIT);

}


/*Corre el builtin , dependiendo cual sea su builtin_index */

void builtin_run (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    const_bstring frst_scomand;
    const char * dir;
    if(index==BUILTIN_CHDIR){
        pipeline_pop_front(pipe);
        frst_scomand = scommand_front(pipeline_front(pipe)); //primer scomand por eso el nombre 
        dir = (const char *)frst_scomand->data; //direccion a la que tiene que dirijirse chdir
        chdir(dir);
        pipeline_destroy(pipe);
    }
    else if (builtin_is_exit(pipe)){
        pipeline_destroy(pipe);
        _exit(0); //EXIT_SUCESS
    }
    else {
        _exit(1); //EXIT FAILURE
    }
    



}
