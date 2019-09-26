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

int flagAsigM = 0;
int flagDeclaration = 0;

int yystopparser=0;
extern yylineno;
FILE  *yyin;
ast* tree;
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
%token  OP_ASIG

%left   OP_SUMA
%left   OP_RESTA
%left   OP_MULT
%left   OP_DIV
%left   AND
%left   OR
%left   NOT
%left   OP_MAX
%left   OP_MAXEQ
%left   OP_MIN
%left   OP_MINEQ
%left   OP_NEQ
%left   OP_EQ
%left   EQ

%right  MENOS_UNARIO

%type <integer_value> CTE_ENT
%type <float_value> CTE_REAL
%type <string_value> CTE_STRING
%type <string_value> ID
%type <string_value> INT
%type <string_value> STRING
%type <string_value> FLOAT
%type <ast> programa
%type <ast> program
%type <ast> cuerpo_programa
%type <ast> sentencia
%type <ast> lista_de_tipos
%type <ast> tipos_primitivos
%type <ast> asignacion_s
%type <ast> asignacion_m
%type <ast> lista_var
%type <ast> lista_exp
%type <ast> tipo_exp
%type <ast> decision
%type <ast> condiciones
%type <ast> condicion
%type <ast> iteracion
%type <ast> printear
%type <ast> obtain
%type <ast> cteNombre
%type <ast> constExp
%type <ast> expresion
%type <ast> termino
%type <ast> factor
%type <auxLogicOperator> operador_logico

%union {
  int integer_value;
  float float_value;
  char string_value[33];
  struct treeNode* ast;
  char* auxLogicOperator;
}

%start  programa

%% 
programa: program   {$$ = $1; tree = $$; printf("\nRegla 00 : Compilacion Ok\n");}
;
program: definiciones_variables cuerpo_programa   {$$ = $2; printf("\nRegla 1 : Definicion Variables + program\n");} 
    |   definiciones_variables                  {printf("\nRegla 2 : Definicion Variables\n");} 
;
cuerpo_programa: cuerpo_programa sentencia {flagDeclaration = 1; $$ = newNode("Cuerpo_Programa",$1,$2); printf("\nRegla 3 : Program Sentencia\n");}
    |   sentencia {$$ = $1; printf("\nRegla 4 : Sentencia\n");}
;
sentencia: asignacion_s     {$$ = $1; printf("\nRegla 5 : Asignacion Simple\n");}
    |   asignacion_m        {$$ = $1; printf("\nRegla 6 : Asignacion Multiple\n");}
    |   decision            {$$ = $1; printf("\nRegla 7 : Decision\n");}
    |   iteracion           {$$ = $1; printf("\nRegla 8 : Iteracion\n");}
    |   printear            {$$ = $1; printf("\nRegla 9 : Print\n");}
    |   obtain              {$$ = $1; printf("\nRegla 10 : READ\n");}
    |   cteNombre           {$$ = $1; printf("\nRegla 11 : Constante Nombre\n");}
;
asignacion_s: ID OP_ASIG expresion  {$$ = newNode(":=",newLeaf($1),$3); printf("\nRegla 12 : Asig Simple ID := EXPRESION\n");}
    |   ID OP_ASIG CTE_STRING       {$$ = newNode(":=",newLeaf($1),newLeaf(getSymbolName($3,3))); printf("\nRegla 13 : Asig Simple ID := STRING\n");}
;
asignacion_m: C_A {flagAsigM = 1;} lista_var C_C OP_ASIG C_A lista_exp C_C  {flagAsigM =0; $$ = incrustarArbol(); clearAsigMList(); printf("\nRegla 14 : Asignacion Multiple Lista\n");}
;
lista_var: lista_var COMA ID    {if(flagDeclaration == 0){validateIdDeclaration($3); insertIdentifier($3);} if(flagAsigM == 1){insertAsigM($3,1,0,0,0);} printf("\nRegla 15 : Lista, ID\n");}
    |   ID                      {if(flagDeclaration == 0){validateIdDeclaration($1); insertIdentifier($1);} if(flagAsigM == 1){insertAsigM($1,1,0,0,0);} printf("\nRegla 16 : Lista ID\n");}
;
lista_exp: lista_exp COMA tipo_exp   {printf("\nRegla 17 : Lista_EXP, Expresion\n");}
    |   tipo_exp {printf("\nRegla 18 : tipo_exp\n");}  
;
tipo_exp: expresion {printf("\nRegla 19 : Expresion\n");}
    |   CTE_STRING {insertAsigM($1,2,0,0,1); printf("\nRegla 20 : String\n");}
;
decision: IF P_A condiciones P_C L_A cuerpo_programa L_C ELSE L_A cuerpo_programa L_C   {$$ = newNode("IF", $3, newNode("CUERPO_IF",$6,$10)); printf("\nRegla 21 : Decision con Else\n");}
    |   IF P_A condiciones P_C L_A cuerpo_programa L_C {$$ = newNode("IF", $3, $6); printf("\nRegla 22 : Decision\n");}
;
condiciones: condicion AND condicion {$$ = newNode("AND", $1, $3); printf("\nRegla 23 : cond AND cond\n");}
    |   condicion OR condicion  {$$ = newNode("OR", $1, $3); printf("\nRegla 24 : cond OR cond\n");}
    |   NOT P_A condicion P_C {$$ = newNode("NOT", $3, NULL); printf("\nRegla 25 : NOT cond\n");}
    |   condicion {$$ = $1; printf("\nRegla 26 : Condicion\n");}
;
condicion: ID operador_logico factor    {$$ = newNode($2,newLeaf($1),$3); printf("\nRegla 27 : ID Operador Logico Comparado\n");}
;
operador_logico: OP_MAX {$$ = ">"; printf("\nRegla 28 : >\n");}
    |   OP_MIN  {$$ = "<"; printf("\nRegla 29 : <\n");}
    |   OP_MINEQ {$$ = "<="; printf("\nRegla 30 : <=\n");}
    |   OP_MAXEQ {$$ = ">="; printf("\nRegla 31 : >=\n");}
    |   OP_EQ {$$ = "=="; printf("\nRegla 32 : ==\n");}
    |   OP_NEQ {$$ = "!="; printf("\nRegla 33 : !=\n");}
;
iteracion: REPEAT cuerpo_programa UNTIL condiciones     {$$ = newNode("REPEAT",newNode("UNTIL",$4,NULL),$2);printf("\nRegla 34 : Repeat\n");}
    |   REPEAT cuerpo_programa UNTIL NOT condicion    {$$ = newNode("REPEAT",newNode("UNTIL",newNode("NOT",$5,NULL),NULL),$2);printf("\nRegla 35 : Repeat con NOT\n");}
;
printear: PRINT CTE_STRING      {$$ = newNode("READ",newLeaf(getSymbolName($2,3)),NULL);printf("\nRegla 36 : Print String\n");}
    |   PRINT ID                {$$ = newNode("PRINT",newLeaf($2),NULL); printf("\nRegla 37 : Print ID\n");}
;
obtain: READ ID {$$ = newNode("READ",newLeaf($2),NULL); printf("\nRegla 38 : Read Variable\n");}
;
cteNombre: CONST constExp {$$ = $2; printf("\nRegla 38* CONST constExp\n");}
;
constExp: ID OP_ASIG CTE_ENT     {putConstOnSymbolTable($1, "", $3, 0, "CONST_ENT", 1); $$ = newNode(":=",newLeaf($1),newLeaf(getSymbolName(&($3),1))); printf("\nRegla 39 : Cte Con Nombre Entero\n");}
    |   ID OP_ASIG  CTE_STRING    {putConstOnSymbolTable($1, $3, 0, 0, "CONST_STRING", 2); $$ = newNode(":=",newLeaf($1),newLeaf(getSymbolName($3,3))); printf("\nRegla 40 : Cte Con Nombre String\n");}
    |   ID OP_ASIG  CTE_REAL    {putConstOnSymbolTable($1, "", 0, $3, "CONST_FLOAT", 3); $$ = newNode(":=",newLeaf($1),newLeaf(getSymbolName(&($3),2))); printf("\nRegla 41 : Cte Con Nombre Float\n");}
;
expresion: expresion OP_SUMA termino    {$$ = newNode("+",$1,$3); printf("\nRegla 42 : E + T\n");} 
    |   expresion OP_RESTA termino      {$$ = newNode("-",$1,$3); printf("\nRegla 43 : E - T\n");} 
    |   termino {$$ = $1;}
;
termino: termino OP_MULT factor     {$$ = newNode("*",$1,$3); printf("\nRegla 44 : T * F\n");}
    |   termino OP_DIV factor       {$$ = newNode("/",$1,$3); printf("\nRegla 45 : T / F\n");}
    |   factor {$$ = $1;}
;
factor: ID                  {$$ = newLeaf($1); if(flagAsigM == 1){insertAsigM($1,2,0,0,1);} printf("\nRegla 46 : ID\n");}
    |   CTE_ENT             {$$ = newLeaf(getSymbolName(&($1),1)); if(flagAsigM == 1){insertAsigM("",2,0,$1,2);} printf("\nRegla 47 : Entero\n");}
    |   CTE_REAL            {$$ = newLeaf(getSymbolName(&($1),2)); if(flagAsigM == 1){insertAsigM("",2,$1,0,3);} printf("\nRegla 48 : Real\n");}
    |   P_A expresion P_C   {$$ = $2; printf("\nRegla 49 : (E)\n");}
;
definiciones_variables: VAR definicion_variables ENDVAR {printf("\nRegla 50 : VAR def_Var ENDVAR\n");}
;
definicion_variables: definicion_variables def {printf("\nRegla 51 : Def Variable Multiple Linea\n");}
    |   def {printf("\nRegla 52 : Def Variable Simple Linea\n");}
;
def: C_A lista_de_tipos C_C DPTO C_A lista_var C_C {putTypeIdentifierOnSymbolTable(); printf("\nRegla 53 : Lista Tipos - Lista Vars\n");}
;
lista_de_tipos: lista_de_tipos COMA tipos_primitivos {printf("\nRegla 54 : Lista_Tipos, Tipo_Primitivo\n");}
    | tipos_primitivos {printf("\nRegla 55 : Tipo_Primitivo\n");}
;
tipos_primitivos:   STRING {insertTypeIdentifier($1); printf("\nRegla 56 : Tipo_Primitivo String\n");}
    |   FLOAT {insertTypeIdentifier($1); printf("\nRegla 57 : Tipo_Primitivo Float\n");}
    |   INT {insertTypeIdentifier($1); printf("\nRegla 58 : Tipo_Primitivo Int\n");}
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
        ast treeCopy = *tree;
        printAndSaveAST(tree);
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