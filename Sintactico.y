%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "y.tab.h"
int yystopparser=0;
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

// los terminales tienen que estar definidos con %token y son el mas y toda la bola

// las reglas terminan con ;
// ejemplo 
//  expresion: expresion MAS termino;
//  expresion: expresion MENOS termino;
//  expresion: PARENTA expresión PARENTC;

// para printear
// expresion OPMENOS termino {generarPolaca();}|termino {printf("termino \n");};

// expr : PARENTA expr PARENTC;
// El valor retornado por esta regla es, de manera usual, el valor de la expr entre paréntesis. Esto se indica como:
// expr : PARENTA expr PARENTC { $$ = $2; }

%% 
programa: program                         {printf("\nRegla 00 : Compilacion Ok\n");}
;
program: definicion_variables                    {printf("\nRegla 0 : Definicion Variables\n");} 
    |   definicion_variables cuerpo_programa    {printf("\nRegla 0 : Definicion Variables + program\n");} 
;
cuerpo_programa: sentencia                       {printf("\nRegla 0 : Sentencia\n");}
    |   cuerpo_programa sentencia       {printf("\nRegla 0 : Program\n");}
;
sentencia: asignacion_s    {printf("\nRegla 2 : Asignacion Simple\n");}
    |   asignacion_m    {printf("\nRegla 3 : Asignacion Multiple\n");}
    |   decision        {printf("\nRegla 4 : Decision\n");}
    |   iteracion       {printf("\nRegla 5 : Iteracion\n");}
    |   printear        {printf("\nRegla 5 : Print\n");}
    |   obtain          {printf("\nRegla 6 : READ\n");}
    |   declaracion     {printf("\nRegla 7 : Declaracion\n");}
    |   cteNombre       {printf("\nRegla 8 : Constante Nombre\n");}
;
definicion_variables:   definicion_variables ENDVAR                     {printf("\nRegla 10 : Def Variable ENDVAR\n");}
    |   definicion_variables declaracion_tipo_variable  {printf("\nRegla 11 : Def Variable DECLARACION\n");}
    |   VAR declaracion_tipo_variable                   {printf("\nRegla 12 : Def Variable VAR DECLARACION\n");}
;
declaracion_tipo_variable:  declaracion_tipo_variable C_C DPTO declaracion_id   {printf("\nRegla 13 : Declaracion C_C\n");}
    |   declaracion_tipo_variable COMA tipos_primitivos     {printf("\nRegla 14 : Declaracion MAS DE UNO\n");}
    |   C_A tipos_primitivos                                {printf("\nRegla 15 : Declaracion C_A\n");}
;
tipos_primitivos:   STRING
    |   FLOAT
    |   INT
;
declaracion_id: declaracion_id C_C        {printf("\nRegla 16 : Declaracion C_C\n");}
    |   declaracion_id COMA ID    {printf("\nRegla 17 : COMA CTA_ENT\n");}
    |   C_A ID                    {printf("\nRegla 18 : ID\n");}
;
asignacion_s: ID OP_ASIG expresion    {printf("\nRegla 9 : Asig Simple ID := EXPRESION\n");}
    |   ID OP_ASIG CTE_STRING   {printf("\nRegla 10: Asig Simple ID := STRING\n");}
;
asignacion_m: C_A lista_var C_C OP_ASIG C_A lista_exp C_C  {printf("\nRegla 11 : Asignacion Multiple Lista\n");}
;
lista_var: lista_var COMA ID   {printf("\nRegla 12 : Lista, ID\n");}
    |   ID                  {printf("\nRegla 13 : Lista ID\n");}
;
lista_exp: lista_exp COMA var  {printf("\nRegla 14 : Lista_EXP, ID\n");}
    |   var                 {printf("\nRegla 15 : Variable\n");}
;
var: ID          {printf("\nRegla 16 : ID\n");}
    |   CTE_ENT     {printf("\nRegla 17 : Entero\n");}
    |   CTE_REAL    {printf("\nRegla 18 : Real\n");}
    |   CTE_STRING  {printf("\nRegla 19 : String\n");}
;
decision: IF P_A condiciones P_C L_A program L_C ELSE L_A program L_C   {printf("\nRegla 20 : Decision con Else\n");}
    |   IF P_A condiciones P_C L_A program L_C                        {printf("\nRegla 21 : Decision\n");}
;
condiciones: condicion               
    |   condicion AND condicion {printf("\nRegla 35 : AND\n");}
    |   condicion OR condicion  {printf("\nRegla 36 : OR\n");}

;
condicion: ID OP_MAX CTE_ENT       {printf("\nRegla 22 : ID > ENTERO\n");}
    |   ID OP_MAX ID            {printf("\nRegla 23 : ID > ID\n");}
    |   ID OP_MIN CTE_ENT       {printf("\nRegla 24 : ID < ENTERO\n");}
    |   ID OP_MIN ID            {printf("\nRegla 25 : ID < ID\n");}
    |   ID OP_MAXEQ CTE_ENT     {printf("\nRegla 26 : ID >= ENTERO\n");}
    |   ID OP_MAXEQ ID          {printf("\nRegla 27 : ID >= ID\n");}
    |   ID OP_MINEQ CTE_ENT     {printf("\nRegla 28 : ID <= ENTERO\n");}
    |   ID OP_MINEQ ID          {printf("\nRegla 29 : ID <= ID\n");}
    |   ID OP_EQ CTE_ENT        {printf("\nRegla 30 : ID == ENTERO\n");}
    |   ID OP_EQ ID             {printf("\nRegla 31 : ID == ID\n");}
    |   ID OP_NEQ CTE_ENT       {printf("\nRegla 32 : ID != ENTERO\n");}
    |   ID OP_NEQ ID            {printf("\nRegla 33 : ID != ID\n");}
;
iteracion: REPEAT program UNTIL condiciones     {printf("\nRegla 37 : Repeat\n");}
    |   REPEAT program UNTIL NOT condiciones {printf("\nRegla 34 : NOT\n");}
;
printear: PRINT CTE_STRING    {printf("\nRegla 38 : Print String\n");}
    |   PRINT ID            {printf("\nRegla 39 : Print ID\n");}
;
obtain: READ ID {printf("\nRegla 40 : Read Variable\n");}
;
declaracion: VAR C_A lista_tipos C_C DPTO OP_ASIG C_A lista_var C_C ENDVAR {printf("\nRegla 41 : Declaracion Variables\n");}
;
lista_tipos: lista_tipos COMA INT   {printf("\nRegla 42 : Lista_Tipo, INT\n");}
    |   lista_tipos COMA FLOAT   {printf("\nRegla 43 : FLOAT\n");}
    |   INT {printf("\nRegla 44 : Tipo Integer\n");}
    |   FLOAT   {printf("\nRegla 45 : Tipo Float\n");}
;
cteNombre: CONST ID OP_ASIG CTE_ENT     {printf("\nRegla 46 : Cte Con Nombre Entero\n");}
    |   CONST ID OP_ASIG  CTE_STRING  {printf("\nRegla 47 : Cte Con Nombre String\n");}
;
expresion: expresion OP_SUMA termino   {printf("\nRegla 48 : E + T\n");} 
    |   expresion OP_RESTA termino  {printf("\nRegla 49 : E - T\n");} 
    |   termino 
;
termino: termino OP_MULT factor  {printf("\nRegla 50 : T * F\n");}
    |   termino OP_DIV factor   {printf("\nRegla 51 : T / F\n");}
    |   factor 
;
factor: ID                  {printf("\nRegla 48 : ID\n");}
    |   CTE_ENT             {printf("\nRegla 49 : Entero\n");}
    |   CTE_REAL            {printf("\nRegla 50 : Real\n");}
    |   P_A expresion P_C   {printf("\nRegla 51 : (E)\n");}
;
%%

int main(int argc,char *argv[]){
    if ((yyin = fopen(argv[1], "rt")) == NULL) {
	    printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
    } else {
	    yyparse();
    }
    fclose(yyin);
    return 0;
}

int yyerror(void) {
    printf("\n[!] Syntax Error\n\n");
	system ("Pause");
	exit (1);
}