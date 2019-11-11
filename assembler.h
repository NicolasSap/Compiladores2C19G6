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

struct stackUntilCondition { 
    int top; 
    char array[30][600]; 
}; 

void printTsInAsm();
void endAssembler();
void swapTopStack();
void generateAssembler();
void generateCondition();
void generateCode();
void initiateCode();
int verifyIsCondition();
void goThroughTree();
void printLabel();
void printJump();
void printOrJump();
int pop();
void push();
char* popOperator();
void pushOperator();
char* popUntil();
void pushUntil();
void generateCodeOperation();
void generateCodeAsignation();
void generateCodeAsignationSimple();
char * removeFirstCharConstant();
char * insertFirtsChar();
void generateSumCode();
void freeStack();

struct stackStatements* stack;
struct stackStatements* repeatStack;
struct stackOperators* stackOperator;
struct stackUntilCondition* stackUntil;
char auxString[200];
char auxCondition[300];
int auxCount = 0;
int repeatCount = 0;
int wasOr = 0;
int wasAnd = 0;
int isUntil = 0;
int wasIfNot = 0;
int countIf = 0; // si es 0 printea como until, si es mayor printea como if

void generateAssembler(ast* tree) {
    ast* copy = tree;
    file = fopen("Final.asm", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    stack = (struct stackStatements*) malloc(sizeof(struct stackStatements)); 
    stack->array = (int*) malloc(5000* sizeof(int));
    stack->top = 0;

    repeatStack = (struct stackStatements*) malloc(sizeof(struct stackStatements)); 
    repeatStack->array = (int*) malloc(5000* sizeof(int));
    repeatStack->top = 0;

    stackOperator = (struct stackOperators*) malloc(sizeof(struct stackOperators));
    stackOperator->top = 0;

    stackUntil = (struct stackUntilCondition*) malloc(sizeof(struct stackUntilCondition));
    stackUntil->top = 0;

    initiateCode();
    goThroughTree(copy);
    // free all st(?)
    freeStack();
    endAssembler();
    fclose(file);
}


void initiateCode() {
    strcpy(auxString,"*010101*"); // Código para que no printee FSTP

    fprintf(file,"include macros2.asm \n");
    fprintf(file,"include number.asm \n\n\n");
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
    fprintf(file, "STRLEN PROC\n\tmov bx,0\nSTRL01:\n\tcmp BYTE PTR [SI+BX],'$'\n\tje STREND\n\tinc BX\n");
    fprintf(file, "\tcmp BX, MAXTEXTSIZE\n\tjl STRL01\nSTREND:\n\tret\nSTRLEN ENDP\n\nCOPY PROC\n");
    fprintf(file, "\tcall STRLEN\n\tcmp bx,MAXTEXTSIZE\n\tjle COPYSIZEOK\n\tmov bx,MAXTEXTSIZE\nCOPYSIZEOK:\n\tmov cx,bx\n");
    fprintf(file, "\tcld\n\trep movsb\n\tmov al,'$'\n\tmov BYTE PTR [DI],al\n\tret\nCOPY ENDP\n\n");

    fprintf(file, "\tbegin: .startup\n\n");
}

void endAssembler(){
    fprintf(file, "\n\n;END PROGRAM\n\tmov AX, 4C00h\n");
    fprintf(file, "\tint 21h\n\n");
    fprintf(file, "\tEND begin");
}

void printTsInAsm() {
    symbolNode* current = symbolTable;
    while(current != NULL){
        if (strcmp(current->type, "STRING") == 0) {
           fprintf(file, "\t%s\tdb\tMAXTEXTSIZE dup (?),'$'\n", current->name);
        } else if (strcmp(current->type, "STRING_CTE") == 0 || strcmp(current->type, "CONST_STRING") == 0) {
            fprintf(file, "\t%s\tdb\t'%s','$', %d dup (?)\n", current->name, current->value, current->length);
        } else if (strcmp(current->type, "INTEGER_CTE") == 0) {
            fprintf(file, "\t%s\tdd\t%s.0\n", current->name, current->value);
        } else if (strcmp(current->type, "FLOAT_CTE") == 0) {
            char * aux = current->name;
            while(*aux != '.'){
                aux ++;
            }
            *aux = '_';
            fprintf(file, "\t%s\tdd\t%s\n", current->name, current->value);            
        } else if(strcmp(current->value,"-") == 0) {
            fprintf(file, "\t%s\tdd\t?\n", current->name);
        } else {
            fprintf(file, "\t%s\tdd\t%s\n", current->name, current->value);
        }
        current = current->next;
    }  
}

void goThroughTree(ast *root) {
    char auxCond3[100]; // condicion a concatenar si es until con and u or
    char value [4]; // operando a popear si es until con and u or
    if (strcmp(root->value,"IF") == 0) {
        countIf++;
        if (strcmp(root->left->value,"NOT") == 0) {
            wasIfNot = 1;
        }
    } else if (strcmp(root->value,"UNTIL") == 0) {
        isUntil = 1;
        fprintf(file, "\nREPEAT_%d:\n", repeatCount);
        push(repeatStack, repeatCount);
        repeatCount++;
    } else if (strcmp(root->value,"AND") == 0) {
        push(stack, auxCount);
    }
    if ( root->left != NULL ) {
        goThroughTree (root->left);
    }
    if (strcmp(root->value,"NOT") == 0) {
        if (isUntil == 1 && wasIfNot == 0) {
            strcpy(value, popOperator());
            if (strcmp(value,">=") == 0) {    
                sprintf(auxCond3,"\tJB REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,">") == 0) {         
                sprintf(auxCond3,"\tJBE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<=") == 0) {      
                sprintf(auxCond3,"\tJA REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<") == 0) {      
                sprintf(auxCond3,"\tJAE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"!=") == 0) {      
                sprintf(auxCond3,"\tJE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"==") == 0) {      
                sprintf(auxCond3,"\tJNE REPEAT_%d\n", pop(repeatStack));
            }
            strcat(auxCondition,auxCond3);// concatenar el printOrJump a la variable auxCondition
        } else {
            printOrJump("IF", popOperator()); // SE HACE EL CONTRARIO PORQUE ES UN NOT
            wasIfNot = 0;
        }
    } else if (strcmp(root->value,"OR") == 0) {
        wasOr = 1;
        if ( isUntil == 0 ){
            printOrJump("IF", popOperator());
            auxCount++;
        } else {
            strcpy(value, popOperator());
            int index = repeatStack->array[repeatStack->top];
            push(repeatStack,index);
            if (strcmp(value,">=") == 0) {    
                sprintf(auxCond3,"\tJAE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,">") == 0) {      
                sprintf(auxCond3,"\tJA REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<=") == 0) {      
                sprintf(auxCond3,"\tJBE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<") == 0) {      
                sprintf(auxCond3,"\tJB REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"!=") == 0) {      
                sprintf(auxCond3,"\tJNE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"==") == 0) {      
                sprintf(auxCond3,"\tJE REPEAT_%d\n", pop(repeatStack));
            }
            strcat(auxCondition,auxCond3); // concatenar el printOrJump a la variable auxCondition
        }
    } else if (strcmp(root->value,"AND") == 0) {
        wasAnd = 1;
        if ( isUntil == 0 ){
            printJump("IF", popOperator());
        } else {
            strcpy(value, popOperator());
            push(repeatStack, repeatCount);
            if (strcmp(value,">=") == 0) {    
                sprintf(auxCond3,"\tJB REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,">") == 0) {      
                sprintf(auxCond3,"\tJBE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<=") == 0) {      
                sprintf(auxCond3,"\tJA REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"<") == 0) {      
                sprintf(auxCond3,"\tJAE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"!=") == 0) {      
                sprintf(auxCond3,"\tJE REPEAT_%d\n", pop(repeatStack));
            }else if (strcmp(value,"==") == 0) {      
                sprintf(auxCond3,"\tJNE REPEAT_%d\n", pop(repeatStack));
            }
            strcat(auxCondition,auxCond3);// concatenar el printOrJump a la variable auxCondition
        }
    } else if (strcmp(root->value,"IF") == 0) {
        printJump("IF", popOperator());
        if (wasOr == 1) {
            swapTopStack(stack);
            printLabel("IF", stack);
            wasOr = 0;
        }
        auxCount ++;
    } else if (strcmp(root->value,"CUERPO_IF") == 0) {
        fprintf(file,"\tJMP IF_%d\n", auxCount);
        printLabel("IF", stack);
        push(stack, auxCount);
    }
    if( root->right != NULL ) {
        goThroughTree (root->right);
    }
    generateCode(root);
}

void generateCode(ast* root) {
    // no va a funcar si usamos para alguno que tenga un solo hijo ocn null
    if (strcmp(root->value,"UNTIL") == 0) {
        pushUntil(auxCondition);
    } else if (strcmp(root->value,"PRINT") == 0) {
        symbolNode* symbol = findSymbol(root->left->value);
        if((strcmp(symbol->type, "STRING_CTE") == 0 || strcmp(symbol->type, "STRING") == 0 || strcmp(symbol->type, "CONST_STRING") == 0)) {
            fprintf(file,"\tdisplayString %s\n", root->left->value);
        } else if ((strcmp(symbol->type, "INT") == 0) || (strcmp(symbol->type, "FLOAT") == 0)) {
            fprintf(file,"\tdisplayFloat %s,2\n", root->left->value);
        } 
        fprintf(file,"\tnewLine 1\n");
    } else if(strcmp(root->value,"READ") == 0) {
        symbolNode* symbol = findSymbol(root->left->value);
        if((strcmp(symbol->type, "STRING_CTE") == 0 || strcmp(symbol->type, "STRING") == 0 || strcmp(symbol->type, "CONST_STRING") == 0)) {
            fprintf(file,"\tgetString %s\n", root->left->value);
        } else if ((strcmp(symbol->type, "INT") == 0) || (strcmp(symbol->type, "FLOAT") == 0)) {
            fprintf(file,"\tGetFloat %s\n", root->left->value);
        } 
    }
    if(root->right != NULL && root->left  != NULL) {
        char operation[200];
        if (verifyIsCondition(root->value)) {
            generateCondition(root);
        } else if (strcmp(root->value,"REPEAT") == 0) {
            strcpy(auxCondition, popUntil());
            fprintf(file, "%s", auxCondition);
            strcpy(auxCondition, "");
            printOrJump("REPEAT", popOperator());
            if (wasAnd == 1) {
                printLabel("REPEAT", repeatStack);
                wasAnd == 0;
            }
        } else if (strcmp(root->value,"IF") == 0) {
            printLabel("IF", stack);
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
    push(s, poppedCount1);
    push(s, poppedCount);
}

void printLabel(char * operation, struct stackStatements* s) {
    fprintf(file, "\n%s_%d:\n", operation, pop(s));
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

char* popUntil() { 
    return stackUntil->array[stackUntil->top--]; 
}

void pushUntil(char* item) { 
    strcpy(stackUntil->array[++stackUntil->top],item);
} 

void generateCondition(ast * root) {    
    if(strchr(root->left->value, '.') != NULL){
        char * aux = root->left->value;
        while(*aux != '.'){
            aux ++;
        }
        *aux = '_';
    }
    if(strchr(root->right->value, '.') != NULL){
        char * aux = root->right->value;
        while(*aux != '.'){
            aux ++;
        }
        *aux = '_';
    }
    // puede no ser until, o que sea until y que tenga if dentro
    if (isUntil == 0 || isUntil == 1 && countIf > 0) {
        sprintf(auxCondition, "\t; Condition\n\tFLD %s\n\tFCOMP %s\n\tFSTSW AX\n\tSAHF\n", root->left->value, root->right->value);
        fprintf(file, "%s", auxCondition);
        strcpy(auxCondition, "");
        countIf--;
    } else if (wasOr == 1 || wasAnd == 1) { // UNTIL CON AND U OR
        char auxCond2[100];
        sprintf(auxCond2, "\t; Condition\n\tFLD %s\n\tFCOMP %s\n\tFSTSW AX\n\tSAHF\n", root->left->value, root->right->value);
        strcat(auxCondition,auxCond2);
        wasOr = 0;
    } else if (wasOr == 0 || wasAnd == 0) { // UNTIL CON UNA CONDICION SOLA
        sprintf(auxCondition, "\t; Condition\n\tFLD %s\n\tFCOMP %s\n\tFSTSW AX\n\tSAHF\n", root->left->value, root->right->value);
    }
    pushOperator(root->value);
}

void printJump(char * operation, char * value) {
    int count;
    if (strcmp(operation, "REPEAT") == 0) { // es repeat
        count = repeatCount;
        push(repeatStack, repeatCount);
    } else {
        count = auxCount;
        push(stack, auxCount);
    }
    if (strcmp(value,">=") == 0) {   
        fprintf(file,"\tJB %s_%d\n", operation, count);
    }else if (strcmp(value,">") == 0) {      
        fprintf(file,"\tJBE %s_%d\n", operation, count);
    }else if (strcmp(value,"<=") == 0) {      
        fprintf(file,"\tJA %s_%d\n", operation, count);
    }else if (strcmp(value,"<") == 0) {      
        fprintf(file,"\tJAE %s_%d\n", operation, count);
    }else if (strcmp(value,"!=") == 0) {      
        fprintf(file,"\tJE %s_%d\n", operation, count);
    }else if (strcmp(value,"==") == 0) {      
        fprintf(file,"\tJNE %s_%d\n", operation, count);
    }
    
    fprintf(file,"\n");
}

void printOrJump(char * operation, char * value){
    int count;
    if (strcmp(operation, "REPEAT") == 0) { // es repeat
        count = pop(repeatStack);
        if (wasAnd == 1) {
            push(repeatStack, repeatCount);
        }
    } else {
        count = auxCount;
        push(stack, auxCount);
    }
    if (strcmp(value,">=") == 0) {    
        fprintf(file,"\tJAE %s_%d\n", operation, count);
    }else if (strcmp(value,">") == 0) {      
        fprintf(file,"\tJA %s_%d\n", operation, count);
    }else if (strcmp(value,"<=") == 0) {      
        fprintf(file,"\tJBE %s_%d\n", operation, count);
    }else if (strcmp(value,"<") == 0) {      
        fprintf(file,"\tJB %s_%d\n", operation, count);
    }else if (strcmp(value,"!=") == 0) {      
        fprintf(file,"\tJNE %s_%d\n", operation, count);
    }else if (strcmp(value,"==") == 0) {      
        fprintf(file,"\tJE %s_%d\n", operation, count);
    }
    fprintf(file,"\n");
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

    if(strchr(root->left->value, '.') != NULL){
        char * aux = root->left->value;
        while(*aux != '.'){
            aux ++;
        }
        *aux = '_';
    }
    if(strchr(root->right->value, '.') != NULL){
        char * aux = root->right->value;
        while(*aux != '.'){
            aux ++;
        }
        *aux = '_';
    }

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
    symbolNode* symbol = findSymbol(root->left->value);
    if((strcmp(symbol->type, "STRING") == 0)) {
        fprintf(file, "\tLEA SI, %s\n\tLEA DI,%s\n\tCALL COPY\n", root->right->value, root->left->value);
    } else {
        if(strchr(root->left->value, '.') != NULL){
            char * aux = root->left->value;
            while(*aux != '.'){
                aux ++;
            }
            *aux = '_';
        }
        if(strchr(root->right->value, '.') != NULL){
            char * aux = root->right->value;
            while(*aux != '.'){
                aux ++;
            }
            *aux = '_';
        }
        fprintf(file, "\t; Simple Asignation\n");
        fprintf(file, "\tFLD %s\n", root->right->value);
        fprintf(file, "\tFSTP %s\n", root->left->value); 
        strcpy(auxString,"*010101*"); // Código para que no printee FSTP
    }
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
}