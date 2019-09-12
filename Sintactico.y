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
programa: program {printf("\nRegla 0 : Compilacion Ok\n");}
;
program: sentencia {printf("\nRegla 1 : Sentencia\n");}
    | program sentencia {printf("\nRegla 0000 : Program\n");}
;
sentencia: asignacion_s {printf("\nRegla 2 : Asignacion Simple\n");}
    | asignacion_m {printf("\nRegla 3 : Asignacion Multiple\n");}
    | decision {printf("\nRegla 4 : Decision\n");}
    | iteracion {printf("\nRegla 5 : Iteracion\n");}
    | printear {printf("\nRegla 5 : Print\n");}
    | obtain {printf("\nRegla 6 : READ\n");}
    | declaracion {printf("\nRegla 7 : Declaracion\n");}
    | cteNombre {printf("\nRegla 8 : Constante Nombre\n");}
;
asignacion_s: ID OP_ASIG expresion {printf("\nRegla 9 : Asignacion Simple ID :=\n");}
;
asignacion_m: C_A lista_var C_C OP_ASIG C_A lista_exp C_C  {printf("\nRegla 10 : Asignacion Multiple Lista\n");}
;
lista_var: lista_var COMA ID {printf("\nRegla 11 : Lista, ID\n");}
    | ID {printf("\nRegla 12 : Lista ID\n");}
;
lista_exp: lista_exp COMA var {printf("\nRegla 13 : Lista_EXP, ID\n");}
    | var {printf("\nRegla X1 : Variable\n");}
;
var: ID {printf("\nRegla 14 : ListaEXP ID\n");}
    | CTE_ENT {printf("\nRegla X2 : Entero\n");}
    | CTE_REAL {printf("\nRegla X3 : Real\n");}
    | CTE_STRING {printf("\nRegla X4 : String\n");}
;
decision: IF P_A condicion P_C L_A sentencia L_C ELSE L_A sentencia L_C {printf("\nRegla 15 : Decision con Else\n");}
    | IF P_A condicion P_C L_A sentencia L_C {printf("\nRegla 16 : Decision\n");}
;
condicion: ID OP_MAX CTE_ENT {printf("\nRegla 17 : ID > ENTERO\n");}
    | ID OP_MAX ID {printf("\nRegla 18 : ID > ID\n");}
    | ID OP_MIN CTE_ENT {printf("\nRegla 19 : ID < ENTERO\n");}
    | ID OP_MIN ID {printf("\nRegla 20 : ID < ID\n");}
    | ID OP_MAXEQ CTE_ENT {printf("\nRegla 21 : ID >= ENTERO\n");}
    | ID OP_MAXEQ ID {printf("\nRegla 22 : ID >= ID\n");}
    | ID OP_MINEQ CTE_ENT {printf("\nRegla 23 : ID <= ENTERO\n");}
    | ID OP_MINEQ ID {printf("\nRegla 24 : ID <= ID\n");}
    | ID OP_EQ CTE_ENT {printf("\nRegla 25 : ID == ENTERO\n");}
    | ID OP_EQ ID {printf("\nRegla 26 : ID == ID\n");}
    | ID OP_NEQ CTE_ENT {printf("\nRegla 27 : ID != ENTERO\n");}
    | ID OP_NEQ ID {printf("\nRegla 28 : ID != ID\n");}
    | condicion AND condicion {printf("\nRegla 29 : AND\n");}
    | condicion OR condicion {printf("\nRegla 30 : OR\n");}
    | NOT ID {printf("\nRegla 31 : NOT\n");}
;
iteracion: REPEAT sentencia UNTIL condicion {printf("\nRegla 32 : Repeat\n");}
;
printear: PRINT CTE_STRING {printf("\nRegla 33 : Print String\n");}
    | PRINT ID {printf("\nRegla 34 : Print ID\n");}
;
obtain: READ ID {printf("\nRegla 35 : Read Variable\n");}
;
declaracion: VAR DPTO ENDVAR {printf("\nRegla 36 : Declaracion Variables\n");}
;
cteNombre: CONST ID EQ CTE_ENT {printf("\nRegla 37 : Cte Con Nombre Entero\n");}
    | CONST ID EQ CTE_STRING {printf("\nRegla 38 : Cte Con Nombre String\n");}
;
expresion: expresion OP_SUMA termino {printf("\nRegla 39 : E + T\n");} 
    | expresion OP_RESTA termino {printf("\nRegla 40 : E - T\n");} 
    | termino {printf("\nRegla 41 : Termino\n");}
;
termino: termino OP_MULT factor {printf("\nRegla 42 : T * F\n");}
    | termino OP_DIV factor {printf("\nRegla 43 : T / F\n");}
    | factor {printf("\nRegla 44 : Factor\n");}
;
factor: ID {printf("\nRegla 45 : ID\n");}
    | CTE_ENT {printf("\nRegla 46 : Entero\n");}
    | CTE_REAL {printf("\nRegla 47 : Real\n");}
    | P_A expresion P_C {printf("\nRegla 48 : (E)\n");}
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