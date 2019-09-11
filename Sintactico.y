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
programa : program {printf("\nRegla 0 : Compilación Ok\n");}
;
program : sentencia {printf("\nRegla 1 : Sentencia\n");}
;
sentencia : asignacion_s {printf("\nRegla 2 : Asignación Simple\n");}
    | asignacion_m {printf("\nRegla 3 : Asignación Multiple\n");}
;
asignacion_s : ID OP_ASIG expresion {printf("\nRegla X : Asignación Simple ID :=\n");}
;
asignacion_m : C_A lista_var C_C OP_ASIG C_A lista_exp C_C  {printf("\nRegla X : Asignación Multiple Lista\n");}
;
lista_var : lista_var COMA ID {printf("\nRegla X : Lista, ID\n");}
    | ID {printf("\nRegla X : Lista ID\n");}
;
decision : IF P_A condicion P_C L_A sentencia L_C ELSE L_A sentencia L_C {printf("\nRegla X : Decision con Else\n");}
    | IF P_A comparacion P_C L_A sentencia L_C {printf("\nRegla X : Decision\n");}
;
condicion : ID OP_MAX CTE_ENT {printf("\nRegla X : ID > ENTERO\n");}
    | ID OP_MAX ID {printf("\nRegla X : ID > ID\n");}
    | ID OP_MIN CTE_ENT {printf("\nRegla X : ID < ENTERO\n");}
    | ID OP_MIN ID {printf("\nRegla X : ID < ID\n");}
    | ID OP_MAXEQ CTE_ENT {printf("\nRegla X : ID >= ENTERO\n");}
    | ID OP_MAXEQ ID {printf("\nRegla X : ID >= ID\n");}
    | ID OP_MINEQ CTE_ENT {printf("\nRegla X : ID <= ENTERO\n");}
    | ID OP_MINEQ ID {printf("\nRegla X : ID <= ID\n");}
    | ID OP_EQ CTE_ENT {printf("\nRegla X : ID == ENTERO\n");}
    | ID OP_EQ ID {printf("\nRegla X : ID == ID\n");}
    | ID OP_NEQ CTE_ENT {printf("\nRegla X : ID != ENTERO\n");}
    | ID OP_NEQ ID {printf("\nRegla X : ID != ID\n");}
    | condicion AND condicion {printf("\nRegla X : AND\n");}
    | condicion OR condicion {printf("\nRegla X : OR\n");}
    | NOT ID {printf("\nRegla X : NOT\n");}
;
iteracion : REPEAT sentencia UNTIL condicion {printf("\nRegla X : Repeat\n");}
;
printear : PRINT CTE_STRING {printf("\nRegla X : Print String\n");}
    | PRINT ID {printf("\nRegla X : Print ID\n");}
;
obtain : READ ID {printf("\nRegla X : Read Variable\n");}
;
declaracion : VAR DPTO ENDVAR {printf("\nRegla X : Declaracion Variables\n");}
;
cteNombre : CONST ID EQ CTE_ENT {printf("\nRegla X : Cte Con Nombre Entero\n");}
    | CONST ID EQ CTE_STRING {printf("\nRegla X : Cte Con Nombre String\n");}
;
expresion : expresion OP_SUMA termino {printf("\nRegla X : E + T\n");} 
    | expresion OP_RESTA termino {printf("\nRegla X : E - T\n");} 
    | termino {printf("\nRegla X : Termino\n");}
;
termino : termino OP_MULT factor {printf("\nRegla X : T * F\n");}
    | termino OP_DIV factor {printf("\nRegla X : T / F\n");}
    | factor {printf("\nRegla X : Factor\n");}
;
factor : ID {printf("\nRegla X : ID\n");}
    | CTE_ENT {printf("\nRegla X : Entero\n");}
    | CTE_REAL {printf("\nRegla X : Real\n");}
    | P_A expresion P_C {printf("\nRegla X : (E)\n");}
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
    printf("Syntax Error\n");
	system ("Pause");
	exit (1);
}