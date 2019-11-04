#include "command.h"
#include <stdbool.h>
#include <stdlib.h>
#include "assert.h"
#include <stdio.h>

scommand scommand_new(void){
      scommand new = calloc(sizeof(scommand),1);
      new->list = g_slist_alloc();
      new->in = NULL;
      new->out = NULL;
      return new;
}


scommand scommand_destroy(scommand self){
       assert(self != NULL);
      g_slist_free(self->list);
      self->in = NULL;
      self->out = NULL;
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
       if ((g_slist_length(self->list)) == 0) {
              return true;
       };
       return false;
}

unsigned int scommand_length(const scommand self){
       unsigned int aux ;
       assert(self!=NULL);          
       aux = g_slist_length(self->list) - 1;        
       return aux;
}


const_bstring scommand_front(const scommand self){
       const_bstring aux ;
       assert(self!=NULL && !scommand_is_empty(self));
       aux = (const_bstring)g_slist_nth (self->list,0);
       return aux;
}

const_bstring scommand_get_redir_in(const scommand self) {
       const_bstring aux;
      assert(self!=NULL);
       aux = (const_bstring)self->in->data;
       return aux;
}

const_bstring scommand_get_redir_out(const scommand self){
       const_bstring aux;
     assert(self!=NULL);
       aux = (const_bstring)self->out->data;
       return aux;
}


bstring scommand_to_string(const scommand self){
       
       int j ;
       bstring ret ;
       if (self == NULL){
              return NULL;
       }      
       j = 0;
       ret = self->list->data;
       for (unsigned int i = 0; i < scommand_length(self); i++){
            j = bconcat(ret,(const_bstring) g_slist_nth (self->list, i));  
            assert(j == BSTR_ERR);
       };      
       scommand_destroy(self);
       return ret;      
}



pipeline pipeline_new(void){

       pipeline new = calloc(sizeof(pipeline),1);
       new->list = g_slist_alloc();
       new->wait = true;
       return new;
}

pipeline pipeline_destroy(pipeline self){
 
       assert(self!=NULL);
       g_slist_free(self->list);
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
       bool aux = (g_slist_length(self->list) == 0) ;
       return aux;
}


unsigned int pipeline_length(const pipeline self){
      
       unsigned int length;
       assert(self!=NULL);
       length = g_slist_length(self->list) - 1;
       return length;
}


scommand pipeline_front(const pipeline self){
      
        scommand first;
        assert(self!=NULL && !pipeline_is_empty(self));
        first = (scommand)g_slist_nth(self->list,0);
        return first;
}


bool pipeline_get_wait(const pipeline self){
       
        assert(self!=NULL);
        bool wait = (self->wait == true);
        return wait;
}


bstring pipeline_to_string(const pipeline self){

        int j;
        bstring ret;
        if (self == NULL){
            return NULL;
        }
        ret = scommand_to_string(self->list->data);
        for (unsigned int i = 1; i < g_slist_length (self->list); i++) {
              j = bconcat(ret, (const_bstring) scommand_to_string((scommand)g_slist_nth (self->list, i)));
              assert(j == BSTR_ERR);
        }
        pipeline_destroy(self);
        return ret;
}                     











         
























