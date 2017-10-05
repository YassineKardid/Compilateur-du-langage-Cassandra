#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cassandra.h"
#include "error.h"
#include "tabsymb.h"

extern columnData *valInsert;
extern column *ValAddClm;

extern char *yytext;
extern int yylineno;

extern boolean follow_token;
extern table *tete;
extern ClmInsert * begin;
extern ClmSelect *began;
extern values *start;
extern values *valClm;
extern char *tabInsert;
char *tabSelect;
char *ClmCondi;
int mult;
extern char * keysCree;
extern int nbTab;
extern int iniErr;

/////////delete/////////////
boolean _Simple_selection()
{
	boolean result;
	if(token==IDF)
	{
		result=true;
	}
	else{result=false;}
return result;	
}
           //////////////////////////////////
boolean _List_simple_selection()
{
	boolean result;
	if(_Simple_selection())
	{
		token=_lire_token();
		result=_List_simple_selection_aux();
	}
	else {result=false;}
return result;	
}
          ////////////////////////////////////
boolean _List_simple_selection_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_simple_selection())
		{
			result=true;
		}
		else {result=false;}
	}
	else if(token==FROM)
	{
		follow_token=true;
		result=true;
	}
	else {result=false;}
return result;	
}          

           ///////////////////////////////////
boolean _Condition()
{
	boolean result;
	if(_Simple_selection())
	{
		token=_lire_token();
		if(_Operator())
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


boolean _List_condition()
{
	boolean result;
	if(_Condition())
	{
		token=_lire_token();
		result=_List_condition_aux();
	}
	else{result=false;}
return result;
}
           //////////////////////////////////////////
boolean _List_condition_aux()
{
	boolean result;
	if (token==AND)
	{
		if(_List_condition())
		{
			result=true;
		}
		else{result=false;}
	}
	else if(token==PVIRG)
	{
		follow_token=true;
		result=true;
	}
	else{result=false;}
return result;
}           
            /////////////////////////////////////////
boolean _Select()
{

	boolean result;
	if(_Select_aux())
	{

		token=_lire_token();
		if (Where())
		{
			token=_lire_token();
			if(Group())
			{
				token=_lire_token();
				if (Order())
				{
					token=_lire_token();
					if(token==PVIRG)
					{
						result=true;
					}
					else {result=false;}	
				}
				else if(token==PVIRG)
				{
					result=true;
				}
				else {result=false;}	
			}
			else if (Order())
			{
				token=_lire_token();
				if(token==PVIRG)
				{
					result=true;
				}
				else {result=false;}	
			}
			else if(token==PVIRG)
			{
				//printf("%s vaaaaaclmmmmm\n",valClm->data);
				test_select(tete,tabSelect,began);
				if(iniErr== 0)
				{


					if(mult==1)
					{
	                    afficher_table_data(tete);
	                    mult=0;
					}
					else
					{
						afficher_element(began,tete,start,tabSelect,ClmCondi);
					}
			    }
				
				result=true;
			}
			else {result=false;}	

		}
		else if(Group())
		{
			token=_lire_token();
			if (Order())
			{
				token=_lire_token();
				if(token==PVIRG)
				{
					result=true;
				}
				else {result=false;}	
			}
			else if(token==PVIRG)
			{
				result=true;
			}
			else {result=false;}	
		}
		else if (Order())
		{
			token=_lire_token();
			if(token==PVIRG)
			{
				result=true;
			}
			else {result=false;}	
		}
		else if(token==PVIRG)
		{
			//printf("%s vaaaaaclmmmmm\n",valClm->data);
				test_select(tete,tabSelect,began);
				if(iniErr== 0)
				{


					if(mult==1)
					{
	                    afficher_table_data(tete);
	                    mult=0;
					}
					else
					{
						afficher_element(began,tete,start,tabSelect,ClmCondi);
					}
			    }
			result=true;
		}
		else {result=false;}			
				
	}
	else {result=false;}
return result;		
}
                           /////////////////////////////////////////
boolean Where()
{
	boolean result;
	if(token==WHERE)
	{
		token=_lire_token();
		if(_Where_clause())
		{
			result=true;
		}
		else {result=false;}
	}
	else {result=false;}
return result;
}  

                           ///////////////////////////////////////////
boolean _Where_clause()
{
 	boolean result;
 	if(_Relation())
 	{
 		token=_lire_token();
 		result=_Where_clause_aux();
 	}
 	else{result=false;}

return result;
}
                            ////////////////////////////////////////
boolean _Where_clause_aux()
{
	boolean result;
	if(token==AND)
	{
        token=_lire_token();
		if(_Where_clause())
		{
			result=true;
		}
		else{result=false;}
	}
	else if(token==PVIRG||token==ORDER||token==GROUP||token==IF)
    {
    	follow_token=true;
    	result=true;
    }
    else{result=false;}
return result;    
}                            
                           /////////////////////////////////////////
boolean _Relation()
{ 
	
    boolean result;
    if(token==IDF)                     ///column condition ,?????????? ex 15
    {
    	  //free(ClmCondi);
		ClmCondi=(char *)malloc(strlen(yytext)+1);    //--??
		strcpy(ClmCondi,yytext);
    	token=_lire_token();
    	if(_Operator())
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
    else if(_Names()) 
	{    
		token=_lire_token();
        if(_Operator())
        {
         	token=_lire_token();
         	if(_Tuple_literal())
         	{
         		result=true;
         	}
         	else {result=false;}
        } 	
        else{result=false;}
    }
    else if(token==TOKEN)   
	{
	    token=_lire_token();
	    if(_Names())
	    {
	    	token=_lire_token();
	    	if(_Operator())
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



                            /////////////////////////////////////////
boolean _Operator()
{
	boolean result;
	if(token==EQ)
	{
		result=true;
	}
	else if(token==EX)
	{
		token=_lire_token();
		if (token==EQ)
		{
			result=true;
		}
	}
	else if(token==IN)
	{
		result=true;
	}
	else if(token==CONTAINS)
	{
		token=_lire_token();
		if(token==KEY)
        {
            result=true; 
        } 
        else
        {
        	follow_token=true;
        	result=true;
        } 
	}
	else if(token==GMO)
	{
		token=_lire_token();
		if(token==EQ)
        {
            result=true; 
        } 
        else
        {
        	follow_token=true;
        	result=true;
        }
    }    
	else if(token==GMF)
	{
		token=_lire_token();
		if(token==EQ)
        {
            result=true; 
        }
        else
        {
        	follow_token=true;
        	result=true;
        }
	}
	else{result=false;}
return result;	
}
                            ////////////////////////////////////////
boolean Group()
{
	boolean result;
	if(token==GROUP)
	{
		token=_lire_token();
		if(token==BY)
		{
			token=_lire_token();
			if(_Group_by_clause())
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

                            /////////////////////////////////////////
boolean Order()
{
	boolean result;
	if(token==ORDER)
	{
		token=_lire_token();
		if(token==BY)
		{
			token=_lire_token();
			if(_Ordering_clause())
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

                            /////////////////////////////////////////
boolean  _Select_aux()
{
	boolean result;
	if(token==SELECT)
	{
		token=_lire_token();
		if(token==JSON||token==DISTINCT)
		{
			token=_lire_token();
			if(_List_select_clause()||token==MULT)
			{
				if(token==MULT)
				{
					mult=1;                  ///////)))))))))))))))))))))))))))
				}
				token=_lire_token();
				if(token==FROM)
				{
					token=_lire_token();
					if(token==IDF)
					{
						if(in_Tab_tabl(tete,yytext)==false)
		                {
		                	creer_sm_erreur_declaration(tableNotExist,yylineno, yytext);
		                } 
						free(tabSelect);
						tabSelect=(char *)malloc(strlen(yytext)+1);    //--??
		                strcpy(tabSelect,yytext);						
						token=_lire_token();
						if(token==WHERE||token==GROUP||token==ORDER||token==ORDER||token==PVIRG)

						{
							follow_token=true;
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
		else if(_List_select_clause()||token==MULT)
		{
			if(token==MULT)
				{
					mult=1;                  ///////)))))))))))))))))))))))))))
				}
			token=_lire_token();
			if(token==FROM)
			{
				token=_lire_token();
				if(token==IDF)
				{
					if(in_Tab_tabl(tete,yytext)==false)
	                {
	                	creer_sm_erreur_declaration(tableNotExist,yylineno, yytext);
	                } 
	                //free(tabSelect);
					tabSelect=(char *)malloc(strlen(yytext)+1);    //--??
	                strcpy(tabSelect,yytext);	
					token=_lire_token();
					if(token==WHERE||token==GROUP||token==ORDER||token==ORDER||token==PVIRG)

					{
						follow_token=true;
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
                            /////////////////////////////////////////
boolean _Select_clause()
{
	boolean result;
	if(_Selector())
	{
		token=_lire_token();
        if(token==AS)
        {   
        	token=_lire_token();
        	if(token==IDF)
        	{
        		result=true;
        	}
        	else{result=false;}
        }
        else if(token==VIRG||token==FROM)
        {
        	follow_token=true;
        	result=true;
        }
		else{result=false;}
	}
    else{result=false;}
return result;
} 
                           //////////////////////////////////////////
boolean _List_select_clause()
{
	boolean result;
	if(_Select_clause())
	{
		token=_lire_token();
		result=_List_select_clause_aux();
	}
	else{result=false;}
return result;	
}             
                           //////////////////////////////////////////
boolean _List_select_clause_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
        if(_List_select_clause())
        {
        	result=true;
        }
        else{result=false;}

	}
	else if(token==FROM)
	{
		follow_token=true;
		result=true;
	}
	else{result=false;}
return result;
}                                         
                           //////////////////////////////////////////
boolean _Selector()
{
	boolean result;
	if(token==IDF)               ////les columns a selectionnner ,,, inserer dans *ClmSelect
	{ 
		began= add_clm_select(began,yytext); 
	    token=_lire_token();
		if(token==POPEN)
		{
			token=_lire_token();
			if(token==PCLOSE)
			{
				result=true;
			}
			else if(_Selector())
			{
				result=_Selector_function_aux();
			}
			else{result=false;}
		} 
		else if(token==AS||token==VIRG||token==PCLOSE||token==FROM)
		{
			follow_token=true;
			result=true;
		}
		else{result=false;}
    }
	else if(token==CAST)
	{
		token=_lire_token();
		if(token==POPEN)
		{
			token=_lire_token();
			if(_Selector())
			{
				token=_lire_token();
				if(token==AS)
                {
                	token=_lire_token();
                	if(_Cql_type())
                	{
                		token=_lire_token();
                		if(token==PCLOSE)
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
	}
	else if(token==COUNT)
	{
        token=_lire_token();
        if (token==POPEN)
        {
        	token=_lire_token();
        	if(token==MULT)
        	{
        		token=_lire_token();
        		if(token==PCLOSE)
        		{
        			result=true;
        		}
        		else{result=false;}
        	}
        	else{result=false;}
        }
        else{result=false;}
	}
	else if(_Term())
	{
		result=true;
	}
	else{result=false;}
return result;		
}                                                      
                            /////////////////////////////////////////
boolean _Selector_function_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_Selector())
		{
			result=true;
		}
		else{result=false;}
	}	
	else if (token==PCLOSE)
	{
		result=true;
	}
	else{result=false;}
return result;	
}

                            /////////////////////////////////////////
boolean _Group_by_clause()
{
	boolean result;
    if(token==IDF)
    {
    	token=_lire_token();
    	result=_Group_by_clause_aux();
    }
    else{result=false;}
return result;    	
}  
                           //////////////////////////////////////////
boolean _Group_by_clause_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_Group_by_clause())
		{
			result=true;
		}
		else{result=false;}
	}
	else if(token==ORDER||token==PVIRG)
    {
    	follow_token=true;
    	result=true;
    }
    else{ result=false;}
return result;    
}

                            /////////////////////////////////////////
boolean _Ordering_clause()
{
	boolean result;
	if(token==IDF)
    {
    	token=_lire_token();
		if(token==ASC||token==DESC)
        {
		   token=_lire_token();
		   result=_Ordering_clause_aux();
	    }
	    else{result=_Ordering_clause_aux();}
    }
	else{result=false;}
return result;	
}
                            //////////////////////////////////////////
boolean _Ordering_clause_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_Ordering_clause())
		{
			result=true;
		}
		else{result=true;}
	}
	else if (token==PVIRG)
	{
		follow_token=true;
		result=true;
	}
return result;	
}   

                             

                          //////////////////////////////////////
boolean _Partition_key()
{
	boolean result;
	if(token==IDF)
	{
		result=true;
	}
	else if(token==POPEN)
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
boolean _List_partition_key()
{
	boolean result;
	if(_Partition_keyd())
	{
		token=_lire_token();
		result=_List_partition_key_aux();
	    //follow_token=true;
	}
	else{result=false;}
return result;
}
                         ////////////////////////////////////
boolean _Partition_keyd()
{
	boolean result;
	if(token==IDF)
	{
		if(in_list_clm(tete,tabInsert,yytext)==false)
		{
          creer_sm_erreur_declaration(columnNotExist,yylineno,yytext);
        }
		begin= add_clm_insert(begin,yytext);    //l'ajout des columns dont ont veut inserer
		result=true;
	}
	else{ result=false;}
return result;	
} 
                        ///////////////////////////////////
boolean _List_partition_key_aux()
{
	boolean result;
	if(token==VIRG)
	{
		token=_lire_token();
		if(_List_partition_key())
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
}  
               ///////////////////////////////////
  boolean _Delete()
{
	boolean result;
	if(token==DELETE)
	{	
		token=_lire_token();
		if(_List_simple_selection())
		{
			token=_lire_token();
			if(token==FROM)
			{ 
				token=_lire_token();
				if(token==IDF)
				{
                    if(in_Tab_tabl(tete,yytext)==false)
	                {
	                	creer_sm_erreur_declaration(tableNotExist,yylineno, yytext);
	                } 
					token=_lire_token();
					if(_Using())
					{						
						token=_lire_token();
						if(Where())
						{							
							token=_lire_token();
							if(token==IF)
							{
								token=_lire_token();
								if(token==EXISTS)
								{
									token=_lire_token();
									if(token==PVIRG)
									{
										result=true;
									}
									else{result=false;}
								}
								else if(_List_condition()) //--liste condition
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
							else if(token==PVIRG)
							{
								result=true;
							}
							else{result=false;}
						}
						else{result=false;}
					}
					else if(Where())
					{
						token=_lire_token();
						if(token==IF)
						{
							token=_lire_token();
							if(token==EXISTS)
							{
								token=_lire_token();
								if(token==PVIRG)
								{
									result=true;
								}
								else{result=false;}
							}
							else if(_List_condition()) //--liste condition
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
						else if(token==PVIRG)
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
		else if(token==FROM)
		{ 
			token=_lire_token();
			if(token==IDF)
			{
				if(in_Tab_tabl(tete,yytext)==false)
                {
                	creer_sm_erreur_declaration(tableNotExist,yylineno, yytext);
                } 
				token=_lire_token();
				if(_Using())
				{
					token=_lire_token();
					if(Where())
					{
						token=_lire_token();
						if(token==IF)
						{
							token=_lire_token();
							if(token==EXISTS)
							{
								token=_lire_token();
								if(token==PVIRG)
								{
									result=true;
								}
								else{result=false;}
							}
							else if(_List_condition()) //--liste condition
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
						else if(token==PVIRG)
						{
							result=true;
						}
						else{result=false;}
					}
					else{result=false;}
				}
				else if(Where())
				{
					token=_lire_token();
					if(token==IF)
					{
						token=_lire_token();
						if(token==EXISTS)
						{
							token=_lire_token();
							if(token==PVIRG)
							{
								result=true;
							}
							else{result=false;}
						}
						else if(_List_condition()) //--liste condition
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
					else if(token==PVIRG)
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
           ///////////////////////////////////////////////                                                             