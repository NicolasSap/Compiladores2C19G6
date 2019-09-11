%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "y.tab.h"
FILE  *yyin;
int yylval;
%}

%option noyywrap  
%option yylineno

DIGITO			[0-9]
LETRA			[a-zA-Z]
CHAR_ESP        [" "|"!"|"""|"#"|"$"|"%"|"&"|"'"|"("|")"|"*"|"+"|"-"|","|"."|"/"|":"|";"|"<"|">"|"="|"?"|"@"|"["|"]"|"{"|"}"|"\"|"^"|"_"|"`"|"~"]
ID			    {LETRA}({LETRA}|{DIGITO}|_)*
CTE_STING       "({LETRA}|{DIGITO}|{CHAR_ESP})*"
CTE_ENT         {DIGITO}+
CTE_REAL        {DIGITO}*"."{DIGITO}+|{DIGITO}+"."{DIGITO}*|{DIGITO}+"."{DIGITO}+
COMMENT         "--/"({LETRA}|{DIGITO}|{CHAR_ESP})*"/--"|"--/"({LETRA}|{DIGITO}|{CHAR_ESP})*"--/"({LETRA}|{DIGITO}|{CHAR_ESP})*"/--"({LETRA}|{DIGITO}|{CHAR_ESP})*"/--"

%%
":="			{ return OP_ASIG;}
"+"				{ return OP_SUMA;}
"-"				{ return OP_RESTA;}
"*"				{ return OP_MULT;}
"/"				{ return OP_DIV;}
"("				{ return P_A;}
")"				{ return P_C;}
"["				{ return C_A;}
"]"				{ return C_C;}
"{"				{ return L_A} 
"}"				{ return L_C} 
">"             { return OP_MAX;}
">="            { return OP_MAXEQ;}
"<"             { return OP_MIN;}
"<="            { return OP_MINEQ;}
"!="            { return OP_NEQ;}
"=="            { return OP_EQ;}
";"             { return PYC;}
"."             { return PTO;}
","				{ return COMA;}
"&&"			{ return AND;}
"||"			{ return OR;}
"!"				{ return NOT;}
"IF"			{ return IF;}
"ELSE"			{ return ELSE;}
"ENDIF"			{ return ENDIF;}
"REPEAT"		{ return REPEAT;}
"UNTIL"			{ return UNTIL;}
"VAR"			{ return VAR;}
"ENDVAR"		{ return ENDVAR;}
"INTEGER"		{ return INT;}
"FLOAT"			{ return FLOAT;}
"STRING"		{ return STRING;}
"PRINT"			{ return PRINT;}
"READ"			{ return READ;}
"CONST"			{ return CONST;}
"BLANK"			{ return BLANK;}
{ID}			{ return ID;}
{CTE_STING}		{ return CTE_STING;}
{CTE_ENT}		{ return CTE_ENT;}
{CTE_REAL}		{ return CTE_REAL;}
%%