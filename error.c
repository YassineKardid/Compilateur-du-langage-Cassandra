#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define nbErrMax 200
static int nbErrDec = 0;    //NBERRDECL
static int nbErr=0;
int iniErr;

static smError * ErDec[nbErrMax];    //ERDECL[NBERRMAX]   


smError * creer_sm_erreur(SemanticErrorType et, int line,char* name){
  smError * err = (smError*) malloc (sizeof (smError) );
  err->name = (char *) malloc (strlen(name));
  strcpy(err->name, name);
  err->line=line;
  err->errort = et;
  return err;
}
                             ///////////////////////////////////////////

                             ///////////////////////////////////////////

int nombre_sm_erreurs(){
  return  nbErrDec;
}
                               ////////////////////////////////////////

void afficher_sm_erreurs(){
  int idecl = 0;
  nbErr=1;
  printf("\n");
  while (idecl < nbErrDec) {
    afficher_sm_erreur(ErDec[idecl]->errort,ErDec[idecl]->line, ErDec[idecl]->name);
    idecl++;
    nbErr++;
  }

}
                                /////////////////////////////////////////////

void afficher_sm_erreur(SemanticErrorType et, int line, char* name){  
   
  printf("\tErreur %d : '%s' ", nbErr,  name);
  switch (et){
  case keyspaceNotExist: printf("Keyspace n'est pas cree \n"); break;
  case keyspaceAlreadyExist:  printf("kayspace deja cree\n"); break;
  case keyspaceNotDeclared: printf("Aucun Keyspace déclarée:Essayer la requete:use keyspace_name; \n"); break;
  case tableAlreadyExist:  printf("table deja cree\n"); break;
  case tableNotExist:  printf("table n'est pas cree\n"); break;
  case columnNotExist:  printf("column n'exist pas\n"); break;
  case columnAlreadyExist:  printf("column deja exist\n"); break;
  case incompatibleType : printf("incompatible type de definition\n");break;
  case insertNbFaut : printf("nombre de valeurs different aux nombre de colonnes\n"); break;
  }
}
                             /////////////////////////////////////////
void creer_sm_erreur_declaration(SemanticErrorType et, int line, char* name){
   iniErr++;
   ErDec[nbErrDec++]= creer_sm_erreur(et, line, name); 
}