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
OP_SUMA         "+"
OP_RESTA        "-"
OP_ASIG         ":="
OP_MULT         "*"
OP_DIV          "/"
OP_EQ           "=="
OP_NEQ          "!="
OP_MIN          "<"
OP_MAX          ">"
OP_MINEQ        "<="
OP_MAXEQ        ">="
AND             "&&"
OR              "||"
NOT             "!"
P_A             "("
P_C             ")"
C_A             "["
C_C             "]"
L_A             "{"
L_C             "}"
PYC             ";"
COMA            ","
PTO             "."
BLANK           " "
IF              "IF"
ELSE            "ELSE"
ENDIF           "ENDIF"
REPEAT          "REPEAT"
UNTIL           "UNTIL"
VAR             "VAR"
ENDVAR          "ENDVAR"
INT             "INTEGER"
FLOAT           "FLOAT"
STING           "STRING"
PRINT           "PRINT"
READ            "READ"
CONST           "CONST"
ID			    {LETRA}({LETRA}|{DIGITO}|_)*
CTE_STING       \"({LETRA}|{NUMERO}|{CHAR_ESP})*\"
CTE_ENT         {DIGITO}+
CTE_REAL        {DIGITO}*"."{DIGITO}+|{DIGITO}+"."{DIGITO}*|{DIGITO}+"."{DIGITO}+
COMMENT         "--/"({LETRA}|{NUMERO}|{CHAR_ESP})*"/--"|"--/"({LETRA}|{NUMERO}|{CHAR_ESP})*"--/"({LETRA}|{NUMERO}|{CHAR_ESP})*"/--"({LETRA}|{NUMERO}|{CHAR_ESP})*"/--"

%%

%%