#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra.h"
#include "error.h"
#include "tabsymb.h"

extern char *yytext;
extern int yylineno;

extern boolean follow_token;
extern int iniErr;
table *tete;
ClmInsert * begin;
ClmSelect *began;
char *tabInsert;
columnData *valInsert;
column *valAddClm;
values *valClm;
values *start;
char * keysCree;
int nbTab;

boolean _Insert()
{
	boolean result;
	if (token==INSERT)	
	{
		token=_lire_token();
		if(token==INTO)
		{
			token=_lire_token();
			if(token==IDF)
			{
				if(in_Tab_tabl(tete,yytext)==false)
                {
                	creer_sm_erreur_declaration(tableNotExist,yylineno, yytext);
                } 
                //free(tabInsert);
				tabInsert=(char *)malloc(strlen(yytext)+1);    //--??
                strcpy(tabInsert,yytext);
				token=_lire_token();

				if(token==PNT)
				{
					token=_lire_token();
                    if (token==IDF)
                    {   
                    	token=_lire_token();
                    	if(_Json_clause()||_Names_values())
                    	{
                    		token=_lire_token();
                    	    if(token==IF)
							{
								token=_lire_token();
								if(token==NOT)
								{
									token=_lire_token();
									if(token==EXISTS)
									{
										token=_lire_token();
										if(token==PVIRG||_Using())
										{
											//table=add_clm_data_table(tete,column *debut,tabInsert,char*clmName,columnData *clm);
											if (iniErr== 0)
            								{
											insert_veri(tete ,start,begin,tabInsert);
											}
											result=true;
										}
										else{ result=false;}
									}
									else{ result=false;}
								}
								else{ result=false;}
							}
							else if(token==PVIRG||_Using())
							{ 

								if (iniErr== 0)
								{
								insert_veri(tete ,start,begin,tabInsert);
								}
								result=true;
							} 
							else{ result=false;}  
                    	}
                    	else if(token==IF)
						{
							token=_lire_token();
							if(token==NOT)
							{
								token=_lire_token();
								if(token==EXISTS)
								{
									token=_lire_token();
									if(token==PVIRG||_Using())
									{
										if (iniErr == 0)
        								{
										insert_veri(tete ,start,begin,tabInsert);
										}
										
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
				}
				else if(_Json_clause()||_Names_values())
				{
					token=_lire_token();
					if(token==PVIRG||_Using())
					{
						if (iniErr== 0)
						{
						insert_veri(tete ,start,begin,tabInsert);
						}
						result=true;
					}
					else if(token==IF)
					{
						token=_lire_token();
						if(token==NOT)
						{
							token=_lire_token();
							if(token==EXISTS)
							{
								token=_lire_token();
								if(token==PVIRG||_Using())
								{
									
									if (iniErr== 0)
    								{
									insert_veri(tete ,start,begin,tabInsert);
									}
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
			}
			else{ result=false;}
		}
		else{ result=false;}
	}
	else{ result=false;}
return result;	
}
             ////////////////////////////////////////////
boolean _Using()
{
	boolean result;
	if(token==USING)
	{   
		token=_lire_token();	
        if(_List_update_parametre())
        {
        	result=true;
        }
        else{ result=false;}       	 
	}
	else{ result=false;}
		
return result;
}
            /////////////////////////////////////////////
boolean _List_update_parametre()
{
	boolean result;
	if(_Update_parametre())
	{
        token=_lire_token();
        result=_List_update_parametre_aux();  
	}
	else {result=false;}
return result;	
}    
             ///////////////////////////////////////////
boolean _List_update_parametre_aux()
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
		if(_List_update_parametre())
		{
			result=true;
		}
		else {result=false;}
	}
	else {result=false;}
return result;	
}                     
             ////////////////////////////////////////////
boolean _Update_parametre()
{
	boolean result;
	if(token==TIMESTAMP||token==TTL)
	{
		token=_lire_token();
		if(token==INUMBER||_Bind_marker())
		{
			result=true;
		}
		else{ result=false;}
	}
	else{ result=false;} 
return result;	      
}             
             ////////////////////////////////////////////
boolean _Json_clause()
{
	boolean result;
	if(token==JSON)
	{
		token=_lire_token();
		if(token==STR)
		{
			token=_lire_token();
			if(token==IF||token==USING||token==PVIRG)
			{
				follow_token=true;           //////////////--------
				result=true;
			}
			else if(token==DEFAULT)
			{
				token=_lire_token();
				if(token==NUL_L||token==UNSET)
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
return result;	
}             
             ////////////////////////////////////////////
 boolean _Names_values()
 {
 	boolean result;
 	if(_Names())
 	{
 		token=_lire_token();
 		if(token==VALUES)
 		{
 			token=_lire_token();
 			if(_Tuple_literal())
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
  
                  /////////////////////////////////////
 boolean _Names()
{
	boolean result;
	
	if(token==POPEN)
	{
		token=_lire_token();
		if(_List_partition_key())
		{
			result=true;
		}
		else{ result=false;}
			
	}
	else{result=false;}
return result;	
}                                      
                           //////////////////////////////////////
boolean _List_names()
{
	boolean result;
	if(_Namesd())
	{
		token=_lire_token();
		result=_List_names_aux();
	    //follow_token=true;
	}
	else{result=false;}
return result;
}
                         ////////////////////////////////////
boolean _Namesd()
{
	boolean result;
	if(token==IDF)
	{
		result=true;
	}
	else{ result=false;}
return result;	
} 
                        ///////////////////////////////////
boolean _List_names_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_names())
		{            
            result=true;
		}
		else{result=false;}
	}
	else if(token==PCLOSE)
	{
		result=true;
	}
    else {result=false;}
return result;    	
}                    /////////////////////////////////////                                 
                     //////////////////////////////////////

