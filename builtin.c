#include "builtin.h"
#include <unistd.h>


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

bool builtin_is_exit (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    return (index==BUILTIN_EXIT);

}

void builtin_run (const pipeline pipe){
    cmd_id index = builtin_index(pipe);
    const_bstring dir;
    if(index==BUILTIN_CHDIR){
        pipeline_pop_front(pipe);
        dir = scommand_front(pipeline_front(pipe));
        chdir(dir);
    }
    else if (builtin_is_exit(pipe)){
        pipeline_destroy(pipe);
        _exit(0);
    }
    else {
        _exit(1);
    }
    



}
