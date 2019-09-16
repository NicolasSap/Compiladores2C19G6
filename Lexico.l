%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <float.h>
#include "y.tab.h"

FILE  *yyin;
int   yylval;

void checkearLimitesConstanteString();
void checkearLimitesConstanteFloat();
void checkearLimitesConstanteInteger();

%}

%option noyywrap  
%option yylineno

DIGITO			[0-9]
LETRA			  [a-zA-Z]
ID			    {LETRA}({LETRA}|{DIGITO})*
CTE_ENT     {DIGITO}+
CTE_REAL    {DIGITO}*"."{DIGITO}+|{DIGITO}+"."{DIGITO}*|{DIGITO}+"."{DIGITO}+
CTE_STRING  \".*\"
COMMENT     "--\/".*"\/--"
BLANK       [" "]

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
"{"				{ return L_A;} 
"}"				{ return L_C;} 
">"       { return OP_MAX;}
">="      { return OP_MAXEQ;}
"<"       { return OP_MIN;}
"<="      { return OP_MINEQ;}
"!="      { return OP_NEQ;}
"=="      { return OP_EQ;}
"="       { return EQ;}
";"       { return PYC;}
"."       { return PTO;}
":"       { return DPTO;}
","				{ return COMA;}
"&&"			{ return AND;}
"||"			{ return OR;}
"!"				{ return NOT;}
"IF"			{ return IF;}
"ELSE"		{ return ELSE;}
"ENDIF"		{ return ENDIF;}
"REPEAT"	{ return REPEAT;}
"UNTIL"		{ return UNTIL;}
"VAR"			{ return VAR;}
"ENDVAR"	{ return ENDVAR;}
"INT"		  { return INT;}
"FLOAT" 	{ return FLOAT;}
"STRING"	{ return STRING;}
"PRINT"		{ return PRINT;}
"READ"		{ return READ;}
"CONST"		{ return CONST;}
{ID}			{ insert(yytext); return ID;}
{COMMENT}             
{CTE_STRING}	{ checkearLimitesConstanteString(); insert(yytext); return CTE_STRING;}
{CTE_ENT}		{ checkearLimitesConstanteInteger(); insert(yytext); return CTE_ENT;}
{CTE_REAL}		{ checkearLimitesConstanteFloat(); insert(yytext); return CTE_REAL;}
"BLANK"
"\n"
"\t"
"\r\n"

%%

void checkearLimitesConstanteString() {
  if (yyleng > 32) {
    printf("Valor maximo excedido en linea %d\n", yylineno);
    exit(1);
  }
}

void checkearLimitesConstanteFloat() {
  if (strcmp(yytext,"0") != 0){
    if (atof(yytext) > FLT_MAX || yyleng > 39) {
        printf("Valor maximo excedido en linea %d\n", yylineno);
        exit(1);
    }
  }
}

void checkearLimitesConstanteInteger() {
  if (atoi(yytext) > 65535) {
    printf("Valor maximo excedido en linea %d\n", yylineno);
    exit(1);
  }
}