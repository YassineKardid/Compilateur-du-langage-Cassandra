#ifndef TABLE_SYMB_H
#define TABLE_SYMB_H
#include "cassandra.h"
#include <jansson.h>

//int maxClm=100;

typedef enum 
   {
    Int, Bool, Double,Text,Varint,String
   } Type;


typedef struct 
{
  char *name;
}keyspace;


typedef struct columnData
{
  char   *data;        //*name;  //data
  int i;
  struct columnData *next;
 
}columnData;


typedef struct column
{
	char * name;
  Type typeClm;
  boolean primary;
  boolean statiic;
  columnData *tete;
	struct column *next;
}column;


typedef struct table       //tete
{
	char *name;
	column *debut;
	struct table *next;
}table;


typedef struct ClmInsert     //begin
{
  char *name;
  int i;
  struct ClmInsert *next;
}ClmInsert;
typedef struct ClmSelect      //began
{
  char *name;
  int i;
  struct ClmSelect *next;
}ClmSelect;

typedef struct values         //start
{
  Type val;
  char *data;
  int i;
  struct values *next;
}values;


void test_select(table*tete,char *tabSelect,ClmSelect *began);

void charger(int filecount);

void sauvgarder_nb_table(int nbTb);
int chager_nb_table();
void afficher(table*tete);



extern void add_keyspace(char *name);
extern table * add_table (table * tete,char *name);
extern void add_clm_table(table *tete,char * tabname,column *clm);
extern column * add_clm(column* debut,column *clm);


extern table* _delete_table(table* tete,char * tabname);
extern table* _delete_clm(table* tete,char * tabname,char* clmName);
extern void afficher_Type(Type t);
extern Type _chaine_to_type(char* s);
extern void _afficher_table(table* tete, char* tabname);
extern table* find_list_table(char* keyspaceName);
extern column * find_list_clm(table *tete,char * nameTab);
void delete_data(table *tete,char *tabname);

void sauvgarder_tables(table *tete);
void ajout_fichier();
FILE* ouvrir_fichier(char*nom,char*mode);
//table* charger_tables();
void charger_tables(int filecount);
//table* charger_tables(table *tete,int filecount);
void charger_keyspaces();
char * transformer_type_string(Type t);
Type transformer_string_type(const char * val) ;
//table *json_to_table(char * string_table) ;
void json_to_table(char * string_table) ;

void afficher_table_data(table *tete);


extern ClmInsert * add_clm_insert(ClmInsert* debut,char * name);//enregistrement des noms des comlumns dont ont veut inserer
extern values * add_val_insert(values* debut,values *val);//enregistrement des valeur d'insertion

extern ClmSelect * add_clm_select(ClmSelect* debut,char * name);//enregistrement des noms des comlumns dont ont veut selectioneer
void afficher_element(ClmSelect* began,table *tete,values *start,char *tabSelect,char *Clmcondi);

json_t *load_json(const char *text);

//Creation d'une autre structure pour l'enregistrement des valeur s'insertion dans les tables
//extern table *add_clm_data_table(table *tete,char * tabname,columnData *clm); changé
extern column *add_clm_data_clm(column *tete,char * clmName,columnData *clm);
extern columnData * add_data_clm(columnData* debut,columnData *clm);

table *add_clm_data_table(table *tete,char * tabname,char*clmName,columnData *clm);
column *find_clm(table *tete,char * nameTab,char *nameClm) ;
void  insert_veri(table *tete ,values *start,ClmInsert *begin,char *tabcree);

extern boolean in_Tab_keys(char * keyspaceName);
extern boolean in_Tab_tabl(table *tete,char * tabname);
extern boolean in_list_clm(table *tete,char * tabname,char * clmName);


#endif




















/*typedef struct keyspace_options{
    char* name;

    typetoken durable_writes;
    boolean class_check;
    char* class;
    int replication_factor;
    int success;
}keyspace_options;
FILE *file;
keyspace_options keyspace_option;
char* text;
int cursor;
data_error* errs;
data_error* errors;

json_t *root;
json_t *data;
json_t *parser;
json_error_t error;

*/

/*
void Affichage_element(){






        printf("\n\t\t +------------------------------------------------------------------+\n");
        printf("\t\t |+----------------------------------------------------------------+|\n");
        printf("\t\t ||\t      ***** LISTE DONNEES DE LA BASE : *****\t\t    ||");
        printf("\n\t\t |+----------------------------------------------------------------+|\n");
        printf("\t\t +------------------------------------------------------------------+\n\n\n");














 
    table* p = (table*)malloc(sizeof(table*));
    p->nom=(char*)malloc(sizeof(char*));
    p = EVE;
//    printf("les données de la liste chainee\n");
                int j=1;
    while(p != NULL){


        printf("\n");
        printf("\t\t                      >>>  Nom de la table %d: %s  <<<                  \n\n\n",j,p->nom);
  printf("\t\t                      >>>  Enable or not:  %d   <<<                  \n\n\n",p->able);//est ce qu'il est enabled ou disabled


                // affichage de column family
                if(p->CF_nom!=NULL){
    liste_CF* q = (liste_CF*)malloc(sizeof(liste_CF*));
    q = p->CF_nom;
                int i=1;
      while(q != NULL){
                                

        printf("\t\t__________________________________________________________________________");
        printf("\n\t\t|  Nom de la CF  %d                          |       %s\n",i,q->nom);
        printf("\t\t|___________________________________________|____________________________|\n");
//        printf("Nom de la CF %d: %s\n",i,q->nom);
        q = q->suivant;
                                i++;
          }
                  }
                else
    printf("pas de column family a afficher");
    //affichage de row
    if(p->row!=NULL){
                liste* q = (liste*)malloc(sizeof(liste));
    q = p->row;
                int i=1;
      while(q != NULL){
        printf("\t\t__________________________________________________________________________");
        printf("\n\t\t|  Nom de la ligne  %d                       |       %s\n",i,q->nom);
        printf("\t\t|___________________________________________|____________________________|\n");
                                
//        printf("Nom de la ligne %d: %s\n",i,q->nom);
        q = q->suivant;
                                i++;
          }
                  }
    else
            printf("pas de lignes a afficher");
    p = p->suivant;
                j++;
                //printf("sortir de la boucle");
     
  }
  }

*/


