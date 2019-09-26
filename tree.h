#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h> 

// Ast stands for Abstract Syntax Tree

typedef struct treeNode {
    char* value;
    struct treeNode* left;
    struct treeNode* right;
} ast;

typedef struct listAst {
    ast* tree;
    struct listAst* next;
} l_ast;

typedef struct listAsig {
    char name[220];
    char v[220];
    int i;
    float f;
    struct listAsig* next;
} asigSymbol;

void printAndSaveAST();
void printAST();
ast* newNode();
ast* newLeaf();
ast* incrustarArbol();

FILE *file;

l_ast* listaAsigM = NULL;
asigSymbol* reverseList();
asigSymbol* asigMList;
void clearAsigMList();
void printAsigM();
int cantAvanzar();
void deleteLastNode();
ast* finishAsig();
void printM();
l_ast* insertMult();

char* searchValue(asigSymbol* a) {
    char s[220];
    int len;
    char* aux;
    strcpy(s,"");
    if (strcmp(a->v,"") == 0) {
        if (a->i == 0) {
            if (a->f != 0.00) {
                gcvt (a->f, 7, s);
                len = strlen(s);
                aux = (char*) malloc(len+1);
                strcpy(aux,s);
                s[0] = '_' ;
                concatenate(s, aux);
            }
        } else {
            itoa(a->i,s,10);
            len = strlen(s);
            aux = (char*) malloc(len+1);
            strcpy(aux,s);
            s[0] = '_' ;
            concatenate(s, aux);
        }
    } else {
        strcpy(s,a->v);
    }  
    return strdup(s);
}

l_ast* insertMult(ast* t) {
    l_ast* node = (l_ast*) malloc(sizeof(l_ast));
    node->tree = t;
    node->next = listaAsigM;
    listaAsigM = node;
}

ast* finishAsig() {
    char* phraseAsig = "Asig_M";
    l_ast* current = listaAsigM;
    ast* node = current->tree;
    if(current->next != NULL) {
        current = current->next;
    } else {
        return node;
    }
    while (current != NULL) {
        node = newNode(phraseAsig, current->tree, node);
        current = current->next;
    }
    return node;
}

void printM() {
    l_ast* current = listaAsigM;
    while (current != NULL) {
        printf("\n-----\n%s - %s - %s\n-----\n", current->tree->value, current->tree->left->value, current->tree->right->value);
        current = current->next;
    } 
}

int length() {
    int cant = 0;
    asigSymbol* aux = asigMList;
    while (aux != NULL) {
        cant++;
        aux = aux->next;
    } 
    return cant;
}

ast* incrustarArbol() {
    char* symbolAsig = ":=";
    ast* asigT;
    int i;
    printAsigM(asigMList);
    asigSymbol* aux = reverseList();
    i = length();
    for ( i; i>0; i--){
        insertMult(newNode(symbolAsig, newLeaf(aux->name), newLeaf(searchValue(aux))));
        deleteLastNode();
        aux = reverseList();
    }
    printM();
    return finishAsig();
}

void deleteLastNode() {
    asigSymbol* aux = asigMList;
    asigSymbol* ant = asigMList;
    while (aux->next != NULL) {
        ant = aux;
        aux = aux->next;
    }
    ant->next = NULL;
    free(aux);
}

asigSymbol* reverseList() {
    asigSymbol* aux = asigMList;
    asigSymbol* ant = asigMList;
    while (aux != NULL) {
        ant = aux;
        aux = aux->next;
    } 
    return ant;
}

int cantAvanzar() {
    int cant = 0;
    asigSymbol* aux = asigMList;
    while (aux != NULL) {
        if (strcmp(aux->v,"") == 0) {
            if (aux->i == 0) {
                if (aux->f == 0.00) {
                    cant++;
                }
            }
        }  
        aux = aux->next;
    } 
    return cant-1;
}

asigSymbol* insertAsigM(char* value, int choice, float f, int i, int t) {
    asigSymbol* node = (asigSymbol*) malloc(sizeof(asigSymbol));
    asigSymbol* aux = asigMList;
    int avanzar;
    if (choice == 1) {
        int len = strlen(value);
        char* valueToInsert = (char*) malloc(len+1);
        strcpy(valueToInsert, value);
        strcpy(node->name, valueToInsert);
        node->next = asigMList;
        asigMList = node;
    } else if (choice == 2) {
        avanzar = cantAvanzar();
        while (avanzar != 0) {
            aux = aux->next;
            avanzar--;
        } 
        node = aux;
        if (t == 1){ //string
            int len;
            if (value[0] == '"') {
                removeChar(value, '"');
                len = strlen(value);
                char* aux = (char*) malloc(len+1);
                strcpy(aux,value);
                value[0] = '_' ;
                concatenate(value, aux);
            }
            len = strlen(value);
            char* valueToInsert = (char*) malloc(len+1);
            strcpy(valueToInsert, value);
            strcpy(node->v, valueToInsert);
        } else if (t == 2){// entero
            node->i = i;
        } else if (t == 3){// float
            node->f = f;
        }
    }
    return node;
}

void clearAsigMList() {
    asigMList = NULL;
}

void printAsigM(asigSymbol* a) {
    asigSymbol* current = a;
    printf("\n LISTA ASIG MULT \n");
    while(current != NULL){
        printf("%s - %s - %f - %d\n", current->name, current->v, current->f, current->i);
        current = current->next;
    }   
}

ast* newNode(char* operation, ast* leftNode, ast* rightNode) {
    ast* node = (ast*) malloc(sizeof(ast));
    node->value = operation;
    node->left = leftNode;
    node->right = rightNode;
    return node;
}


ast* newLeaf(char* value) {
    ast* node = (ast*) malloc(sizeof(ast));
   
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void printAST(ast* tree) { 
     if (tree == NULL) 
          return; 
  
     /* first recur on left child */
     printAST(tree->left); 
  
     /* then print the data of node */
     //printf("%s ", tree->value);
     fprintf(file, "%s ", tree->value);
  
     /* now recur on right child */
     printAST(tree->right); 
} 


void printAndSaveAST(ast* tree) {
    ast* copy = tree;
    file = fopen("intermedia.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    printAST(copy);
    fclose(file);
}