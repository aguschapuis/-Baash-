/* Ejecuta comandos simples y pipelines.
 * No toca ningún comando interno.
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "command.h"


void execute_pipeline(pipeline apipe);
/*
 * Ejecuta un pipeline, identificando comandos internos, forkeando, y
 *   redirigiendo la entrada y salida. puede modificar `apipe' en el proceso
 *   de ejecución.
 *   apipe: pipeline a ejecutar
 * Requires: apipe!=NULL
 */

void extern_run (pipeline pipeline);


int exec_simple_command(pipeline pipeline);

/* Ejecuta un comando simple */

int exec_pipe_command(pipeline pipeline);

/* Ejecuta un pipe */

#endif /* EXECUTE_H */
