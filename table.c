#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra.h"
#include "error.h"
#include "tabsymb.h"
#include <jansson.h>

extern columnData *valInsert;
column *valAddClm;
extern char *yytext;
extern int yylineno;
extern boolean follow_token;

char *tabcree;
char* tab_alter;
char *nameClm;
table *tete;
extern ClmInsert * begin;
extern values *start;
extern values *valClm;
extern char * keysCree;
extern int nbTab;



                 
//======================================================================//

boolean _TABLE(){
  boolean result;
    //table* tete0;
    if(token==TABLE){

      if(keysCree==NULL){
        creer_sm_erreur_declaration(keyspaceNotDeclared,yylineno,"Error");
      }
      else{
        //tete=find_list_table(keysCree);
        token=_lire_token();
        if(_NOT_EXISTS()){ 
          token=_lire_token(); 
        }
        if(token==IDF){

          if(in_Tab_tabl(tete,yytext)==true){
            creer_sm_erreur_declaration(tableAlreadyExist,yylineno,yytext);
          }
          else
          { 
            tete=add_table(tete,yytext);
            table* q;
            q=(table*)malloc(sizeof(table));
            q=tete;
            while(q != NULL){
          
              q=q->next;
            }
            free(q);
            free(tabcree);
            tabcree=(char*)malloc(strlen(yytext)+1);    //--??
            strcpy(tabcree,yytext);
          }

          token=_lire_token();
          if(token==POPEN){
            token=_lire_token();
            if(_COLUMN_DEFINITION()){
              token=_lire_token();
              if(_COLUMN_DEFINITION_AUX()){
                token=_lire_token();
                if(token==PVIRG){
                  //_afficher_table(tete,tabcree);
                  result=true;
                }
                else if(token==WITH){
                  token=_lire_token();
                  if(_TABLE_OPTIONS()){
                    result=true;
                  }
                  else result=false;
                }
                else result=false;
              }
              else result=false;
            }
            else result=false;
          }
          else result=false;
        }
        else result=false;
      }
    }
    else result=false;

  return result;
}

//======================================================================//


boolean _COLUMN_DEFINITION(){
  boolean result,aux;
  aux=false;
  if(token==IDF){
    if(tabcree != NULL){
      valAddClm=(column*)malloc(sizeof(column));
      valAddClm->name=(char *)malloc(strlen(yytext)+1);
      strcpy(valAddClm->name,yytext);
      nameClm=(char *)malloc(strlen(yytext)+1);
      strcpy(nameClm,yytext);
      if(in_list_clm(tete,tabcree,nameClm)==true){
          creer_sm_erreur_declaration(columnAlreadyExist,yylineno,valAddClm->name);
      }
      else aux=true;
    }
    token=_lire_token();
    if(_Type()){
      //valAddClm->typeClm=_chaine_to_type(yytext);
      
      //afficher_Type(valAddClm->typeClm);
      add_clm_table(tete,tabcree,valAddClm);
     // _afficher_table(tete,tabcree);
      //free(valAddClm);
      result=true;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _COLUMN_DEFINITION_AUX(){
  boolean result;
  if(token==VIRG){
  	free(nameClm);
    token=_lire_token();
    if(_PRIMARY_KEY()){
      token=_lire_token();
      if(token==POPEN){
        token=_lire_token();
        if(_PRIMARY_KEY_2()){
          token=_lire_token();
          result=_COLUMN_DEFINITION_AUX();
        }
        else result=false;
      }
      else result=false;
    }
    else if(_COLUMN_DEFINITION()){
      token=_lire_token();
      result=_COLUMN_DEFINITION_AUX();
    }
    else result=false;
  }
  else if(_PRIMARY_KEY()){
    if(tabcree != NULL){
      if(in_list_clm(tete,tabcree,nameClm)==true){
        column* p;
        p=(column*)malloc(sizeof(column));
        p=find_clm(tete,tabcree,nameClm);
        p->primary=true; 
      }
    }
    token=_lire_token();
    result=_COLUMN_DEFINITION_AUX();
  }
  else if(token==STATIC){
    if(tabcree != NULL){
      if(in_list_clm(tete,tabcree,nameClm)==true){
      column* p;
      p=(column*)malloc(sizeof(column));
      p=find_clm(tete,tabcree,nameClm);
      p->statiic=true; 
      }
    }
    token=_lire_token();
    result=_COLUMN_DEFINITION_AUX();
  }
  else if(token==PCLOSE){
      result=true;
  }
  else result=false;
  return result; 
}

//======================================================================//

boolean _PRIMARY_KEY_2(){
  boolean result;
  if(_PARTITION_KEY()){
    token=_lire_token();
    result=_CLUSTERING_COLUMNS();
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _PARTITION_KEY(){
  boolean result;
  if(token==IDF){
    result=true;  
  }
  else if(token==POPEN){
    token=_lire_token();
    result=_PARTITION_KEY_AUX();
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _PARTITION_KEY_AUX(){
  boolean result;
  if(token==IDF){
    token=_lire_token();
    result=_PARTITION_KEY_AUX_AUX();
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _PARTITION_KEY_AUX_AUX(){
  boolean result;
  if(token==VIRG){
    token=_lire_token();
    if(token==IDF){
      token=_lire_token();
      result=_PARTITION_KEY_AUX_AUX();
    }
    else result=false;
  }
  else if(token==PCLOSE){
    result=true;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _CLUSTERING_COLUMNS_AUX(){
  boolean result;
  if(token==VIRG){
    token=_lire_token();
    if(token==IDF){
      token=_lire_token();
      result=_CLUSTERING_COLUMNS();
    }
  }
  else if(token==PCLOSE){
    result=true;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _CLUSTERING_COLUMNS(){
  boolean result;
  if(token==VIRG){
    token=_lire_token();
    if(token==IDF){
      token=_lire_token();
      result=_CLUSTERING_COLUMNS();
    }
    else result=false;  
  }
  else if(token==PCLOSE){
    result=true;
  }
  return result;
}

//======================================================================//


//======================================================================//

boolean _COLUMN_DEFINITION_ALTER(){
  boolean result;
  if(token==IDF){
    if(tab_alter != NULL){
      nameClm=(char*)malloc(20*sizeof(char));
      valAddClm=(column*)malloc(sizeof(column));
      valAddClm->name=(char *)malloc(strlen(yytext)+1);
      strcpy(valAddClm->name,yytext);
      strcpy(nameClm,yytext);
      if(in_list_clm(tete,tab_alter,valAddClm->name)==true){
          creer_sm_erreur_declaration(columnAlreadyExist,yylineno,valAddClm->name);
      }
      else{
        table* q;
        q=(table*)malloc(sizeof(table));
        q=tete;
        while(q != NULL){
          //printf("%s\n",q->name);
          q=q->next;
        }
        add_clm_table(tete,tab_alter,valAddClm);
      }
      free(valAddClm);
    }
    token=_lire_token();
    if(_Type()){
      result=true;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _PRIMARY_KEY(){
  boolean result;
  if(token==PRIMARY){
    token=_lire_token();
    if (token==KEY)
    {
      result=true;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _TABLE_OPTIONS(){
  boolean result;
  if(token==COMPACT){
    token=_lire_token();
    if(token==STORAGE){
      token=_lire_token();
      if(token==PVIRG){
        result=true;
      }
      else if(token==AND){
        token=_lire_token();
        result=_TABLE_OPTIONS();
      }
      else result=false;
    }
    else result=false;
  }
  else if(token==CLUSTERING){
    token=_lire_token();
    if(token==ORDER){
      token=_lire_token();
      if(token==BY){
        token=_lire_token();
        if(token==POPEN){
          token=_lire_token();
          if(_CLUSTERING_ORDER()){
            token=_lire_token();
            if(token==AND){
              token=_lire_token();
              result=_TABLE_OPTIONS();
            }
            else if(token==PVIRG){
              result=true;
            }
            else result=false;
          }
          else result=false;
        }
        else result=false;
      }
      else result=false;
    }
    else result=false;
  }
  else if(_Options()){
    result=true;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _CLUSTERING_ORDER(){
  boolean result;
  if(token==IDF){
    token=_lire_token();
    if(token==ASC || token==DESC){
      token=_lire_token();
      result=_CLUSTERING_ORDER_AUX();
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _CLUSTERING_ORDER_AUX(){
  boolean result;
  if(token==VIRG){
    token=_lire_token();
    result=_CLUSTERING_ORDER();
  }
  else if(token==PCLOSE){
    result=true;
  }
  else result=false;
  return result;
}

//======================================================================//


boolean _ALTER_TABLE(){
  boolean result;
  if(token==TABLE){
    token=_lire_token();
    if(token==IDF){
      if(in_Tab_tabl(tete,yytext)==false){
        creer_sm_erreur_declaration(tableNotExist,yylineno,yytext);
      }
      else{
        free(tab_alter);
        tab_alter=(char*)malloc(strlen(yytext)+1);
        strcpy(tab_alter,yytext);
        token=_lire_token();
        result=_ALTER_TABLE_INSTRUCTIONS();
      }
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

/*alter_table_statement   ::=  ALTER TABLE table_name alter_table_instruction
alter_table_instruction ::=  ALTER column_name TYPE cql_type
                             | ADD column_name cql_type ( ',' column_name cql_type )*
                             | DROP column_name ( column_name )*
                             | WITH options
*/

boolean _ALTER_TABLE_INSTRUCTIONS(){
  boolean result;
  if(token==ALTER){
    token=_lire_token();
    if(token==IDF){
      valAddClm=(column*)malloc(sizeof(column));
      valAddClm->name=(char*)malloc(sizeof(strlen(yytext)+1));
      strcpy(valAddClm->name,yytext);
      token=_lire_token();
      if(token==TYPE){
        token=_lire_token();
        if(_Type()){
          //valAddClm->typeClm=_chaine_to_type(yytext);
          token=_lire_token();
          if(token==PVIRG){
          	if(in_list_clm(tete,tab_alter,valAddClm->name)==false){
          		creer_sm_erreur_declaration(columnNotExist,yylineno,valAddClm->name);
          	}
          	else{
          		tete=_delete_clm(tete,tab_alter,valAddClm->name);
	            add_clm_table(tete,tab_alter,valAddClm);
          	}
            free(valAddClm);
            result=true;
          }
          else result=false;
        }
        else result=false;
      }
      else result=false;
    }
    else result=false;
  }
  else if(token==ADD){
    token=_lire_token();
    strcpy(tabcree,tab_alter);
    if(_COLUMN_DEFINITION()){
      token=_lire_token();
      result=_COLUMN_DEFINITION_AUX_ALTER();
    }
    else result=false;
  }
  else if(token==DROP){
    token=_lire_token();
    if(token==IDF){
      if(in_list_clm(tete,tab_alter,yytext)==false){
        creer_sm_erreur_declaration(columnNotExist,yylineno,yytext);
      }
      else{
      	//printf("alter drop--1\n");
        tete=_delete_clm(tete,tab_alter,yytext);
        //printf("alter drop--2\n");
        token=_lire_token();
        result=_IDF_AUX();
      }
    }
    else result=false;
  }
  else if(token==WITH){
    token=_lire_token();
    if(_Options()){
      result=true;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _COLUMN_DEFINITION_AUX_ALTER(){
  boolean result;
  if(token==VIRG){
    token=_lire_token();
    if(_COLUMN_DEFINITION_ALTER()){
      token=_lire_token();
      result=_COLUMN_DEFINITION_AUX_ALTER();
    }
    else result=false;
  }
  else if(token==PVIRG){
      result=true;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _IDF_AUX(){
  boolean result;
  if(token==PVIRG){
    result=true;
  }
  else if(token==IDF){
  	//printf("566\n");
    if(in_list_clm(tete,tab_alter,yytext)==false){
        creer_sm_erreur_declaration(columnNotExist,yylineno,yytext);
    }
    else{
     // printf("IDF aux--1\n");
      tete=_delete_clm(tete,tab_alter,yytext);
      //printf("IDF aux--1\n");
      token=_lire_token();
      result=_IDF_AUX();
    }
  }
  return result;
}

//======================================================================//

boolean _DROP_TABLE(){
  boolean result;
  if(token==TABLE){
    token=_lire_token();
    if(token==IDF){
      if(in_Tab_tabl(tete,yytext)==false){
        creer_sm_erreur_declaration(tableNotExist,yylineno,yytext);
      }
      else{
        tete=_delete_table(tete,yytext);
        /*if(tete==NULL){
        	tete=(table*)malloc(sizeof(table));
        	tete->name=(char*)malloc(20*sizeof(char));
        }*/
        token=_lire_token();
        if(token==PVIRG){
          result=true;
        }
        else result=false;
      }
    }
    else if(_IF_EXISTS()){
      token=_lire_token();
      if(token==IDF){
        if(in_Tab_tabl(tete,yytext)==false){
        }
        else{
          tete=_delete_table(tete,yytext);
          token=_lire_token();
          if(token==PVIRG){
            result=true;
          }
          else result=false;
        }
      }
      else result=false;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _TRUNCATE_AUX(){
  boolean result;
  if (token==TABLE){
    token=_lire_token();
    if(token==IDF){         // _TABLE_NAME()
      if(in_Tab_tabl(tete,yytext)==false){
        creer_sm_erreur_declaration(tableNotExist,yylineno,yytext);
      }
      else
      { 
        delete_data(tete,yytext);
        token=_lire_token();
        if(token==PVIRG){
          result=true;
        }
        else result=false;
      }
    }
    else result=false;
  }
  else if(token==IDF){      // _TABLE_NAME()
    if(in_Tab_tabl(tete,yytext)==false){
        creer_sm_erreur_declaration(tableNotExist,yylineno,yytext);
    }
    else{
      delete_data(tete,yytext);
      token=_lire_token();
      if(token==PVIRG){
        result=true;
      }
      else result=false;
    }
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _TRUNCATE(){
  boolean result;
  if(token==TRUNCATE){
  	token=_lire_token();
    result=_TRUNCATE_AUX();
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _TABLE_NAME(){
  boolean result;
  if(token==STR || token==IDF){
    token=_lire_token();
    if(token==PNT){
      token=_lire_token();
      if(token==STR || token==IDF){
        result=true;
      }
      else result=false;
    }
    else{
    	result=true;
    	follow_token=true;
    } 
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _IF_EXISTS(){
  boolean result;
  if(token==IF){
    token=_lire_token();
    if(token==EXISTS){
      result=true;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _TYPE()
{
  boolean result;
  
   if(token==INT)
      {
         result=true;
      } 
    else if(token==DOUBLE)
   {
        result=true;
   }
   else if(token==STRING)
   {
       result=true;
   }
    else if(token==VARINT)
   {
       result=true;
   }
    else if(token==TEXT)
   {
       result=true;
   }
   else result=false;

   return result;

}
  

//======================================================================//

boolean _NOT_EXISTS(){
  boolean result;
  if(token==IF){
    token=_lire_token();
    if(token==NOT){
      token=_lire_token();
      if(token==EXISTS){
        result=true;
      }
      else result=false;
    }
    else result=false;
  }
  else result=false;
  return result;
}

//======================================================================//

boolean _CQL_Type(){
  boolean result;
  if(_Type()){
    result=true;
  }
  /*if(_NATIVE_Type()){
    result=true;
  }
  else if(_COLLECTION_Type()){
    result=true;
  }
  else if(_USER_DEFINED_Type()){
    result=true;
  }
  else if(_TUPLE_Type()){
    result=true;
  }
  else if(_CUSTOM_Type()){
    result=true;
  }*/
  else result=false;
  return result;
}

//======================================================================//


boolean _test(){
  boolean result;
  if(_Type()){
      token=_lire_token();
      if(_PRIMARY_KEY()){
        result=true;
      }
      else if(token==STATIC){
        token=_lire_token();
        if(_PRIMARY_KEY()){
          result=true; 
        }
        else{
          //follow_token=true;
          result=true;
        } 
      }
      
        //follow_token=true;
        result=true;
      
      //result=true;
  }
  else result=false;
  return result;
}

//======================================================================//



 void json_to_table(char * string_table)  
{
  json_t * root = load_json(string_table);
  int size,sizeClm;
  int a;
  int i,j;
  char nom[30];
  json_t * json_name_table;
  json_t * json_defintion_columns , * json_column , * json_name_column , *json_type_column;  
  json_t * json_data;
  columnData *dt;
  column * cl,*d;
  
  cl = (column *)malloc(sizeof(column));
  dt= (columnData *)malloc(sizeof(columnData));  

  json_name_table = json_object_get(root , "nom");
  //char * nom = json_string_value(json_name_table);
  strcpy(nom ,json_string_value(json_name_table));
  printf("Nom Table : %s\n",nom);

  tete=add_table(tete,nom);
   
  json_defintion_columns = json_object_get(root , "data");
  sizeClm =  json_array_size(json_defintion_columns);
  printf("size colonne = %d\n",sizeClm);

  for(j = 0 ; j < sizeClm ; j ++)
  { 
    
    json_column = json_array_get(json_defintion_columns , j);
    json_name_column = json_object_get(json_column , "nomClm");
    json_type_column = json_object_get(json_column , "type");

    //column * cl;
    cl = (column *)malloc(sizeof(column));
    cl->name = (char *)malloc((strlen(json_string_value(json_name_column)) + 1)*sizeof(char));
    strcpy(cl->name , json_string_value(json_name_column));
    cl->typeClm = transformer_string_type(json_string_value(json_type_column)) ;
    //d=cl;
    //printf("\n\n colonne d'ajout= %s\n", cl->name);
    add_clm_table(tete,nom,cl);
  
    json_data = json_object_get(json_column , "values");
    size =  json_array_size(json_data);

    for(i = 0 ; i < size ; i ++)
    {
       json_t *json_values = json_array_get(json_data , i);
       //columnData *dt;
       dt= (columnData *)malloc(sizeof(columnData));
              
       dt->data=(char *)malloc((strlen(json_string_value(json_values))+ 1)*sizeof(char)); 
       strcpy(dt->data , json_string_value(json_values));

       tete=add_clm_data_table(tete,nom,cl->name,dt);
       
    }
  }
  json_decref(root);
}





