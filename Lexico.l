%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <float.h>
#include <string.h>
#include "y.tab.h"

FILE  *yyin;

void checkearLimitesConstanteString();
void checkearLimitesConstanteFloat();
void checkearLimitesConstanteInteger();
void validarComentarios();

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
"INT"		  { strcpy(yylval.string_value, yytext); return INT;}
"FLOAT" 	{ strcpy(yylval.string_value, yytext); return FLOAT;}
"STRING"	{ strcpy(yylval.string_value, yytext); return STRING;}
"PRINT"		{ return PRINT;}
"READ"		{ return READ;}
"CONST"		{ return CONST;}
{ID}			{ strcpy(yylval.string_value, yytext); insert(yytext); return ID;}
{COMMENT} { validarComentarios();}
{CTE_STRING}	{ checkearLimitesConstanteString();   strcpy(yylval.string_value, yytext);  insert(yytext); return CTE_STRING;}
{CTE_ENT}		  { checkearLimitesConstanteInteger();  yylval.integer_value = atoi(yytext);  insert(yytext); return CTE_ENT;}
{CTE_REAL}		{ checkearLimitesConstanteFloat();    yylval.float_value = atof(yytext);    insert(yytext); return CTE_REAL;}
"BLANK"
"\n"
"\t"
"\r\n"

%%


void validarComentarios() {
  char * cadena = yytext;
  cadena = cadena + 3;
  int comentarioApertura = 0;
  int i;
  
  for(i=0;  i < yyleng - 3; i ++){
    if(*cadena == '-' && *(cadena+1) == '-' && *(cadena+2) == '/'){
      if(comentarioApertura == 1){
        printf("Syntax Error: Comentarios multinivel no permitidos %d\n", yylineno);
        exit(1);
      }
      comentarioApertura = 1;
    }

    if(*cadena == '/' && *(cadena+1) == '-' && *(cadena+2) == '-'){
      comentarioApertura = 0;
    }

    cadena ++;
  }
}

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