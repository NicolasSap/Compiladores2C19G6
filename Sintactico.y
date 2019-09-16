%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "y.tab.h"
#include "simbolo.h"

int yystopparser=0;
extern yylineno;
FILE  *yyin;
%}

%token  DIGITO
%token  LETRA 
%token  CHAR_ESP
%token  COMMENT
%token  ID
%token  CTE_STRING
%token  CTE_ENT
%token  CTE_REAL
%token  P_A
%token  P_C
%token  C_A
%token  C_C
%token  L_A
%token  L_C
%token  PYC
%token  PTO
%token  DPTO
%token  COMA
%token  OP_MAX
%token  OP_MAXEQ
%token  OP_MIN
%token  OP_MINEQ
%token  OP_NEQ
%token  OP_EQ
%token  EQ
%token  IF
%token  ELSE
%token  ENDIF
%token  VAR
%token  ENDVAR
%token  REPEAT
%token  UNTIL
%token  PRINT
%token  READ
%token  CONST
%token  INT
%token  FLOAT
%token  STRING

%right  MENOS_UNARIO
%right  OP_ASIG

%left   OP_SUMA
%left   OP_RESTA
%left   OP_MULT
%left   OP_DIV
%left   AND
%left   OR
%left   NOT

%start  programa

%% 
programa: program   {printf("\nRegla 00 : Compilacion Ok\n");}
;
program: definicion_variables                  {printf("\nRegla 1 : Definicion Variables\n");} 
    |   definicion_variables cuerpo_programa   {printf("\nRegla 2 : Definicion Variables + program\n");} 
;
cuerpo_programa: sentencia                  {printf("\nRegla 3 : Sentencia\n");}
    |   cuerpo_programa sentencia           {printf("\nRegla 4 : Program Sentencia\n");}
;
sentencia: asignacion_s     {printf("\nRegla 5 : Asignacion Simple\n");}
    |   asignacion_m        {printf("\nRegla 6 : Asignacion Multiple\n");}
    |   decision            {printf("\nRegla 7 : Decision\n");}
    |   iteracion           {printf("\nRegla 8 : Iteracion\n");}
    |   printear            {printf("\nRegla 9 : Print\n");}
    |   obtain              {printf("\nRegla 10 : READ\n");}
    |   cteNombre           {printf("\nRegla 11 : Constante Nombre\n");}
;
definicion_variables: VAR C_A lista_de_tipos C_C DPTO C_A lista_de_variables C_C ENDVAR {printf("\nRegla 12 : Def Variable VAR - ENDVAR\n");}
;
lista_de_tipos: lista_de_tipos COMA tipos_primitivos {printf("\nRegla 13 : Lista_Tipos, Tipo_Primitivo\n");}
    | tipos_primitivos {printf("\nRegla 14 : Tipo_Primitivo\n");}
;
tipos_primitivos:   STRING {printf("\nRegla 15 : Tipo_Primitivo String\n");}
    |   FLOAT {printf("\nRegla 16 : Tipo_Primitivo Float\n");}
    |   INT {printf("\nRegla 17 : Tipo_Primitivo Int\n");}
;
lista_de_variables: lista_de_variables COMA ID {printf("\nRegla 18 : Lista_Variables, ID\n");}
    | ID {printf("\nRegla 19 : ID de lista_variables\n");}
;
asignacion_s: ID OP_ASIG expresion  {printf("\nRegla 20 : Asig Simple ID := EXPRESION\n");}
    |   ID OP_ASIG CTE_STRING       {printf("\nRegla 21 : Asig Simple ID := STRING\n");}
;
asignacion_m: C_A lista_var C_C OP_ASIG C_A lista_exp C_C  {printf("\nRegla 22 : Asignacion Multiple Lista\n");}
;
lista_var: lista_var COMA ID    {printf("\nRegla 23 : Lista, ID\n");}
    |   ID                      {printf("\nRegla 24 : Lista ID\n");}
;
lista_exp: lista_exp COMA var   {printf("\nRegla 25 : Lista_EXP, ID\n");}
    |   var                     {printf("\nRegla 26 : Variable\n");}
;
var: ID             {printf("\nRegla 27 : ID var\n");}
    |   CTE_ENT     {printf("\nRegla 28 : Entero var\n");}
    |   CTE_REAL    {printf("\nRegla 29 : Real var\n");}
    |   CTE_STRING  {printf("\nRegla 30 : String var\n");}
;
decision: IF P_A condiciones P_C L_A cuerpo_programa L_C ELSE L_A cuerpo_programa L_C   {printf("\nRegla 31 : Decision con Else\n");}
    |   IF P_A condiciones P_C L_A cuerpo_programa L_C {printf("\nRegla 32 : Decision\n");}
;
condiciones: condicion {printf("\nRegla 33 : Condicion\n");}     
    |   condicion AND condicion {printf("\nRegla 34 : cond AND cond\n");}
    |   condicion OR condicion  {printf("\nRegla 35 : cond OR cond\n");}
    |   NOT condicion {printf("\nRegla 36 : NOT cond\n");}
;
condicion: ID operador_logico comparado    {printf("\nRegla 37 : ID Operador Logico Comparado\n");}
;
operador_logico: OP_MAX {printf("\nRegla 38 : >\n");}
    |   OP_MIN  {printf("\nRegla 39 : <\n");}
    |   OP_MINEQ {printf("\nRegla 40 : <=\n");}
    |   OP_MAXEQ {printf("\nRegla 41 : >=\n");}
    |   OP_EQ {printf("\nRegla 42 : ==\n");}
    |   OP_NEQ {printf("\nRegla 43 : !=\n");}
;
comparado: ID {printf("\nRegla 43 : ID Comparado\n");}
    |   CTE_ENT {printf("\nRegla 44 : CTE_ENT Comparado\n");}
    |   CTE_REAL {printf("\nRegla 45 : CTE_REAL Comparado\n");}
;
iteracion: REPEAT cuerpo_programa UNTIL condiciones     {printf("\nRegla 46 : Repeat\n");}
    |   REPEAT cuerpo_programa UNTIL NOT condiciones    {printf("\nRegla 47 : Repeat con NOT\n");}
;
printear: PRINT CTE_STRING      {printf("\nRegla 48 : Print String\n");}
    |   PRINT ID                {printf("\nRegla 49 : Print ID\n");}
;
obtain: READ ID {printf("\nRegla 50 : Read Variable\n");}
;
cteNombre: CONST ID OP_ASIG CTE_ENT     {printf("\nRegla 51 : Cte Con Nombre Entero\n");}
    |   CONST ID OP_ASIG  CTE_STRING    {printf("\nRegla 52 : Cte Con Nombre String\n");}
;
expresion: expresion OP_SUMA termino    {printf("\nRegla 53 : E + T\n");} 
    |   expresion OP_RESTA termino      {printf("\nRegla 54 : E - T\n");} 
    |   termino 
;
termino: termino OP_MULT factor     {printf("\nRegla 55 : T * F\n");}
    |   termino OP_DIV factor       {printf("\nRegla 56 : T / F\n");}
    |   factor 
;
factor: ID                  {printf("\nRegla 57 : ID\n");}
    |   CTE_ENT             {printf("\nRegla 58 : Entero\n");}
    |   CTE_REAL            {printf("\nRegla 59 : Real\n");}
    |   P_A expresion P_C   {printf("\nRegla 60 : (E)\n");}
;
%%

int main(int argc,char *argv[]){
    if ((yyin = fopen(argv[1], "rt")) == NULL) {
	    printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
    } else {
        symbolTable = NULL;
            do {
                yyparse();
            } while(!feof(yyin));
        saveTable();
    }
    fclose(yyin);
    return 0;
}

int yyerror(void) {
    printf("\n[!] Syntax Error on line %d\n\n", yylineno);
	system ("Pause");
	exit (1);
}