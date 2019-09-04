#include "builtin.h"
#include <unistd.h>


/*Esta primer funcion determina cual de los 3 valores de cmd_id le corresponde al pipe*/

cmd_id builtin_index (const pipeline pipe){
    cmd_id index;
    if (bstricmp(scommand_front(pipeline_front(pipe)),bfromcstr("cd"))){
        index = BUILTIN_CHDIR;
    }
    else if (bstricmp(scommand_front(pipeline_front(pipe)), bfromcstr("exit"))){
        index = BUILTIN_EXIT;
    }
    else {
        index = BUILTIN_UNKNOWN;
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
