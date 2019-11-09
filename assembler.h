#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "simbolo.h"
#include "tree.h"

struct stackStatements { 
    int top; 
    int* array; 
};

struct stackOperators { 
    int top; 
    char array[5000][300]; 
}; 

void printTsInAsm();
void swapTopStack();
void generateAssembler();
void generateCondition();
void generateCode();
void initiateCode();
int verifyIsCondition();
void generateAndCondition();
void generateOrCondition();
void goThroughTree();
void printLabel();
void printJump();
void printOrJump();
int pop();
void push();
char* popOperator();
void pushOperator();
void generateCodeOperation();
void generateCodeAsignation();
void generateCodeAsignationSimple();
char * removeFirstCharConstant();
char * insertFirtsChar();
void generateSumCode();
void freeStack();

struct stackStatements* stackIf;
struct stackOperators* stackOperator;
char auxString[200];
int auxCount = 0;
int wasOr = 0;

void generateAssembler(ast* tree) {
    ast* copy = tree;
    file = fopen("Final.asm", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    stackIf = (struct stackStatements*) malloc(sizeof(struct stackStatements)); 
    stackIf->array = (int*) malloc(5000* sizeof(int));
    stackOperator = (struct stackOperators*) malloc(sizeof(struct stackOperators));
    initiateCode();
    goThroughTree(copy);
    // free all st(?)
    freeStack();
    fclose(file);
}

void initiateCode() {
    strcpy(auxString,"*010101*"); // Código para que no printee FSTP

    fprintf(file,"include macros2.asm \n\n\n");
    fprintf(file,".MODEL LARGE\n");
    fprintf(file,".386\n");
    fprintf(file,".STACK 200h\n\n");
    fprintf(file,"MAXTEXTSIZE equ 40\n\n");
    fprintf(file,".DATA\n");

    printTsInAsm();

    fprintf(file, "\t_SUM\tdd\t?\n");
    fprintf(file, "\t_SUB\tdd\t?\n");
    fprintf(file, "\t_MUL\tdd\t?\n");
    fprintf(file, "\t_DIV\tdd\t?\n");

    fprintf(file, "\n\n.CODE\n");
}

void printTsInAsm() {
    symbolNode* current = symbolTable;
    while(current != NULL){
        if (strcmp(current->type, "STRING") == 0) {
           fprintf(file, "\t%s\tdb\tMAXTEXTSIZE dup (?),'$'\n", current->name);
        } else if (strcmp(current->type, "STRING_CTE") == 0) {
            fprintf(file, "\t%s\tdb\t'%s','$', %d dup (?)\n", current->name, current->value, current->length);
        } else if (strcmp(current->type, "INTEGER_CTE") == 0) {
            fprintf(file, "\t%s\tdd\t%s.0\n", current->name, current->value);
        } else if(strcmp(current->value,"-") == 0) {
            fprintf(file, "\t%s\tdd\t?\n", current->name);
        } else {
            fprintf(file, "\t%s\tdd\t%s\n", current->name, current->value);
        }
        current = current->next;
    }  
}

void goThroughTree(ast *root) {
    if ( root->left != NULL ) {
        goThroughTree (root->left);
    }
    if (strcmp(root->value,"NOT") == 0) {
        printOrJump(popOperator()); // SE HACE EL CONTRARIO PORQUE ES UN NOT
    } else if (strcmp(root->value,"OR") == 0) {
        printOrJump(popOperator());
        wasOr = 1;
        auxCount++;
    } else if (strcmp(root->value,"AND") == 0) {
        printJump(popOperator());
    } else if (strcmp(root->value,"IF") == 0) {
        printJump(popOperator());
        if (wasOr == 1) {
            swapTopStack(stackIf);
            printLabel("IF");
            wasOr = 0;
        }
        auxCount ++;
    } else if (strcmp(root->value,"CUERPO_IF") == 0) {
        fprintf(file,"\tJI IF_%d\n", auxCount);
        printLabel("IF");
        push(stackIf, auxCount);
    }
    if( root->right != NULL ) {
        goThroughTree (root->right);
    }
    generateCode(root);
}

void generateCode(ast* root) {
    // no va a funcar si usamos para alguno que tenga un solo hijo ocn null
    if(root->right != NULL && root->left  != NULL) {
        printf("\tLEFT=[%s]\t[%s]\tRIGHT[%s]\n", root->left->value, root->value, root->right->value);
        char operation[200];
        if (verifyIsCondition(root->value)) {


            generateCondition(root);



        } else if (strcmp(root->value,"IF") == 0) {
            printLabel(root->value);
        } else if (strcmp(root->value,"+") == 0) {
            strcpy(operation, "ADD");
            generateCodeOperation(root, &operation);
        } else if (strcmp(root->value,"*") == 0) {        
            strcpy(operation, "MUL");
            generateCodeOperation(root, &operation);
        } else if (strcmp(root->value,"/") == 0) {        
            strcpy(operation, "DIV");
            generateCodeOperation(root, &operation);
        } else if (strcmp(root->value,"-") == 0) {     
            strcpy(operation, "SUB"); 
            generateCodeOperation(root, &operation);
        } else if (strcmp(root->value,":=") == 0) {       
            if (strcmp(root->right->value,"_SUM") == 0 || strcmp(root->right->value,"_SUB") == 0 || strcmp(root->right->value,"_MUL") == 0 || strcmp(root->right->value,"_DIV") == 0) {
                generateCodeAsignation(root);
            } else {
                generateCodeAsignationSimple(root);
            }
        }
    }
}

void swapTopStack (struct stackStatements* s) {
    int poppedCount;
    int poppedCount1;
    poppedCount1 = pop(s);
    poppedCount = pop(s);
    push(stackIf, poppedCount1);
    push(stackIf, poppedCount);
}

void printLabel(char * operation){
    fprintf(file, "\n%s_%d:\n", operation, pop(stackIf));
}

int pop(struct stackStatements* stack) { 
    return stack->array[stack->top--]; 
}

void push(struct stackStatements* stack, int item) { 
    stack->array[++stack->top] = item; 
} 

char* popOperator() { 
    return stackOperator->array[stackOperator->top--]; 
}

void pushOperator(char* item) { 
    strcpy(stackOperator->array[++stackOperator->top],item); 
} 

void generateCondition(ast * root) {
    fprintf(file, "\t; Condition\n");
    fprintf(file,"\tFLD %s\n", root->left->value);
    fprintf(file,"\tFCOMP %s\n", root->right->value);
    fprintf(file,"\tFSTSW AX\n");
    fprintf(file,"\tSAHF\n");
    pushOperator(root->value);
}

void printJump(char * value){
    if (strcmp(value,">=") == 0) {    
        fprintf(file,"\tJL IF_%d\n", auxCount);
    }else if (strcmp(value,">") == 0) {      
        fprintf(file,"\tJLE IF_%d\n", auxCount);
    }else if (strcmp(value,"<=") == 0) {      
        fprintf(file,"\tJG IF_%d\n", auxCount);
    }else if (strcmp(value,"<") == 0) {      
        fprintf(file,"\tJGE IF_%d\n", auxCount);
    }else if (strcmp(value,"!=") == 0) {      
        fprintf(file,"\tJE IF_%d\n", auxCount);
    }else if (strcmp(value,"==") == 0) {      
        fprintf(file,"\tJNE IF_%d\n", auxCount);
    }
    push(stackIf, auxCount);
    fprintf(file,"\n");
}

void printOrJump(char * value){
    if (strcmp(value,">=") == 0) {    
        fprintf(file,"\tJGE IF_%d\n", auxCount);
    }else if (strcmp(value,">") == 0) {      
        fprintf(file,"\tJG IF_%d\n", auxCount);
    }else if (strcmp(value,"<=") == 0) {      
        fprintf(file,"\tJLE IF_%d\n", auxCount);
    }else if (strcmp(value,"<") == 0) {      
        fprintf(file,"\tJL IF_%d\n", auxCount);
    }else if (strcmp(value,"!=") == 0) {      
        fprintf(file,"\tJNE IF_%d\n", auxCount);
    }else if (strcmp(value,"==") == 0) {      
        fprintf(file,"\tJE IF_%d\n", auxCount);
    }
    push(stackIf, auxCount);
    fprintf(file,"\n");
}

void generateAndCondition(ast * root) {

}

void generateOrCondition(ast * root) {

}

int verifyIsCondition(char* value) {
    if (
        strcmp(value,">=") == 0 ||
        strcmp(value,">") == 0 || 
        strcmp(value,"<=") == 0 || 
        strcmp(value,"<") == 0 ||
        strcmp(value,"!=") == 0 ||
        strcmp(value,"==") == 0) {
            return 1; // is an operand
    }
    return 0; // not an operand
}

void generateCodeOperation(ast * root, char * operation) {

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
    fprintf(file, "\tFSTP %s\n", root->left->value); 
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