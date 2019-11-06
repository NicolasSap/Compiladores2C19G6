#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "simbolo.h"
#include "tree.h"

void generateAssembler();
void generateCode();
void goThroughTree();
void generateCodeOperation();
void generateCodeAsignation();
char * removeFirstCharConstant();
char * insertFirtsChar();
void generateSumCode();
void freeStack();

/*
void padding();

void padding(char ch, int n) {
    int i;

    for ( i = 0; i < n; i++ ){
        putchar ( ch );
    }
}
*/

void generateSumCode(char* n1, char* n2) {
    printf(".DATA\n");
    printf("\nN1 \t dd \t %s", n1);
    printf("\nN2 \t dd \t %s", n2);
    printf("\nRes \t dd \t ?");

    printf("\n\n.CODE\n");
    printf("fild  \t N1\n");
    printf("fild  \t N2\n");
    printf("fadd\n");
    printf("fstp  \t Res\n");
    printf("ffree\n");
}

void generateAssembler(ast* tree) {
    ast* copy = tree;
    file = fopen("Final.asm", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    goThroughTree(copy);
    fclose(file);
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
        printf("\t[%s]\n", root->value);
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
        }/*else if (strcmp(root->value,":=") == 0) {       
            strcpy(operation, "ASIG"); 
            generateCodeOperation(root, &operation);
            //generateCodeAsignation(root, &operation);
        }*/
    }
}

void generateCodeOperation(ast * root, char * operation) {
    // Search operands to know the type
    symbolNode* symbolLeft = findSymbol(root->left->value);
    symbolNode* symbolRight = findSymbol(root->right->value);

    // If int do FILD, IF float do FLD for left then for right
    //Left
    if(strcmp(symbolLeft->type,"CTE_ENT") == 0 || strcmp(symbolLeft->type,"INTEGER_CTE") == 0 || strcmp(symbolLeft->type,"INT") == 0){
        fprintf(file, "\tFILD %s\n", root->left->value);
    }else if(strcmp(symbolLeft->type,"CTE_REAL") == 0 || strcmp(symbolLeft->type,"FLOAT_CTE") == 0 || strcmp(symbolLeft->type,"FLOAT") == 0){
        fprintf(file, "\tFLD %s\n", root->left->value);
    }
    //Right
    if(strcmp(symbolRight->type,"CTE_ENT") == 0 || strcmp(symbolRight->type,"INTEGER_CTE") == 0 || strcmp(symbolRight->type,"INT") == 0){
        fprintf(file, "\tFILD %s\n", root->right->value);
    }else if(strcmp(symbolRight->type,"CTE_REAL") == 0 || strcmp(symbolRight->type,"FLOAT_CTE") == 0 || strcmp(symbolRight->type,"FLOAT") == 0){
        fprintf(file, "\tFLD %s\n", root->right->value);
    }

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
    fprintf(file, "\tFSTP %s\n", root->value);
    // free all st(?)
    freeStack();
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
     fprintf(file, "\tFFREE st(7)\n");
     fprintf(file, "\n");
}
/*
void generateCodeOperation(ast * root, char * operation) {
    // Left leaf
    char * aux;
    if(root->left->value[0] == '@') {
        aux = root->left->value;
    }else if (root->left->value[0] == '_') {
        aux = removeFirstCharConstant(root->left->value);
    }else{
        aux = insertFirtsChar(root->left->value);
    }

    printf("MOV R1, %s\n", aux);
    free(aux);
    // Rigth leaf
    aux = (char *) malloc(strlen(root->right->value)+1);
    if(root->right->value[0] == '@') {
        aux = root->left->value;
    }else if (root->right->value[0] == '_') {
        aux = removeFirstCharConstant(root->right->value);
    }else{
        aux = insertFirtsChar(root->right->value);
    }

    //Node
    printf("%s R1, %s\n",operation,aux);
    free(aux);
    
    char *s;
    s = (char *) malloc(200);
    sprintf(s,"@AUX_%d\0",contador);

    free(root->value);
    root->value = (char *) malloc(strlen(s));
    strcpy(root->value, s);

    printf("MOV %s, R1\n",root->value);

    contador ++;
}
*/
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
}
/*
void generateCodeAsignation(ast * root, char * operation) {
    char * aux;
    aux = (char *) malloc(strlen(root->right->value)+1);
    if(root->right->value[0] == '@') {
        aux = root->left->value;
    } else if (root->right->value[0] == '_') {
        aux = removeFirstCharConstant(root->right->value);
    } else {
        aux = insertFirtsChar(root->right->value);
    }
    printf("%s R1, %s\n",operation,aux);
    printf("MOV _%s, R1\n", root->left->value);
}*/