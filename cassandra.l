
%{
#include "cassandra.h"
%}
%option noyywrap
%option yylineno
%%

"("			    {return POPEN;}
"static"        {return STATIC;}
"as"            {return AS;}
"token"         {return TOKEN;}
"in"            {return IN;} 
"contains"      {return CONTAINS;}
"asc"           {return ASC;}
"desc"          {return DESC;}
"distinct"      {return DISTINCT;}
"group"	        {return GROUP;}
"by"            {return BY;}
"order"         {return ORDER;}
"cast"          {return CAST;}


"ttl"           {return TTL;}
"timestamp"     {return TIMESTAMP;}
"unset"         {return UNSET;}
"null"          {return NUL_L;}
"default"       {return DEFAULT;}
"json"          {return JSON;}
"using"         {return USING;}
"values"        {return VALUES;}
"create" 		{return CREATE;}

"int" 			{return INT;}
"varint" 		{return VARINT;}
"string"        {return STRING;}
"text" 			{return TEXT;}
"bool" 			{return BOOL;}
"double" 		{return DOUBLE;}

"alter" 		{return ALTER;}
"select" 		{return SELECT;}
"keyspace" 		{return KEYSPACE;}
"from" 			{return FROM;}
"describe" 		{return DESCRIBE;}
"tables" 		{return TABLES;}
"table"         {return TABLE;}
"and" 			{return AND;}
"primary" 		{return PRIMARY;}
"key" 			{return KEY;}
"primarykey"    {return PRIMARYKEY;}
"with" 			{return WITH;}
"type" 			{return TYPE;}
"use" 			{return USE;}
"drop" 			{return DROP;}
"comlumn"       {return COLUMN;}
"name"          {return NAME;}
"add"           {return ADD;}
"new"           {return NEW;}
"insert"        {return INSERT;}
"into"          {return INTO;}
"delete"        {return DELETE;}
"or"            {return OR;}
"replace"       {return REPLACE;}
"map"           {return MAP;}
"where"         {return WHERE;}
"count"         {return COUNT;}
"if"            {return IF;}
"not"           {return NOT;}
"exists"        {return EXISTS;}

"true" 			{return TRUE;}
"false" 		{return FALSE;}
"date"          {return DATE;}
"set"           {return SET;}
"list"          {return LIST;}
"tuple"         {return TUPLE;}
"'"             {return APS;}
"<"             {return GMO;}
">"             {return GMF;}
","             {return VIRG;}

")"			    {return PCLOSE;}
"{"				{return AOPEN;}
"}"				{return ACLOSE;}
";" 			{return PVIRG;}
":"             {return DPNT;}
"."             {return PNT;}
"["             {return COPEN;}
"]"             {return CCLOSE;}
"|"             {return OUL;}
"/"			    {return DIV;}
"?"             {return PEX;}
"!"             {return EX;}        //ajouté


"ascii"			{return ASCII;}
"bigint"		{return BIGINT;}
"blob"			{return BLOB;}
"boolean"		{return BOOLEAN;}
"counter"		{return COUNTER;}
"decimal"		{return DECIMAL;}
"float"			{return FLOAT;}
"inet"			{return INET;}
"time"			{return TIME;}
"timeuuid"		{return TIMEUUID;}
"tinyint"		{return TINYINT;}
"uuid"			{return UUID;}
"varchar"		{return VARCHAR;}
"clustering"	{return CLUSTERING;}
"compact"		{return COMPACT;}
"storage"		{return STORAGE;}
"truncate"		{return TRUNCATE;}




"\'"[^"\'"]*"\'"	{return STR;}
"\""[a-zA-Z_][a-zA-Z0-9_]*"\""|[a-zA-Z_][a-zA-Z0-9_]* {return IDF;}
[0-9]+[.][0-9]+		{return DNUMBER;}
[0-9]+		{return INUMBER;}

"="			{return EQ;}
"+"			{return PLUS;}
"-"			{return MINUS;}
"*"			{ return MULT;}

[--][^\n]*   /* commentaires */
[//][^\n]*   /* commentaires */
.			
%%
