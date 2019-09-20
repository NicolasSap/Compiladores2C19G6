%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "y.tab.h"
#include "simbolo.h"
#include "tree.h"

void validateAsignation();
void validateType();
void validateIdDeclaration();

char* seen[200];
int seenIndex = 0;

int flagDeclaration = 0;

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

%type <integer_value> CTE_ENT
%type <float_value> CTE_REAL
%type <string_value> CTE_STRING
%type <string_value> ID
%type <string_value> INT
%type <string_value> STRING
%type <string_value> FLOAT
%type <ast> program
%type <ast> cuerpo_programa
%type <ast> sentencia
%type <ast> lista_de_tipos
%type <ast> tipos_primitivos
%type <ast> asignacion_s
%type <ast> asignacion_m
%type <ast> lista_var
%type <ast> lista_exp
%type <ast> decision
%type <ast> condiciones
%type <ast> condicion
%type <ast> operador_logico
%type <ast> iteracion
%type <ast> printear
%type <ast> obtain
%type <ast> cteNombre
%type <ast> expresion
%type <ast> termino
%type <ast> factor

%union {
  int integer_value;
  float float_value;
  char string_value[30];
  struct treeNode* ast;
}

%start  programa

%% 
programa: program   {printf("\nRegla 00 : Compilacion Ok\n");}
;
program: definiciones_variables                  {printf("\nRegla 1 : Definicion Variables\n");} 
    |   definiciones_variables cuerpo_programa   {printf("\nRegla 2 : Definicion Variables + program\n");} 
;
cuerpo_programa: sentencia                  {printf("\nRegla 3 : Sentencia\n");}
    |   cuerpo_programa sentencia           {flagDeclaration = 1; printf("\nRegla 4 : Program Sentencia\n");}
;
sentencia: asignacion_s     {printf("\nRegla 5 : Asignacion Simple\n");}
    |   asignacion_m        {printf("\nRegla 6 : Asignacion Multiple\n");}
    |   decision            {printf("\nRegla 7 : Decision\n");}
    |   iteracion           {printf("\nRegla 8 : Iteracion\n");}
    |   printear            {printf("\nRegla 9 : Print\n");}
    |   obtain              {printf("\nRegla 10 : READ\n");}
    |   cteNombre           {printf("\nRegla 11 : Constante Nombre\n");}
;
definiciones_variables: VAR definicion_variables ENDVAR {printf("\nRegla 12 : VAR def_Var ENDVAR\n");}
;
definicion_variables: definicion_variables def {printf("\nRegla 13 : Def Variable Multiple Linea\n");}
    |   def {printf("\nRegla 14 : Def Variable Simple Linea\n");}
;
def: C_A lista_de_tipos C_C DPTO C_A lista_var C_C {putTypeIdentifierOnSymbolTable(); printf("\nRegla 15 : Lista Tipos - Lista Vars\n");}
;
lista_de_tipos: lista_de_tipos COMA tipos_primitivos {printf("\nRegla 16 : Lista_Tipos, Tipo_Primitivo\n");}
    | tipos_primitivos {printf("\nRegla 17 : Tipo_Primitivo\n");}
;
tipos_primitivos:   STRING {insertTypeIdentifier($1); printf("\nRegla 18 : Tipo_Primitivo String\n");}
    |   FLOAT {insertTypeIdentifier($1); printf("\nRegla 19 : Tipo_Primitivo Float\n");}
    |   INT {insertTypeIdentifier($1); printf("\nRegla 20 : Tipo_Primitivo Int\n");}
;
asignacion_s: ID OP_ASIG expresion  {printf("\nRegla 21 : Asig Simple ID := EXPRESION\n");}
    |   ID OP_ASIG CTE_STRING       {printf("\nRegla 22 : Asig Simple ID := STRING\n");}
;
asignacion_m: C_A lista_var C_C OP_ASIG C_A lista_exp C_C  {printf("\nRegla 23 : Asignacion Multiple Lista\n");}
;
lista_var: lista_var COMA ID    {if(flagDeclaration == 0){validateIdDeclaration($3); insertIdentifier($3);} printf("\nRegla 24 : Lista, ID\n");}
    |   ID                      {if(flagDeclaration == 0){validateIdDeclaration($1); insertIdentifier($1);} printf("\nRegla 25 : Lista ID\n");}
;
lista_exp: lista_exp COMA expresion   {printf("\nRegla 26 : Lista_EXP, Expresion\n");}
    |   lista_exp COMA CTE_STRING   {printf("\nRegla 27 : Lista_EXP, String\n");}
    |   expresion                     {printf("\nRegla 28 : Expresion\n");}
    |   CTE_STRING  {printf("\nRegla 29 : String\n");}
;
decision: IF P_A condiciones P_C L_A cuerpo_programa L_C ELSE L_A cuerpo_programa L_C   {printf("\nRegla 27* : Decision con Else\n");}
    |   IF P_A condiciones P_C L_A cuerpo_programa L_C {printf("\nRegla 28* : Decision\n");}
;
condiciones: condicion {printf("\nRegla 29* : Condicion\n");}     
    |   condicion AND condicion {printf("\nRegla 30 : cond AND cond\n");}
    |   condicion OR condicion  {printf("\nRegla 31 : cond OR cond\n");}
    |   NOT condicion {printf("\nRegla 32 : NOT cond\n");}
;
condicion: ID operador_logico factor    {printf("\nRegla 33 : ID Operador Logico Comparado\n");}
;
operador_logico: OP_MAX {printf("\nRegla 34 : >\n");}
    |   OP_MIN  {printf("\nRegla 35 : <\n");}
    |   OP_MINEQ {printf("\nRegla 36 : <=\n");}
    |   OP_MAXEQ {printf("\nRegla 37 : >=\n");}
    |   OP_EQ {printf("\nRegla 38 : ==\n");}
    |   OP_NEQ {printf("\nRegla 39 : !=\n");}
;
iteracion: REPEAT cuerpo_programa UNTIL condiciones     {printf("\nRegla 40 : Repeat\n");}
    |   REPEAT cuerpo_programa UNTIL NOT condiciones    {printf("\nRegla 41 : Repeat con NOT\n");}
;
printear: PRINT CTE_STRING      {printf("\nRegla 42 : Print String\n");}
    |   PRINT ID                {printf("\nRegla 43 : Print ID\n");}
;
obtain: READ ID {printf("\nRegla 44 : Read Variable\n");}
;
cteNombre: CONST ID OP_ASIG CTE_ENT     {putConstOnSymbolTable($2, "", $4, 0, "CONST_ENT", 1); printf("\nRegla 45 : Cte Con Nombre Entero\n");}
    |   CONST ID OP_ASIG  CTE_STRING    {putConstOnSymbolTable($2, $4, 0, 0, "CONST_STRING", 2); printf("\nRegla 46 : Cte Con Nombre String\n");}
    |   CONST ID OP_ASIG  CTE_REAL    {putConstOnSymbolTable($2, "", 0, $4, "CONST_FLOAT", 3); printf("\nRegla 46 : Cte Con Nombre Float\n");}
;
expresion: expresion OP_SUMA termino    {printf("\nRegla 47 : E + T\n");} 
    |   expresion OP_RESTA termino      {printf("\nRegla 48 : E - T\n");} 
    |   termino 
;
termino: termino OP_MULT factor     {printf("\nRegla 49 : T * F\n");}
    |   termino OP_DIV factor       {printf("\nRegla 50 : T / F\n");}
    |   factor 
;
factor: ID                  {printf("\nRegla 51 : ID\n");}
    |   CTE_ENT             {printf("\nRegla 52 : Entero\n");}
    |   CTE_REAL            {printf("\nRegla 53 : Real\n");}
    |   P_A expresion P_C   {printf("\nRegla 54 : (E)\n");}
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

void validateIdDeclaration(char* id) {
    int i = 0;
    for(i = 0; i < seenIndex; i++) {
        if(strcmp(seen[i], id) == 0) {
            fprintf(stderr, "\n Identificator already declared, line: %d\n", yylineno);
            exit(1);
        }
    }
    seen[seenIndex] = strdup(id);
    seenIndex++;
}

/*

ID OP_ASIG expresion  {validateAsignation($1, $3); printf("\nRegla 18 : Asig Simple ID := EXPRESION\n");}

void validateAsignation(char* id, ast* exp) {
    printf("Entre a verficar la asignacion\n\n");
    symbolNode* symbol = findSymbol(id);
    printf("ROMPI ACA\n\n");
    symbolNode* treeValue = findSymbol(exp->value);
    printf("Entrando IF\n\n");
    if (symbol != NULL && treeValue != NULL) {
        printf("IF 1\n\n");
        if((strcmp(symbol->type, "INT") == 0 || strcmp(symbol->type, "FLOAT") == 0) && (strcmp(treeValue->type, "STRING") == 0 || strcmp(treeValue->type, "STRING_C") == 0 )) {
            printf("ADENTRO IF 1\n\n");
            fprintf(stderr, "\n Incompatible assignment, line: %d\n", yylineno);
            exit(1);
        }
        printf("IF 2\n\n");
        if((strcmp(symbol->type, "STRING") == 0) && (strcmp(treeValue->type, "INT") == 0 || strcmp(treeValue->type, "FLOAT") == 0  || strcmp(treeValue->type, "INTEGER_C") == 0 || strcmp(treeValue->type, "FLOAT_C") == 0 )) {
            printf("ADENTRO IF 2\n\n");
            fprintf(stderr, "\n Incompatible assignment, line: %d\n", yylineno);
            exit(1);
        }
    }
}
*/
/*
expresion OP_SUMA termino    {validateType($1, $3, 1); printf("\nRegla 47 : E + T\n");} 


void validateType(ast* left, ast* right, int fail) {      
    printf("Entre a verficar los Tipos en una expresion\n\n");
    if(right->value != NULL) {
        printf("A\n\n");
        symbolNode* symbolLeft = findSymbol(left->value);
        printf("B\n\n");
        symbolNode* symbolRight = findSymbol(right->value);
        if(symbolRight != NULL && symbolLeft != NULL) {
        if(fail == 1 && (
            strcmp(symbolLeft->type, "STRING") == 0 || 
            strcmp(symbolLeft->type, "STRING_C") == 0 ||
            strcmp(symbolRight->type, "STRING") == 0 || 
            strcmp(symbolRight->type, "STRING_C") == 0)) {
            fprintf(stderr, "\n Incompatible operation, line: %d\n", yylineno);
            exit(1);
        }
        }
    }
}
*/