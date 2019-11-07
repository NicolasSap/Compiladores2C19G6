#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "simbolo.h"
#include "tree.h"

void generateAssembler();
void generateCode();
void initiateCode();
void goThroughTree();
void generateCodeOperation();
void generateCodeAsignation();
void generateCodeAsignationSimple();
char * removeFirstCharConstant();
char * insertFirtsChar();
void generateSumCode();
void freeStack();
char auxString[200];

/*
void padding();

void padding(char ch, int n) {
    int i;

    for ( i = 0; i < n; i++ ){
        putchar ( ch );
    }
}
*/

void generateAssembler(ast* tree) {
    ast* copy = tree;
    file = fopen("Final.asm", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    initiateCode();
    goThroughTree(copy);
    // free all st(?)
    freeStack();
    fclose(file);
}

void initiateCode() {
    char nameVar[200];
    char type[200];
    char length[200];
    char valueVar[200];

    FILE * ts = fopen("ts.txt", "r");
    if (ts == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    strcpy(auxString,"*010101*"); // Código para que no printee FSTP
    fprintf(file,"include macros2.asm \n\n\n");
    fprintf(file,".MODEL LARGE\n");
    fprintf(file,".386\n");
    fprintf(file,".STACK 200h\n\n");
    fprintf(file,"MAXTEXTSIZE equ 40\n\n");
    fprintf(file,".DATA\n");

    fgets(nameVar, 200, ts);
    // VER BIEN LO DE SI ES STRING O SI ES INT O ALGUNA WEA ASI
    while (!feof(ts)) {
        fscanf (ts, "%s\t%s\t%s\t%s", nameVar, type, valueVar, length);
        if (strcmp(type, "STRING") == 0) {
           fprintf(file, "\t%s\tdb\tMAXTEXTSIZE dup (?),'$'\n", nameVar);
        } else if (strcmp(type, "STRING_CTE") == 0) {
            fprintf(file, "\t%s\tdb\t'%s','$', %d dup (?)\n", nameVar, valueVar, length);
        } else if (strcmp(type, "INTEGER_CTE") == 0) {
            fprintf(file, "\t%s\tdd\t%s.0\n", nameVar, valueVar);
        } else if(strcmp(valueVar,"-") == 0) {
            fprintf(file, "\t%s\tdd\t?\n", nameVar);
        } else {
            fprintf(file, "\t%s\tdd\t%s\n", nameVar, valueVar);
        }
    }
    fprintf(file, "\t_SUM\tdd\t?\n");
    fprintf(file, "\t_SUB\tdd\t?\n");
    fprintf(file, "\t_MUL\tdd\t?\n");
    fprintf(file, "\t_DIV\tdd\t?\n");
    fclose(ts);

    fprintf(file, "\n\n.CODE\n");
}

void goThroughTree(ast *root) {
    if ( root->left != NULL ) {
        goThroughTree (root->left);
    }
    if( root->right != NULL ) {
        goThroughTree (root->right);
    }
    generateCode(root);
}

void generateCode(ast* root) {
    if(root->right != NULL && root->left  != NULL) {
        printf("\tLEFT=[%s]\t[%s]\tRIGHT[%s]\n", root->left->value, root->value, root->right->value);
        char operation[200];
        if(strcmp(root->value,"+") == 0) {
            strcpy(operation, "ADD");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"*") == 0) {        
            strcpy(operation, "MUL");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"/") == 0) {        
            strcpy(operation, "DIV");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"-") == 0) {       
            strcpy(operation, "SUB"); 
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,":=") == 0) {       
            if (strcmp(root->right->value,"_SUM") == 0 || strcmp(root->right->value,"_SUB") == 0 || strcmp(root->right->value,"_MUL") == 0 || strcmp(root->right->value,"_DIV") == 0) {
                generateCodeAsignation(root);
            } else {
                generateCodeAsignationSimple(root);
            }
            
        }
    }
}

void generateCodeOperation(ast * root, char * operation) {
    // Search operands to know the type
    if(strcmp(auxString,"*010101*") != 0) {
        fprintf(file, "%s", auxString);
        strcpy(auxString,"*010101*"); // Código para que no printee FSTP
    }

    fprintf(file, "\t; %s\n", operation);

    fprintf(file, "\tFLD %s\n", root->left->value);
    fprintf(file, "\tFLD %s\n", root->right->value);
    
    // Check if + - / * and print in .asm
    if(strcmp(operation,"ADD") == 0) {
        fprintf(file, "\tFADD\n", root->value);
        root->value = "_SUM";
    } else if (strcmp(operation,"SUB") == 0) {
        fprintf(file, "\tFSUB\n", root->value);
        root->value = "_SUB";
    } else if (strcmp(operation,"MUL") == 0) {
        fprintf(file, "\tFMUL\n", root->value);
        root->value = "_MUL";  
    } else if (strcmp(operation,"DIV") == 0) {
        fprintf(file, "\tFDIV\n", root->value);
        root->value = "_DIV";  
    }
    sprintf(auxString, "\tFSTP %s\n\n", root->value);
}

void generateCodeAsignation(ast * root) {
    fprintf(file, "\tFSTP %s\n\n", root->left->value); 
    strcpy(auxString,"*010101*"); // Código para que no printee FSTP
}

void generateCodeAsignationSimple(ast * root) {
    fprintf(file, "\t; Simple Asignation\n");
    fprintf(file, "\tFLD %s\n", root->right->value);
    fprintf(file, "\tFSTP %s\n\n", root->left->value); 
    strcpy(auxString,"*010101*"); // Código para que no printee FSTP
}

void freeStack() {
    fprintf(file, "\n\t; FREE STACK\n"); 
    fprintf(file, "\tFFREE st(0)\n");
    fprintf(file, "\tFFREE st(1)\n");
    fprintf(file, "\tFFREE st(2)\n");
    fprintf(file, "\tFFREE st(3)\n");
    fprintf(file, "\tFFREE st(4)\n");
    fprintf(file, "\tFFREE st(5)\n");
    fprintf(file, "\tFFREE st(6)\n");
    fprintf(file, "\tFFREE st(7)\n\n");
    fprintf(file, "\tEND START");
}
/*
char * removeFirstCharConstant(char * constant) {
    char * aux = (char *) malloc(strlen(constant)-1);
    strcpy(aux, ++constant);
    return aux;
}

char * insertFirtsChar(char * constant) {
    char * aux = (char *) malloc(strlen(constant)+1);
    aux[0] = '_';
    strcat(aux,constant);
    return aux;
}*/