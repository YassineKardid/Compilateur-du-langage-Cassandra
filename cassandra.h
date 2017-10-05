#ifndef CASSANDRA_H
#define CASSANDRA_H

typedef enum {
             IDF,INUMBER, DNUMBER, INT, DOUBLE, BOOL, TRUE, FALSE, PVIRG, EQ, 
             POPEN, PCLOSE,PLUS, MINUS, MULT, DIV,VARINT,TEXT,CREATE,ALTER,SELECT,KEYSPACE,
             FROM,DESCRIBE,TABLES,TABLE,AN_D,PRIMARY,KEY,PRIMARYKEY,WITH,TYPE,REPLICATION,USE,
             AOPEN,ACLOSE,DROP,COLUMN,APS,GMO,GMF,VIRG,NAME,OUL,NEW,INSERT,INTO,DELETE,
             STRING,OR,REPLACE,MAP,WHERE,COUNT,IF,NOT,EXISTS,PEX,ADD,DATE,DPNT,
             SET,LIST,COPEN,CCLOSE,PNT,TUPLE,AND,STR,VALUES,USING,JSON,DEFAULT,
             NUL_L,UNSET,TTL,TIMESTAMP,EX,STATIC,/*  */AS,CAST,TOKEN,IN,CONTAINS,
             ASC,DESC,DISTINCT,GROUP,BY,ORDER,CLUSTERING,COMPACT,STORAGE,TRUNCATE,ASCII,BIGINT,
             BLOB,COUNTER,DECIMAL,INET,BOOLEAN,FLOAT,TIME,TIMEUUID,TINYINT,VARCHAR,UUID
            } typetoken;


typedef enum {
 false=0,
 true=1
} boolean;

  //boolean follow_token = false;
  typetoken token;
  
  /*boolean _Table(); 
  boolean _Column_definition() ;
  boolean _List_column_definition();
  boolean _List_column_definition_aux(); 
  boolean _Primary_key();
  boolean  _Partition_key_t();
  boolean _List_column_name();
  boolean _List_column_name_aux();
  boolean _Clustering_columns();
  boolean _Table_options();
  boolean _If_not_exist();*/

  boolean _NOT_EXISTS();
  boolean _TABLE();
  boolean _PRIMARY_KEY_2();
  boolean _PARTITION_KEY();
  boolean _PARTITION_KEY_AUX();
  boolean _PARTITION_KEY_AUX_AUX();
  boolean _CLUSTERING_COLUMNS_AUX();
  boolean _CLUSTERING_COLUMNS();
  boolean _COLUMN_DEFINITION_AUX();
  boolean _COLUMN_DEFINITION();
  boolean _COLUMN_DEFINITION_ALTER();
  boolean _PRIMARY_KEY();
  boolean _CQL_TYPE();
  boolean _NATIVE_TYPE();
  boolean _CUSTOM_TYPE();
  boolean _TUPLE_TYPE();
  boolean _CQL_TYPE_AUX();
  boolean _USER_DEFINED_TYPE();
  boolean _COLLECTION_TYPE();
  boolean _TABLE_OPTIONS();
  boolean _OPTIONS();
  boolean _CLUSTERING_ORDER();
  boolean _CLUSTERING_ORDER_AUX();
  boolean _ALTER_TABLE();
  boolean _ALTER_TABLE_INSTRUCTIONS();

  boolean _COLUMN_DEFINITION_AUX_ALTER();
  boolean _IDF_AUX();
  boolean _IF_EXISTS();
  boolean _TRUNCATE();
  boolean _TRUNCATE_AUX();
  boolean _TABLE_NAME();
  boolean _DROP_TABLE();
  


  boolean _Simple_selection();
  boolean _Condition();
  boolean _Delete();
  boolean _List_condition();
  boolean _List_condition_aux();
  boolean _List_simple_selection();
  boolean _List_simple_selection_aux();
  boolean _List_select_clause();
  boolean _List_select_clause_aux();

  boolean Where();
  boolean Group();
  boolean Order();
  boolean _Select();
  boolean  _Select_aux();
  boolean _Select_clause();
  boolean _Selector();
  boolean _Selector_function_aux();
  boolean _Where_clause();
  boolean _Where_clause_aux();
  boolean _Relation();
  boolean _Operator();
  boolean _Group_by_clause();
  boolean _Group_by_clause_aux();
  boolean _Ordering_clause();
  boolean _Ordering_clause_aux();

     
   //les prototypes  des fonctions utulisées
  boolean _Insert();
  boolean _Using();
  boolean _List_update_parametre();
  boolean _List_update_parametre_aux();
  boolean _Update_parametre();
  boolean _Json_clause();
  boolean _Names_values();
  boolean _Names();
  boolean _List_names();
  boolean _Namesd();
  boolean _List_names_aux();


  boolean _Drop();
  boolean _Alter();
  boolean _Partition_key(); 
  boolean _List_partition_key();
  boolean _Partition_keyd();
  boolean _List_partition_key_aux();

  typetoken _lire_token();
  boolean _Use();     //use a keyspace
  boolean _Type();
  boolean _Create();  //create
  boolean _Const();
  boolean _Options();
  boolean _List_option();
  boolean _List_option_aux();
  boolean _Option();
  boolean _Option_aux();
  boolean _Term();
  boolean _Literal();
  boolean _Collection_literal();
  boolean _Map_literal();
  boolean _Map_literald();
  boolean _List_map_literal();
  boolean _List_map_literal_aux();
  boolean _Set_literal();
  boolean _Set_literald();
  boolean _List_set_literal();
  boolean _List_set_literal_aux();
  boolean _List_literal();
  boolean _List_literald();
  boolean  _List_list_literal();
  boolean  _List_list_literal_aux();
  boolean _Udt_literal();          /////?
  boolean _Udt_literald();
  boolean _List_udt_literal();
  boolean _List_udt_literal_aux();
  boolean _Tuple_literal();
  boolean  _Tuple_literald();
  boolean _List_tuple_literal();
  boolean _List_tuple_literal_aux();
  boolean _Function_call();
  boolean  _Function_calld();
  boolean _List_function_call();
  boolean _List_function_call_aux();
  boolean _Bind_m();
  boolean  _Bind_marker();
  boolean  _Type_hint();
  boolean  _Cql_type();
  boolean  _Coll_map();
  boolean  _Coll_set();
  boolean  _Coll_list();
  boolean  _Collection_type();
  boolean _Collection_literal();
  boolean  _User_defined_type();
  boolean _Tuple_type();
  boolean _Tuple_typed();
  boolean _List_tuple_type();
  boolean _List_tuple_type_aux();
  boolean _Custom_type();

#endif





































