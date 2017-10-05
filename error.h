#ifndef ERROR_H
#define ERROR_H


typedef enum {
    keyspaceNotExist, 
    keyspaceAlreadyExist,
    keyspaceNotDeclared,
    tableNotExist, 
    tableAlreadyExist,
    columnNotExist, 
    columnAlreadyExist,
    incompatibleType,
    insertNbFaut
    }SemanticErrorType;

typedef struct {
  char *name;
  int line;
  SemanticErrorType errort;
} smError;


extern smError *creer_sm_erreur(SemanticErrorType et, int line, char* name) ;
extern int nombre_sm_erreurs();
extern void afficher_sm_erreurs() ;

extern void creer_sm_erreur_declaration(SemanticErrorType et, int line, char* name);

extern void afficher_sm_erreur(SemanticErrorType et, int line, char* name);

#endif
