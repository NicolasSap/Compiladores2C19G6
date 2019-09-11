%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "y.tab.h"
int yystopparser=0;
FILE  *yyin;
%}

%token  DIGITO LETRA CHAR_ESP COMMENT
%token  ID CTE_STRING CTE_ENT CTE_REAL
%right  MENOS_UNARIO OP_ASIG
%left   OP_SUMA OP_RESTA OP_MULT OP_DIV
%token  P_A P_C C_A C_C L_A L_C PYC PTO COMA
%token  AND OR NOT OP_MAX OP_MAXEQ OP_MIN OP_MINEQ OP_NEQ OP_EQ

%start programa

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
programa : program {printf("Compilación OK\n");}
;
program : sentencia
;
sentencia : asignacion_s {printf("Asiganción Simple\n");} | asignacion_m {printf("Asiganción Multiple\n");}
;
asignacion_s : ID OP_ASIG expresion
;
asignacion_m :
;
expresion : expresion OP_SUMA termino {printf("Suma\n");} | expresion OP_RESTA termino {printf("Resta\n");} | termino
;
termino :
;
factor :
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