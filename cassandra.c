
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <jansson.h>
#include "cassandra.h"
#include "error.h"
#include "tabsymb.h"

#include <sys/dir.h>

extern columnData *valInsert;
extern column *valAddClm;
extern table *tete;
extern ClmInsert * begin;
extern values *valClm;
extern values *start;
extern ClmSelect *began;
extern int nbTab;

extern char *yytext;
extern int yylineno;
extern int yylex();
extern char * keysCree;
extern int f;
extern int mult;
extern int iniErr;


boolean follow_token = false;
typetoken _lire_token(){
	if (follow_token)
	{
	    follow_token = false; 
	    return token; 
	}
	else
    { 
		return (typetoken) yylex();
	}
}
                      /////////////////////////////////////
int main(int argc, char const *argv[])
{
    int v=0,t=0;
    iniErr=0;
    nbTab=0;
    f=0;
    mult=0;
    DIR *dir;
    struct dirent *pc;
    char *fic = NULL;
    chdir("./data");
    dir=opendir(".");
    while((pc = readdir(dir))!=NULL)
    {
        fic = pc->d_name;
        if(strcmp(fic ,".") && strcmp(fic ,".."))
        {
          add_keyspace(fic);
          fprintf(stdout, " Nom keyspace :%s \n", fic);

        }
    }
    valClm=(values*)malloc(sizeof(values));
    valInsert=(columnData*)malloc(sizeof(columnData));
    valAddClm=(column*)malloc(sizeof(column));
    begin=(ClmInsert*)malloc(sizeof(ClmInsert));
    start=(values*)malloc(sizeof(values));
    began=(ClmSelect*)malloc(sizeof(ClmSelect));
    
    valInsert->i=0;
    began->i=0;
    begin->i=0;
	valClm->i=0;

    tete=(table*)malloc(sizeof(table));
    tete->name=(char *)malloc(20*sizeof(char));
    tete->debut=(column*)malloc(sizeof(column));
	tete->debut->tete=NULL;
	tete->debut=NULL;	
	start=NULL;
	begin=NULL;
	began=NULL;
	tete=NULL;

    printf("\n\n|>>>> ");
    token = _lire_token();
    if(_Use())
    {
        t=1;
        int i=1;
        if (nombre_sm_erreurs() == 0)
            {

                
                printf("chargement ... ?\n");
                int cha=0;
                scanf("%d",&cha);
                if(cha==0)
                {
                    nbTab=chager_nb_table();
                    printf("keyspace used\n");
                }                
                while(i<=nbTab && cha==0)
                {
                    charger_tables(i);
                    i++;   
                }
                afficher(tete);
            }
        else
        {
            printf("\n\n%d Erreurs sémantiques\n", nombre_sm_erreurs());
            afficher_sm_erreurs();
        }          
    
    }    
    
    while(v==0)
    {   
        iniErr=0;    
        if(t==1)
        {
            printf("\n\n|>>>> ");
            token = _lire_token();
        }
    		    
	    if(_Insert()||_Delete()||_Create()||_Select()||_Alter()||_Use()||_Drop()||_TRUNCATE())
	    {

	    	if (iniErr== 0)
            {

	             printf("EXECUTION REUSSITE\n");
	        }
	        else
            {
                printf("\n\n%d Erreurs sémantiques\n", nombre_sm_erreurs());
                afficher_sm_erreurs();
	       }

	    }	
	    else
	    {
	        printf("plusieurs erreurs syntaxiques\n");
    
            printf("%d erreurs sémantiques\n", nombre_sm_erreurs());
            if (nombre_sm_erreurs()> 0) afficher_sm_erreurs();
	    }
	    printf("\nentrer '0' pour continuer\n");
	    scanf(" %d",&v);
        if(t==0 && v==0)
        {
            printf("\n\n|>>>> ");
            token=_lire_token();
        }
   
    }

  
    printf("\n\n\nsauvgarde... ?\n");
    int sau=0;
    scanf("%d",&sau);
    if(sau==0)
    {
        sauvgarder_tables(tete);
        sauvgarder_nb_table(nbTab);
    }
    
    closedir(dir);
    
}

    
                     //////////////////////////////////////                                 
                     //////////////////////////////////////
boolean _Create()
 {
 	//char  ch[10];
 	boolean result;
 	if(token==CREATE)
    {
        token=_lire_token();
        if(token==KEYSPACE)
        {
            token=_lire_token();
            if(token==IDF)
            {
            	if(in_Tab_keys(yytext)==true)
            	{
            		creer_sm_erreur_declaration(keyspaceAlreadyExist,yylineno, yytext);
            	}
            	                
            	else
            	{ 
                    int nbTb=20;
            		mkdir(yytext,"750");
            		add_keyspace(yytext);          
            	}

                token=_lire_token(); 
                if(token==WITH)
                {
               	   token=_lire_token();
               	   if(_Options())
               	   {
               	   	result=true;
               	   }
               	   else{result=false;}
                }
               else{result=false;}
            }
            else{result=false;}

        }
        else if(_TABLE())
        {
        	result=true;
        }
        else{result=false;}
 	}
 	else{result=false;}

return result;	  
}

                     //////////////////////////////////////
boolean _Alter()
{
    boolean result;
    if(token==ALTER)
    {
    	token=_lire_token();
    	if(token==KEYSPACE)
    	{
    		token=_lire_token();

    		if(token==IDF)
    		{
    			if(in_Tab_keys(yytext)==false)
            	{
            		creer_sm_erreur_declaration(keyspaceNotExist,yylineno, yytext);
            	}	
    			token=_lire_token();
                if(token==WITH)
                {   
                	token=_lire_token();
                	if(_Options())
                	{
                		result=true;
                	}
                	else{result=false;}	
                }
                else{result=false;}
            }
            else{result=false;} 
    	}
    	else if(_ALTER_TABLE()){result=true;}
    }
    else{result=false;}
return result;    
}
                   ////////////////////////////////////////
boolean _Drop()
{
	boolean result;
	if(token==DROP)
	{
		token=_lire_token();
		if(token==KEYSPACE)
		{
			token=_lire_token();
			if(token==IDF)
            {
            	if(in_Tab_keys(yytext)==false)
            	{
            		creer_sm_erreur_declaration(keyspaceNotExist,yylineno, yytext);
            	}
				token=_lire_token();
				if(token==PVIRG)
				{
					result=true;
				}
				else{result=false;}
            }
            else if(token==IF)
            {
            	token=_lire_token();
                if(token==EXISTS)
                {
                	token=_lire_token();
					if(token==IDF)
		            {
						token=_lire_token();
						if(token==PVIRG)
						{
							result=true;
						}
						else{result=false;}
                    }
                    else{result=false;}
                }
                else{result=false;}    
            }
            else{result=false;}
		}
		else if(_DROP_TABLE()){result=true;}
	}
	else{result=false;}
return result;	
}                   
                    ////////////////////////////////////////
boolean _Use()
 {//int a;
 	boolean result;
 	if(token==USE)
 	{
 		token=_lire_token();
 	    if(token==IDF)
 	    {
 	    	if(in_Tab_keys(yytext)==false)
            	{

            		
            		creer_sm_erreur_declaration(keyspaceNotExist,yylineno, yytext);
            	}

            	else
                {
                    keysCree = (char *)malloc(sizeof(strlen(yytext +1)));
                    strcpy(keysCree , yytext);
                }
                
 	     	token=_lire_token();
            if(token=PVIRG)
            {
            	result=true;
            }
            else{result=false;}  
        }
        else{result=false;}     
    } 
    else {result=false; }
return result;
}

                        /////////////////////////////////////////

boolean _Type()
{
	boolean result;

	if(token==INT)
    {
    	valAddClm->typeClm=Int;
        result=true;
	} 
	else if(token==STRING)
	{
		valAddClm->typeClm=String;
	 	result=true;
	}

	else if(token==VARINT)
	{
		valAddClm->typeClm=Varint;
	 	result=true;
	}

	else if(token==TEXT)
	{
	 	valAddClm->typeClm=Text;
	 	result=true;
	}
	else if(token=BOOL)
	{
	  	valAddClm->typeClm=Bool;
	  	result=true;
	}
	else if(token=DOUBLE)
	{
	  	valAddClm->typeClm=Double;
	  	result=true;
	}
	else result=false;

return result;

}


                        /////////////////////////////////////////
boolean _Const(){
    boolean result ;
    int a;
    if (token == INUMBER) 
    {
    	valClm->data=(char *)malloc(strlen(yytext)+1);
 		strcpy(valClm->data,yytext);
        valClm->val=Int;
        result = true;
         }
    else if (token == DNUMBER)  
    {
        valClm->data=(char *)malloc(strlen(yytext)+1);
	    strcpy(valClm->data,yytext);
        valClm->val=Double;
        result = true;
    }
    else if (token ==TRUE||token==FALSE)
    { 
        valClm->data=(char *)malloc(strlen(yytext)+1);
 		strcpy(valClm->data,yytext);
        valClm->val=Bool;
        result = true;
    }
    else if (token == STR)  
    {
        valClm->data=(char *)malloc(strlen(yytext)+1);
 		strcpy(valClm->data,yytext);
        valClm->val=String;
        result = true;
    }
   
    else{ result = false;}

 return result;
}

                        /////////////////////////////////////////
boolean _Options()
{
	boolean result;
	if(_List_option())
	{
		result=true;
	}
	else{ result=false;}
return result;	
}
                        /////////////////////////////////////////
boolean _List_option()
{
	boolean result;
	if(_Option())
	{
		token=_lire_token();
		result=_List_option_aux();	
	}
	else
	{
		result=false;
	}
return result;

}
                       ///////////////////////////////////////
boolean _List_option_aux()
{
	boolean result;
	if(token==PVIRG)
	{
		result=true;
		//follow_token=true;
	}
	else if(token==AND)
	{     
		token=_lire_token();
		if(_List_option())
		{
			result=true;
		}
		else
		{
			result=false;
		}
	}
	else
	{
		result=false;
	}
return result;
}                       
                        ///////////////////////////////////////
boolean _Option()
{
	boolean result;
	if(token==IDF)
	{
		token=_lire_token();
		if(token==EQ)
		{
			token=_lire_token();
			if(_Option_aux())
			{
				result=true;
			}
			else{result=false;}
		}
		else {result=false;}
	}
    else {result=false;}
return result;

}     
                        ////////////////////////////////////////
boolean _Option_aux()
{
	boolean result;
	if(token==IDF)
	{
       result=true;
	}
	else if(_Const())
	{
       result=true;
	}
	else if(_Map_literal())
	{
       result=true;
	}
	else
	{
       result=false;
	}
return result;
}                                           
                        /////////////////////////////////////////
 boolean _Term()
 {
 	boolean result;

 	if(_Const())
 	 {
        result=true;
 	}
 	else if(_Literal()) 
 	{
        result=true;
 	}
 	else if(_Function_call())
 	{
        result=true;
 	}
 	else if(_Type_hint())
 	{
          result=true;
 	}
 	else if(_Bind_marker())
 	{
        result=true;
 	}
 	else
 	{
 	 	result=false;
 	}
return result;
 }     
                        ///////////////////////////////////////
boolean _Literal()      
{
 	boolean result;
 	if(_List_literal())
	{
        result=true;
	}
    else if(token==AOPEN)
    {
    	token=_lire_token();
    	if(_List_udt_literal())
    	{
    		result=true;
    	}
        else if(_Term())               //n'accepte pas _function_call dans le term
    	{
    		token=_lire_token();
    		if(_List_set_literal_aux())
    		{
                result=true;
    		}
    		else if(token==DPNT)
    		{
    			token=_lire_token();
    			if (_Term())
    			{
    				token=_lire_token();
    				if(_List_map_literal_aux())
    				{
                        result=true;
    				}
    				else{result=false;}
    			}
    			else{result=false;}
    		}
    		else{result=false;}	
    	}
        else{result=false;}
    }
 	else if(_Tuple_literal())
 	{  
         result=true;          
 	}
 	else
 	{
 		result=false;
 	}
return result;
}
              
                       ///////////////////////////////////////NVVV
boolean _Collection_literal()
{
	boolean result;
    if(_List_literal())
	{
        result=true;
	}
	else if(token==AOPEN)
	{
		token=_lire_token();
		if(_Term())
		{
			token=_lire_token();
			if(_List_set_literal_aux())
			{ 
				result=true; 
			}
			else if(token==DPNT)
			{   
				token=_lire_token();
				if(_Term())
				{
					token=_lire_token();
					if(_List_map_literal_aux())
				    {
				        result=true;	
				    }
				    else{result=false;}
				}
		     	else{result=false;}	
			}
			else{result=false;}
		}
		else{result=false;}
	}
	else{result=false;}
return result;	
}

                       ///////////////////////////////////////
boolean _Map_literal()
{
	boolean result;
	if(token==AOPEN)
	{
		token=_lire_token();
		if(_List_map_literal())
		{
            result=true;
		}
		else {result=false;}
	}
	else {result=false;}
return result;	
}
                       //////////////////////////////////////

boolean _Map_literald()
{
	boolean result;
	if(_Term())
	{
        token=_lire_token();
        if(token=DPNT)
        {
            token=_lire_token();
          	if(_Term())
          	{
          	 	result=true;
          	}
            else {result=false;}
        }
        else {result=false;}
	}
	else {result=false;}
 return result;
}                          
                        //////////////////////////////////////
boolean _List_map_literal()
{
    boolean result;
    if(_Map_literald())
    {
    	token=_lire_token();
    	result=_List_map_literal_aux();
    }
    else 
    {
    	result=false;
    }
 return result;

}   
                        //////////////////////////////////////
boolean _List_map_literal_aux()
{
	boolean result;
	if (token==VIRG)
	{
		token=_lire_token();
		if(_List_map_literal())
		{
			result=true;
		}
		else
		{
            result=false;
		}
	}
	else if(token==ACLOSE)
	{
		result=true;
		//follow_token=true;
	}
return result;

}                                                                          
                        ///////////////////////////////////////
boolean _Set_literal()
{
	boolean result;
	if(token==AOPEN)
	{
		token=_lire_token();
        if(_List_set_literal())
        {
    	    result=true;
        }
        else{result=false;}
       
    }
    else{result=false;}
   
return result;   
}
                        ///////////////////////////////////////
boolean _Set_literald()
{
    boolean result;
    if(_Term())
    {
		result=true;
	}
	else
	{
		result=false;
	}
   
return result;
}                       
                          ///////////////////////////////////////
boolean _List_set_literal()
{
    boolean result;
    if(_Set_literald())
    {
 	    token=_lire_token();
 	    result=_List_set_literal_aux();
    }
    else 
    {
        result=false;
    }
 return result;

}   
                          ///////////////////////////////////////
boolean _List_set_literal_aux()
{
   boolean result;
   if(token==VIRG)
   {
	   token=_lire_token();
	   if(_List_set_literal())
	   {
		   result=true;
	   }
	   else
	   {
		   result=false;
	   }
    }
    else if(token==ACLOSE)
    {
	result=true;
	//follow_token=true;
    }
return result;
}                  
                          //////////////////////////////////////
boolean _List_literal()
{
	boolean result;
	if(token==COPEN)
	{
		token=_lire_token();
		if(_List_list_literal())
		{
			result=true;
		}
		else{ result=false;}
	}
	else{ result=false;}
 return result;	
}                                                         
                          ///////////////////////////////////////
boolean _List_literald()
{
    boolean result;
   	if(_Term())
	{
		result=true;
	}
	else
	{
		result=false;
	}
return result;

}     
                        ////////////////////////////////////////
boolean  _List_list_literal()
{
	boolean result;
    if(_List_literald())
    {
 	    token=_lire_token();
 	    result=_List_list_literal_aux();
    }
    else 
    {
        result=false;
    }
return result;
}                      
                        ////////////////////////////////////////
boolean   _List_list_literal_aux()
{
	boolean result;
	if(token==VIRG)
    {
	   token=_lire_token();
	   if(_List_list_literal())
	   {
		   result=true;
	   }
	   else
	   {
		   result=false;
	   }
    }
    else if(token==CCLOSE)
    {
	result=true;
	//follow_token=true;
    }
return result;
}                     
                        ////////////////////////////////////////
boolean _Udt_literal()
{
	boolean result;
	if(token==AOPEN)
	{
		token=_lire_token();
		if(_List_udt_literal())
		{
			result=true;
		}
		else{result=false;}
	}
	else
	{result=false;}
return result;	
}
                        ////////////////////////////////////////
boolean _Udt_literald()
{
	boolean result;
	if(token==IDF)
    {
		token=_lire_token();
		if(token==DPNT)
		{
		    token=_lire_token();
		  	if(_Term())
		  	{
		  		result=true;
		  	}
		  	else{result=false;}
		}
		else{result=false;}
	}
    else{result=false;}
      
return result;
}
                        ///////////////////////////////////////

boolean _List_udt_literal()
{
	boolean result;
	if(_Udt_literald())
	{	
		token=_lire_token();
		result=_List_udt_literal_aux();
    }
    else
    {
    	result=false;
    }
return result;  
}
                         ///////////////////////////////////////
boolean _List_udt_literal_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_udt_literal())
		{
			result=true;
		}
		else
		{
			result=false;
		}
	}
	else if(token==ACLOSE)
	{
		result=true;
		//follow_token=true;
	}
return result;	
}                         


                         ///////////////////////////////////////
boolean _Tuple_literal()
{
	boolean result;
	if(token==POPEN)
	{
		token=_lire_token();
		if(_List_tuple_literal())
		{
			result=true;
		}
		else{result=false;} 
	}
	else{result=false;}
return result;	
}                         
                         ////////////////////////////////////////  
boolean  _Tuple_literald()
{
    boolean result;
	if(_Term())
	{
	    start= add_val_insert(start,valClm);     // la fonction qui ajoute les valeur entrer pour chaque atribut
	    //free(valClm);
	    result=true;  	
    }
    else
    {
        result=false;
    }
return result;    
}
                         ////////////////////////////////////////
boolean _List_tuple_literal()
{
	boolean result;
	if(_Tuple_literald())
	{
		token=_lire_token();
		result=_List_tuple_literal_aux();
	}
	else
	{
        result=false;
	}
return result;
}                         

                         ////////////////////////////////////////
boolean _List_tuple_literal_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_tuple_literal())
		{
			result=true;
		}
		else 
		{
			result=false;
		}
	}
	else if(token==PCLOSE)
	{
         result=true;
         //follow_token=true;
	}
return result;	
}               
                         //////////////////////////////////////
boolean _Function_call()
{
	boolean result;
	if(token==IDF)
	{
		token=_lire_token();
		if(token==POPEN)
		{   
			token=_lire_token();
			if(_List_function_call())
			{
			    result=true;
            }
            else{ result=false;}
		}
		else{ result=false;}
	}
	else{result=false;}
return result;
}                                
                         ///////////////////////////////////////
boolean  _Function_calld()
{
    boolean result;
    if(_Term())
    {
        result=true;
    }
    else{ result=false;}
return result;
}
                         ///////////////////////////////////////
boolean _List_function_call()
{
	boolean result;
	if(_Function_calld())
	{
		token=_lire_token();
		result=_List_function_call_aux();
	}
	else
	{
		result=false;
	}
return result;

}     
                         //////////////////////////////////////
boolean _List_function_call_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_function_call())
		{
			result=true;
		}
		else{ result=false;}
	}
	else if (token==PCLOSE)
	{
		 result=true;
		 //follow_token=true;
	}
	else{ result=false;}
return result;
}      

                         ///////////////////////////////////////
boolean _Bind_m()
{
	boolean result;
	if(token==PEX)
	{
		result=true;
	}
	else if(token==DPNT)
	{
		result=true;
	}
	else{result=false;}
return result;
}
                         ///////////////////////////////////////
boolean  _Bind_marker()
{
    boolean result;
    if(_Bind_m())
    {
 	    token=_lire_token();
        if(token==IDF)
        {
     	     result=true;
        }
        else{result=false;}
    }
    else{result=false;}    
return result;    
}

                         ///////////////////////////////////////
boolean  _Type_hint()
{
    boolean result;
    if(token==POPEN)
    {
 	    token=_lire_token();
        if(_Cql_type())
        {
     	    token=_lire_token();
     	    if(token==PCLOSE)
     	    {
     		    token=_lire_token();
     		    if(_Term())
     		    {
     			    result=true;
     		    }
     		    else{result=false;}
     	    }
     	    else{result=false;}
        }
        else{result=false;}
    }
    else{result=false;}
return result;
}

                     ///////////////////////////////////////
boolean  _Cql_type()
{
    boolean result;
    if(_Type())
    {
        result=true;
    }
    else if(_Collection_type())
    {
        result=true;    
    }
    else if(_User_defined_type())
    {
        result=true;
    }
    else if(_Tuple_type())
    {
        result=true;
    }
    else if(_Custom_type())
    {
        result=true;
    }
    else
    {
 	    result=false;
    }
return result;    
}
                     ///////////////////////////////////////
boolean  _Coll_map()
{
 boolean result;
    if(token==GMO)
    {
     	token=_lire_token();
        if(_Cql_type())
        {
         	token=_lire_token();
            if(token=VIRG)
            {
            	token=_lire_token();
            	if (_Cql_type())
            	{
            		token=_lire_token();
            		if(token==GMF)
            		{
            			result=true;
            		}
            		else{ result=false;}
            	}
            	else{ result=false;}
            } 
            else{ result=false;}
        }
        else{ result=false;}
    }
    else{ result=false;} 
  
return result; 
}      

         ///////////////////////////////////////
boolean  _Coll_set()
{
    boolean result;
    if(token==GMO)
    {
     	token=_lire_token();
        if(_Cql_type())
        {
        	token=_lire_token();
        	if(token==GMF)
        	{
        		result=true;
        	}
        	else{ result=false;}
        }
        else{ result=false;}
    }
    else{ result=false;}
return result;   
}                    
         ///////////////////////////////////////
 boolean  _Coll_list()
{
    boolean result;
    if(token==GMO)
	{
	 	token=_lire_token();
	    if(_Cql_type())
	    {
	    	token=_lire_token();
	    	if(token==GMF)
	    	{
	    		result=true;
	    	}
	    	else{ result=false;}
	    }
	    else{ result=false;}
	}
	else{ result=false;}
return result;   
}                    
      
                     ///////////////////////////////////////
boolean  _Collection_type()
{
    boolean result;
    if(token==LIST)
    {
    	token=_lire_token();
    	if(_Coll_list())
    	{
    		result=true;
    	}
    	else{result= false;}	  
    }
    else if(token==MAP)
    {   
    	token=_lire_token();
    	if(_Coll_map())
    	{
    		result=true;
    	}
    	else{result= false;}
        
    }
    else if(token==SET)
    {   
    	token=_lire_token();
    	if(_Coll_set())
        {
            result=true;
        }
        else{result= false;}
    }
    else{result= false;}
return result;	   
}

                         ////////////////////////////////////////
boolean  _User_defined_type()
{
	boolean result;
	if(token==IDF)//keyspace_name
	{
		token=_lire_token();
		if(token==PNT)
		{   
			token=_lire_token();
			if(token==IDF)
			{
				result=true;
			}
			else {result=false;}
		}
		else {result=false;}
	}
	else {result=false;}
return result;	     
}
                        ////////////////////////////////////////
boolean _Tuple_type()
{
    boolean result;
    if(token==TUPLE)
    {
     	token=_lire_token();
     	if(token==GMO)
     	{   token=_lire_token();
     		if(_List_tuple_type())
     		{
     			result=true;
     		}
     		else{result=false;}
     	}
     	else{result=false;}
    }
    else{result=false;}
return result;    
}                        
                        ////////////////////////////////////////
boolean _Tuple_typed()
{
	boolean result;
	if(_Cql_type())
	{
		result=true;
	}
	else{result=false;}
return result;	
}                        
                        ////////////////////////////////////////
boolean _List_tuple_type()
{
	boolean result;
	if(_Tuple_typed())
	{
        token=_lire_token();
        result=_List_tuple_type_aux();
	}
	else {result=false;}
return result;
}                      
                         ////////////////////////////////////////
boolean _List_tuple_type_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_tuple_type())
		{
			result=true;
		}
		else {result=false;}
	}
	else if (token==GMF)
	{
		result=true;
		//follow_token=true;
	}
	else {result=false;}
return result;
}    
                         ////////////////////////////////////////
boolean _Custom_type()
{
	boolean result;
	if(token==STRING)
	{
		result=true;
	}
	else{result=false;}
return result;	
}                                             
                         ////////////////////////////////////////                

