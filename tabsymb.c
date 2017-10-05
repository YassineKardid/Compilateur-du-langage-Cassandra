#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cassandra.h"
#include "tabsymb.h"
#include "error.h"

#include <jansson.h>
#include <sys/types.h> 
#include <dirent.h> 
#include <sys/stat.h> 
extern char *yytext;
extern int yylineno;

#define Nb_m 100                  //NBS
static keyspace Tab_keys[Nb_m]; 
static int Nb_keys = 0;
extern table *tete;
extern ClmInsert * begin;
char *tabInsert;
extern char *tabSelect;

extern columnData *valInsert;
extern column *valAddClm;
extern values *valClm;
extern values *start;
extern char * keysCree;
extern int nbTab;
extern int mult;
int f;



                                    ///////////////////////////////////////////// 
void test_select(table*tete,char *tabSelect,ClmSelect *began)
{
  ClmSelect *s;
  s=began;
  while(s!=NULL)
  {
     if(in_list_clm(tete,tabSelect,s->name)==false)
    {
          creer_sm_erreur_declaration(columnNotExist,yylineno,s->name);
    }
    s=s->next;
  }

}

                                    ///////////////////////////////////////////// 
void charger_tables(int filecount)
{
  printf("\ncharger_tables\n");
  int MAX_CHARS=5000;
  char string_table[MAX_CHARS];
  char temp [30];
  int j = 0;
  char filename[255],ch[10];
  FILE * f;
  int i=0;
  sprintf(filename, "table%d.txt", filecount);
  printf("\n%s\n", filename);
  strcpy(ch , "./");
  strcat(ch,keysCree);
  strcat(ch,"/");
  strcat(ch,filename);
  printf("%s\n",ch );
  f=ouvrir_fichier(ch,"r");
  while(fscanf(f, "%s",temp) != EOF)
  {
        strcat(string_table , temp);
  }
  printf("result : %s\n\n",string_table);
  json_to_table(string_table);
  i=0;
  while(string_table[i]!='\0')
    {
      string_table[i]='\0';
      i++;
    }
  fclose(f);
}
 

                  //////////////////////////////////////////////////
void afficher(table*tete)
{
  //printf("\n\naffichage des tables\n\n\n");
  table *p=tete;
  column *c;
  columnData *dt;
  while(p!=NULL)
  {
    printf("table:  %s\n",p->name );
    c=p->debut;
    while(c!=NULL)
    {
      printf("    colonne: %s\n",c->name );
      dt=c->tete;
      while(dt!=NULL)
      {
         printf("         data: %s \n",dt->data );
         dt=dt->next;
      }      
      c=c->next;
    }
    p=p->next;
  }
}
                  ///////////////////////////////////////////////////
json_t *load_json(const char *text) {
    json_t *root;
    json_error_t error;

    root = json_loads(text, 0, &error);

    if (root) {
        return root;
    } else {
        fprintf(stderr, "json error on line %d: %s\n", error.line, error.text);
        return (json_t *)0;
    }
}
                   //////////////////////////////////////////////////////

                   ///////////////////////////////////////////////////
char * transformer_type_string(Type t)
{
  switch(t){
    case Int : return "integer"; break;
    case String : return "string"; break;
    case Double : return "double"; break;
    case Text : return "text"; break;
    default : return NULL;
  }
}
                    ///////////////////////////////////////////////
Type transformer_string_type(const char * val)    
{
  if(strcmp("integer",val)==0)
  {
    return Int;
  }
  else if(strcmp("string",val)==0)
  {
    return String;
  }
  else if(strcmp("double",val)==0)
  {
    return Double;
  }
  else if(strcmp("text",val)==0)
  {
    return Text;
  }
}     
                  ////////////////////////////////////////////////           
void sauvgarder_nb_table(int nbTb)
{
  FILE *f;
  char ch[10];
  strcpy(ch , "./");
  strcat(ch,keysCree);
  strcat(ch,"/");
  strcat(ch,"nbtab.txt");
  f=ouvrir_fichier(ch,"w");
  fprintf(f,"%d",nbTb);
  fclose(f);
    
}
                    ////////////////////////////////////////////////
int chager_nb_table()
{
  int nbTb;
  FILE *f;
  char ch[10];
  strcpy(ch ,"./");
  strcat(ch,keysCree);
  strcat(ch,"/");
  strcat(ch,"nbtab.txt");
  printf("%s \n", ch);
  f=ouvrir_fichier(ch,"r");
  fscanf(f,"%d",&nbTb);
  fclose(f);
  return nbTb;
}                    
                    /////////////////////////////////////////////////
void charger_keyspaces()
{
   DIR *dir;
  struct dirent *p;
  char *fic = NULL;
  chdir("./data");
  dir=opendir(".");
  while((p = readdir(dir))!=NULL)
  {
        fic = p->d_name;
        if(strcmp(fic , ".") && strcmp(fic , ".."))
        {
          add_keyspace(fic);
          fprintf(stdout, " Nom fichier :%s \n", fic);

        }
  }
  closedir(dir);
  return ;
}
////////////////////////////////////////////////
FILE*ouvrir_fichier(char*nom,char*mode)
{
  //printf("ouvrir fichier\n");
FILE*fv=fopen(nom,mode);
if(fv==NULL)
{
printf("\nimpossible d'ouvrir le fichier\n");
exit(0);
}
return fv;
}
                          ///////////////////////////////////////////////
void sauvgarder_tables(table *tete)
{
printf("\nsauvgarder_tables\n");

char filename[50],ch[10];
int filecount=1,a,maxtab=3;
FILE * f;
table*p;
column *cl;
columnData *dt;
char* s = NULL;
//char sd[255];
p=tete;
if(p==NULL)
{
    printf("\nil n'y'a aucun table a sauvgarder\n");
    return;
}
nbTab=0;
while(p!=NULL)
{
    nbTab++;
    json_t *root = json_object();
    json_t *json_arr = json_array();

    cl=p->debut;
    sprintf(filename, "table%d.txt", filecount);
    printf("\n%s\n", filename);
    strcpy(ch , "./");
    strcat(ch,keysCree);
    strcat(ch,"/");
    strcat(ch,filename);
    printf("%s\n",ch );
    f=ouvrir_fichier(ch,"w");
    json_object_set_new(root , "data" , json_arr);
    
    while(cl!=NULL)
    {
      json_t * json_values = json_array();
      json_t * json_column = json_object();
      dt=cl->tete; 
      //printf("%s clname\n",cl->name );
      json_array_append(json_arr , json_column);
      json_object_set_new(json_column , "type" , json_string(transformer_type_string(cl->typeClm)));
      json_object_set_new(json_column , "nomClm" , json_string(cl-> name));
      json_object_set_new(json_column , "values" , json_values);
          
      while(dt!=NULL)
      {
        //printf("%s dataaaa\n",dt->data );
        json_array_append(json_values , json_string(dt->data));
        //json_array_append(json_values , json_string(dt->data));
        dt=dt->next;
      }
      cl=cl->next;
    }
    json_object_set_new( root,"nom", json_string(p->name) );
    
    filecount++;
    s = json_dumps(root, 0);
    puts(s);
    fputs(s , f);
    json_decref(root);
    p=p->next;    
    fclose(f);
}
}


               //////////////////////////////////////////////
void add_keyspace(char *name)
{
  Tab_keys[Nb_keys].name=(char *)malloc(strlen(name)+1);    //--??
  strcpy(Tab_keys[Nb_keys].name,name);
  Nb_keys++;
}

                    ////////////////////////////////////
table * add_table (table * tete,char *name){

  table *p,*b;
  p=(table*)malloc(sizeof(table));
    //b=(table*)malloc(sizeof(table));
  b=tete;
  if(b==NULL){

      //printf("add_table--1\n");
      p=(table*)malloc(sizeof(table));
      p->name=(char *)malloc(strlen(name)+1);
      strcpy(p->name,name);
      p->next=NULL;
      tete=p;
      return tete;
  }
    else{

      while(b->next != NULL)
      {
        b=b->next;;
      }
      //printf("add_table--2\n");
      p->name=(char *)malloc(strlen(name)+1);
      strcpy(p->name,name);
      p->next=NULL;
      b->next=p;  
    }
    
    return tete;  

}    
                    ////////////////////////////////////
void add_clm_table(table *tete,char * tabname,column *clm){
  table *p;
  int k=0;
  p=tete;
  while(p!=NULL && k==0)
  {
        if(strcmp(p->name,tabname)==0){
          k=1;
        }
        else{
          p=p->next;
        }
  }
  if(k==1)
  { 
    p->debut=add_clm(p->debut,clm);
  }
  else printf("\nerror tabcree not in tete\n");
} 
                    ///////////////////////////////////
column * add_clm(column* debut,column *clm)
{
  column *p,*b;
  int a;
  p=(column*)malloc(sizeof(column));
  b=debut;
  if(debut==NULL)
  {
      //printf("\nAddclm--1\n");
      p->name=(char *)malloc(strlen(clm->name)+1);
      strcpy(p->name,clm->name);
      p->typeClm=clm->typeClm;
      p->primary=clm->primary;
      p->statiic=clm->statiic;
      p->next=NULL;
      debut=p;
      return debut;
  }
  else
  {
    while(b->next!=NULL){ b=b->next; }
    //printf("addclm--2\n");
    p->name=(char *)malloc(strlen(clm->name)+1);
    strcpy(p->name,clm->name);       
    p->typeClm=clm->typeClm;
    p->primary=clm->primary;
    p->statiic=clm->statiic;
    p->next=NULL;
    b->next=p;
    return debut;
  }
   
}    
                  /////////////////////////////////////
ClmInsert * add_clm_insert(ClmInsert* debut,char * name)
{
  ClmInsert *p,*c,*b;
  int a;
  /*printf("\nclminsert\n");
  printf("%s\n",name );*/
  p=(ClmInsert*)malloc(sizeof(ClmInsert));
  b=debut;
  if(debut==NULL)
  {
        p=(ClmInsert*)malloc(sizeof(ClmInsert));
        p->name=(char *)malloc(strlen(name)+1);
        strcpy(p->name,name);
        p->i++;
        debut=p;
        debut->next=NULL;
        return debut;
  }
   c=b->next;
  while(c!=NULL)
  {

    b=b->next;
    c=c->next;
  }
  p->name=(char *)malloc(strlen(name)+1);
  strcpy(p->name,name);
  p->i++;
  b->next=p;
  p->next=NULL;      
return debut; 
}            

                    ////////////////////////////////////
values * add_val_insert(values* debut,values *vall)
{
  int a;
  /*printf("\nvalinsert\n");
  printf("%s\n",vall->data); */
  values *p,*c,*b;
  
  p=(values*)malloc(sizeof(values));
  b=debut;
  if(debut==NULL)
  {
        p=(values*)malloc(sizeof(values));
        p->data=(char *)malloc(strlen(vall->data)+1);
        strcpy(p->data,vall->data);
        p->val=vall->val;
        p->i++;
        debut=p;
        debut->next=NULL;
        return debut;
  }
  c=b->next;
  while(c!=NULL)
  {
    b=b->next;
    c=c->next;
  }
       p=(values*)malloc(sizeof(values));
       p->data=(char *)malloc(strlen(vall->data)+1);
       strcpy(p->data,vall->data); 
       p->val=vall->val;
       p->i++;   
       b->next=p;
       p->next=NULL;
  
return debut;
}            


                    ////////////////////////////////////        ____ non changé
columnData * add_data_clm(columnData* debut,columnData *clm)
{
  columnData *p,*c,*b;
  int a;
  /*printf("\nadd_data_clm\n");
  printf("%s\n", clm->data);*/
  p=(columnData*)malloc(sizeof(columnData));
  b=debut;
  if(debut==NULL)
  {
        p=(columnData*)malloc(sizeof(columnData));
        p->data=(char *)malloc(strlen(clm->data)+1);
        strcpy(p->data,clm->data);
        //printf("%s daaa\n",p->data );
        p->i++;
        debut=p;
        debut->next=NULL;
        return debut;
  }
  c=b->next;
  while(c!=NULL)
  {

    b=b->next;
    c=c->next;
  }
        p=(columnData*)malloc(sizeof(columnData));
        p->data=(char *)malloc(strlen(clm->data)+1);
        strcpy(p->data,clm->data);
        p->i++;
        b->next=p;
        p->next=NULL;
  
return debut; 
}

                ///////////////////////////////////////          

                ////////////////////////////////////////

 column *add_clm_data_clm(column *tete,char * clmName,columnData *clm)
{
  column *p;
  int k=0,a;
  
  p=(column*)malloc(sizeof(column));
  p=tete;
  while(p!=NULL&&k==0)
  {
        /*printf("\nujj\n");*/   
        if(strcmp(p->name,clmName)==0)
        {
          /*printf("\nqwsx\n");*/
          k=1;
        }
        else {p=p->next;}
  }
  if(k==1)
  {
    p->tete=add_data_clm(p->tete,clm);
  }
  else printf("\nerror column n'existe pas l'ajout du data\n");
return tete;          // modificationnnnnnnnnnnnnnnn return p; 

} 
                  //////////////////////////////////
table *add_clm_data_table(table *tete,char * tabname,char*clmName,columnData *clm)
{
  table *p;
  column *c;
  int k=0,t=0,a;
        //printf("\nadd_data_clm_table\n");
  p=(table*)malloc(sizeof(table));
  column *d=(column*)malloc(sizeof(column));
  p=tete;
  d=tete->debut;
  while(p!=NULL&&k==0)
  {
        /*printf("\nllll\n");*/     
        if(strcmp(p->name,tabname)==0)
        {
          /*printf("\ncvvcc\n");*/
          k=1;
        }
        else {p=p->next;}
  }
  if(k==1)
  {
    c=p->debut;
     while(c!=NULL&&t==0)
     {
      /*printf("\nfff\n");*/
      if(strcmp(c->name,clmName)==0)
      {

         //d=add_clm_data_clm(d,clmName,clm);   ///////////// d
        c->tete=add_data_clm(c->tete,clm);
        t=1;
      }
      else{c=c->next;}
     }
  }
  else printf("\nerror table n'existe pas function add_clm_data_table\n");
return tete;                          // modificationnnnnnnnnnnnnnnnnnnnnn
}                   
                   //////////////////////////////////

void  insert_veri(table *tete,values *start,ClmInsert *begin,char *tabcree)
{
  int a;
  /*printf("\ninserVeri\n");*/

  values *p=(values*)malloc(sizeof(values));
  ClmInsert *q=(ClmInsert*)malloc(sizeof(ClmInsert));
  column *d=(column*)malloc(sizeof(column));
  column *cl=(column*)malloc(sizeof(column));
  columnData *info=(columnData*)malloc(sizeof(columnData));
  table *l=(table*)malloc(sizeof(table));
  p=start;
  q=begin;
  l=tete;
  int k=0,t=0;
  while(p!=NULL && q!=NULL)
  {
     d=(column*)malloc(sizeof(column));

     d=find_clm(tete,tabcree,q->name);
     if(f==1)
     {
      k=1;
     }
     
     if(f!=1 && d->typeClm!=p->val)
     {
       creer_sm_erreur_declaration(incompatibleType,yylineno,q->name);
       k=1;
       printf("\ntype differentes\n");
     }
       f=0;
       p=p->next;
       q=q->next;
       //free(d);    
  }
  if(p!=NULL|| q!=NULL)
  {
    printf("\nnombre de declaration differente\n");
    creer_sm_erreur_declaration(insertNbFaut,yylineno,tabcree);
    t=1;
  }
  if(k==0&&t==0)
  {
     p=start;
     q=begin;
     while(p!=NULL && q!=NULL)
     {
      /*printf("\nwwww\n");*/
     info=(columnData*)malloc(sizeof(columnData));
     info->data=(char *)malloc(strlen(p->data)+1);
     strcpy(info->data,p->data);
     l=add_clm_data_table(l,tabcree,q->name,info);
     p=p->next;
     q=q->next;
     //free(info);
    }
  }
}
           ///////////////////////////////////

ClmSelect * add_clm_select(ClmSelect* debut,char * name)
{
  ClmSelect *p,*c,*b;
  int a;
  /*printf("\nclmselect\n");
  printf("%s\n",name );*/
  p=(ClmSelect*)malloc(sizeof(ClmSelect));
  b=debut;
  if(debut==NULL)
  {
        p=(ClmSelect*)malloc(sizeof(ClmSelect));
        p->name=(char *)malloc(strlen(name)+1);
        strcpy(p->name,name);
        p->i++;
        debut=p;
        debut->next=NULL;
        return debut;
  }
   c=b->next;
  while(c!=NULL)
  {

    b=b->next;
    c=c->next;
  }
  p->name=(char *)malloc(strlen(name)+1);
  strcpy(p->name,name);
  p->i++;
  b->next=p;
  p->next=NULL;      
return debut; 
} 
          ////////////////////////////////////////////

column *find_clm(table *tete,char * nameTab,char *nameClm) 
{
  column *c;
  table *p=(table*)malloc(sizeof(table));
  p=tete;
  int k=0,t=0;
  while(p!=NULL && k==0)
  {
    if(strcmp(p->name,nameTab)==0)
    {
      k=1;
    }
    else{
      p=p->next;
    }
  }
  if(p!=NULL && k==1)
  {
    c=p->debut;
    while(c!=NULL && t==0){
      if(strcmp(nameClm,c->name)==0)  //nameClm
      {
        t=1;
        return c;
      }
      else
      {
        c=c->next; 
      }
    }
  }
  /*if(t==0)
  {
     creer_sm_erreur_declaration(columnNotExist,yylineno,nameClm);
  }
/*if(p!=NULL)  
return p->debut;*/
}             
          
                       ///////////////////////////////////////////

column * find_list_clm(table *tete,char * nameTab) 
{
  column *c;
  table *p;
  p=tete;
  int k=0;
  while(p!=NULL && k==0)
  {
    if(strcmp(p->name,nameTab)==0)
    {
      k=1;
    }
    else{
      p=p->next;
    }
  }
  if(p!=NULL && k==1){
    c=p->debut;
    return c;
  }  
  else printf("erreur find_listClm():column n existe pas\n");  
 }

                ///////////////////////////////////////////

void afficher_element(ClmSelect* began,table *tete,values *start,char *tabSelect,char *Clmcondi)
{
  ClmSelect *s;
  table *p,*e;
  column *c,*condi;
  columnData *d,*f;
  values *v;
  int va[10];
  int i=0,max=0,a;
 //printf("affichage function\n");

  s=(ClmSelect*)malloc(sizeof(ClmSelect));
  c=(column*)malloc(sizeof(column));
  condi=(column*)malloc(sizeof(column));
  d=(columnData*)malloc(sizeof(columnData));
  f=(columnData*)malloc(sizeof(columnData));
  p=(table*)malloc(sizeof(table));

  s=began;
  p=tete;
  v=start;

  if(p==NULL)
  {
    return;
  }
  condi=find_clm(p,tabSelect,Clmcondi);
  
  if(condi==NULL)
  {
    //printf("\nzzzz\n");
    return;
  }
  f=condi->tete;
  while(f!=NULL)
  {
    if(strcmp(f->data,valClm->data)==0)        //if(strcmp(f->data,v->data)==0)
    {
        va[i]=f->i;
        i++;
    }
    f=f->next;
  }
  max=i;
  i=0;
  printf("\n    +-------------------------------------------------------------------------------+\n");
  while(s!=NULL)
  {
    /*printf("\ngggg\n");
    printf("%s jjj\n",s->name);*/    
    printf("    |  %s\t:|",s->name );
    //printf("\n|----------|");
    c=find_clm(p,tabSelect,s->name);
    if(c==NULL)
    {
      return ;
    }
    d=c->tete;
    //printf("\nrrrr\n");
    while(i<max)
    {
      //printf("\nqqqq\n");
      if(d->i==va[i])
      {
        /*printf("%s   afffichaaage\n",d->data);*/
         printf("\t%s\t|",d->data );
        i++;
      }
      d=d->next;      
    }
    printf("\n");
    i=0;
        s=s->next;
  }
  printf("    +-------------------------------------------------------------------------------+\n");
}
                      /////////////////////////////////////////////////

void afficher_table_data(table *tete)
{
   table *p;
   p=tete;
   column *c;
   columnData *d;
   int k=0;
  while(p!=NULL && k==0)
  {
     if(strcmp(p->name,tabSelect)==0)
     {
        /*printf("\ncmp\n");*/
        k=1;
     }
     else{
      //printf("\ntable n'existe pas\n");
      p=p->next;}
  }
  c=p->debut;
  if(c==NULL){return;}
  printf("\n    +-------------------------------------------------------------------------------+\n");
  while(c!=NULL)
  {
    d=c->tete;
    printf("    |  %s\t:|",c->name );
    while(d!=NULL)
    {
      
      printf("\t%s\t|",d->data );
      d=d->next;
    }

  printf("\n");
   c=c->next;
  }
  printf("    +-------------------------------------------------------------------------------+\n");
}

//d=find_clm(tete,tabcree,q->name);
/*
    printf("\nrrr\n");
    scanf("%d",&a);*/
                  
                      ///////////////////////////////////////////

                    ///////////////////////////////////////////
                    ///////////////////////////////////////////
                    ///////////////////////////////////////////

boolean in_Tab_keys(char * keyspaceName){

  int i =0;
  while ((i < Nb_keys) && (strcmp(Tab_keys[i].name,keyspaceName) != 0)){
    i++;
  } 

  if (i == Nb_keys) return false;
  else {return true;}
}
                        ///////////////////////////////////////// ,,???
boolean in_Tab_tabl(table *tete,char * tabname){

    table *p;
    p=tete;
    int k=0;
    while (p!=NULL && k==0 )
    {
      if(strcmp(p->name,tabname) == 0){
        k=1;
      }
    else
    {
      p=p->next;
      }
    } 
    if (k == 0) return false;
    else return true;
}
                       ////////////////////////////////////////
boolean in_list_clm(table* tete,char * tabname,char * clmName){

    int k=0,m=0;
    table *p;
    column *l;
    p=tete;                                              //-------------------------??
    while (p!=NULL && k==0)
    { 
      if(strcmp(p->name,tabname) == 0)
      {
        k=1;        
      }
      else{ p=p->next; }
    } 
    if(k==1 && p!=NULL)
    {
        l=p->debut;
      while(l!=NULL && m==0)
      {
        if(strcmp(l->name,clmName)== 0){
          m=1;
          }
          else{ l=l->next; }
      } 
    }
    if (m== 0) return false;
    else return true;
    
}                          

//==================================================================//

table* _delete_table(table* tete,char * tabname){
  int k=0;
  table *p,*q;
  if(tete==NULL) return tete;
  q=tete;
  if(strcmp(q->name,tabname)==0){
    k=1;
    tete=q->next;
    free(q);
    return(tete);
  }
  p=q->next;
  while(p!=NULL){
    if(strcmp(p->name,tabname)==0){
      q->next=p->next;
      free(p);
      return tete;
    }
    q=p;
    p=p->next;
  }
  if(k==0) printf("error _delete_table()\n");
  return tete;
}

/*table* _delete_table(table* tete,char * tabname){
  int k=0;
  table *p,*q;
  p=tete;
  if(p!=NULL){
    while(p!=NULL && k==0){
      if(strcmp(p->name,tabname)==0){
        k=1;
      }
      else{
        p=p->next;
      }
  }
  if(k==1){
    q=p;
    p=q->next;
    free(q);
    return tete;
  }
  }
  printf("erreur _delete_table(): table n'existe pas\n");
}*/

//==================================================================//

table* _delete_clm(table* tete,char * tabname,char* clmName){
  column *p, *q, *list;
  list=find_list_clm(tete,tabname);
  int k=0;
  if(list==NULL){ return tete; }
  q=list;
  if(strcmp(q->name,clmName)==0){
    list=q->next;
    free(q);
    table *t;
    t=tete;
    while(t!=NULL && k==0)
    {
      if(strcmp(t->name,tabname)==0)
      {
        k=1;
      }
      else{
        t=t->next;
      }
    }
    if(t!=NULL && k==1){
      t->debut=list;
    }
    return tete;
  }
  p=q->next;
  while(p!=NULL){
    if(strcmp(p->name,clmName)==0){
      q->next=p->next;
      free(p);
      return tete;
    }
    else{
      q=p;
      p=p->next;
    }
  }
  printf("error _delete_clm()\n");
  return tete;
}


//==================================================================//

void afficher_Type(Type t){  
  
  switch (t){

  case Int: printf("Int\n"); break;
  case Bool: printf("Bool\n"); break;
  case Double: printf("Double\n"); break;
  case Text: printf("Text\n"); break;
  case Varint: printf("Varint\n"); break;
  case String: printf("String\n"); break;

  }

}

//==================================================================//

Type _chaine_to_type(char* s){
  Type t;

  if(strcmp(s,"Int")==0) t=Int;
  else if(strcmp(s,"Bool")==0) t=Bool;
  else if(strcmp(s,"Double")==0) t=Double;
  else if(strcmp(s,"Text")==0) t=Text;
  else if(strcmp(s,"Varint")==0) t=Varint;
  else if(strcmp(s,"String")==0) t=String;
  
  return t;
}

//==================================================================//

void _afficher_table(table* tete, char* tabname){
  int k=0;
  table* p;
  column *c;
  p=tete;
  while(p!=NULL && k==0){
    if(strcmp(p->name,tabname)==0){
      k=1;
    }
    else{
      p=p->next;
    }
  }
  if(k==1){
    c=p->debut;
    while(c!=NULL){ 
      printf("Name-Table:%s\n",tabname);
      printf("Name-Clm:%s\n",c->name);
      printf("Type-Clm:");
    afficher_Type(c->typeClm);
      c=c->next;
    }
  }
  else printf("Error:afficher_table:table vide\n");
}

//==================================================================//
  void delete_data(table *tete,char *tabname)
  {
  int k=0;
  table *p;
  column *c;
  columnData *d;
  while(p!=NULL && k==0)
    {
      if(strcmp(p->name,tabname)==0)
      {
        k=1;
      }
      else{
        p=p->next;
      }
    }
    if(p!=NULL && k==1){
      c=p->debut;
      while(c!=NULL){
        d=c->tete;
        free(d);
        c->tete=NULL;
        c=c->next;
      }
    }
    else{ printf("erreur:table n'existe pas dans le keyspace\n"); }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






