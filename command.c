#include "command.h"
#include <stdbool.h>
#include <stdlib.h>
#include "assert.h"
#include <stdio.h>

scommand scommand_new(void){
       scommand new = calloc(1, sizeof(scommand));
       new->list = NULL;
       new->in = NULL;
       new->out = NULL;
       return new;
}


scommand scommand_destroy(scommand self){
       assert(self != NULL);
       g_slist_free(self->list);
       self->in = NULL;
       self->out = NULL;
       self = NULL;
       return self;
}

void scommand_push_back(scommand self, bstring argument){
       assert(self!=NULL && argument!=NULL);
       self->list = g_slist_append(self->list, argument);     
}


void scommand_pop_front(scommand self){

       assert(self!=NULL && !scommand_is_empty(self));
       self->list = g_slist_remove(self->list,self->list->data); //elimina el elemento que contenga el dato self->list->data (primero)
}


     
void scommand_set_redir_in(scommand self, bstring filename) {
       assert(self!=NULL);
       self->in = filename;
}


void scommand_set_redir_out(scommand self, bstring filename){
       assert(self!=NULL);
       self->out = filename;
}

bool scommand_is_empty(const scommand self){
       assert(self!=NULL);
       return (self->list == NULL);
}

unsigned int scommand_length(const scommand self){
       
       assert(self!=NULL);                  
       return g_slist_length(self->list);
}


const_bstring scommand_front(const scommand self){

       assert(self!=NULL && !scommand_is_empty(self)); 
       return (const_bstring)self->list->data;
}

const_bstring scommand_get_redir_in(const scommand self) {
       const_bstring aux;
       assert(self!=NULL);
       aux = (const_bstring)self->in;
       return aux;
}

const_bstring scommand_get_redir_out(const scommand self){
       const_bstring aux;
       assert(self!=NULL);
       aux = (const_bstring)self->out;
       return aux;
}


bstring scommand_to_string(const scommand self){
       
       bstring ret;
       int j;
       assert(self != NULL);
       ret = NULL;
       while (!(self->list == NULL)){
                     j = bconcat(ret, (const_bstring)g_slist_nth(self->list, 0));
                     assert(j == BSTR_ERR);
              if (self->list->next != NULL){
                     j = bconcat(ret,(const_bstring)" ");
              }
             scommand_pop_front(self);

       }
       return ret;      
}



pipeline pipeline_new(void){

       pipeline new = calloc(sizeof(pipeline),1);
       new->list = NULL;
       new->wait = true;
       return new;
}

pipeline pipeline_destroy(pipeline self){
 
       assert(self!=NULL);
       g_slist_free(self->list);
       self = NULL;
       return self;
}


void pipeline_push_back(pipeline self, scommand sc){
      
      assert(self!=NULL && sc!=NULL);
      self->list = g_slist_append(self->list, sc);
}


void pipeline_pop_front(pipeline self){ 
      
      assert(self!=NULL && !pipeline_is_empty(self));
      self->list = g_slist_remove(self->list,self->list->data);
}


void pipeline_set_wait(pipeline self, const bool w){
      
       assert(self!=NULL);
       self->wait = w;      
}


bool pipeline_is_empty(const pipeline self){
      
       assert(self!=NULL);
       return (self->list == NULL);
}


unsigned int pipeline_length(const pipeline self){
      
       assert(self!=NULL); 
       return g_slist_length(self->list);
}


scommand pipeline_front(const pipeline self){
      
        assert(self!=NULL && !pipeline_is_empty(self));
        return self->list->data;
}


bool pipeline_get_wait(const pipeline self){
       
        assert(self!=NULL);
        return (self->wait == true);
}


bstring pipeline_to_string(const pipeline self){

        unsigned int length; 
        int j;
        bstring ret;
        assert(self != NULL);
        length = pipeline_length(self);
        ret = NULL;
        for (unsigned int i = 0; i < length; i++) {
              j = bconcat(ret, (const_bstring) scommand_to_string(self->list->data));
              if (j == BSTR_ERR){
                     printf("error\n");
                     return NULL;
              }
              //assert(j == BSTR_ERR);
              if (!(self->list->next == NULL)){
                     j = bconcat(ret, (const_bstring) " | ");
              }
              pipeline_pop_front(self);
        }
        pipeline_destroy(self);
        return ret;
}                     