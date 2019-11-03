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

void generateAssembler();
void generateCodeOperation();
void generateCodeAsignation();
char * removeFirstCharConstant();
char * insertFirtsChar();
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

int contador = 1;

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
    asigSymbol* aux = reverseList();
    i = length();
    for ( i; i>0; i--){
        insertMult(newNode(symbolAsig, newLeaf(aux->name), newLeaf(searchValue(aux))));
        deleteLastNode();
        aux = reverseList();
    }
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

void padding ( char ch, int n ){
  int i;

  for ( i = 0; i < n; i++ )
    putchar ( ch );
}

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

void generateCode(ast* root) {
  if(
        root->right != NULL &&
        root->left  != NULL
    ){
        char operation[200];
        if(strcmp(root->value,"+") == 0){
            strcpy(operation, "ADD");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"*") == 0){        
            strcpy(operation, "MUL");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"/") == 0){        
            strcpy(operation, "DIV");
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,"-") == 0){       
            strcpy(operation, "SUB"); 
            generateCodeOperation(root, &operation);
        }else if (strcmp(root->value,":=") == 0){       
            strcpy(operation, "MOV"); 
            generateCodeAsignation(root, &operation);
        }
    }
}

void generateCodeOperation(ast * root, char * operation){
    // Left leaf
    char * aux;
    if(root->left->value[0] == '@'){
        aux = root->left->value;
    }else if (root->left->value[0] == '_'){
        aux = removeFirstCharConstant(root->left->value);
    }else{
        aux = insertFirtsChar(root->left->value);
    }

    printf("MOV R1, %s\n", aux);
    free(aux);
    // Rigth leaf
    aux = (char *) malloc(strlen(root->right->value)+1);
    if(root->right->value[0] == '@'){
        aux = root->left->value;
    }else if (root->right->value[0] == '_'){
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

char * removeFirstCharConstant(char * constant){
    char * aux = (char *) malloc(strlen(constant)-1);
    strcpy(aux, ++constant);
    return aux;
}

char * insertFirtsChar(char * constant){
    char * aux = (char *) malloc(strlen(constant)+1);
    aux[0] = '_';
    strcat(aux,constant);
    return aux;
}

void generateCodeAsignation(ast * root, char * operation){
    char * aux;
    aux = (char *) malloc(strlen(root->right->value)+1);
    if(root->right->value[0] == '@'){
        aux = root->left->value;
    }else if (root->right->value[0] == '_'){
        aux = removeFirstCharConstant(root->right->value);
    }else{
        aux = insertFirtsChar(root->right->value);
    }

    printf("%s R1, %s\n",operation,aux);
    printf("MOV _%s, R1\n", root->left->value);
}


void goThroughTree (ast *root) {
    if ( root->left != NULL ) {
        goThroughTree (root->left);
    }
    if( root->right != NULL ) {
        goThroughTree (root->right);
    }
    generateCode  (root);
}

void generateAssembler(ast* tree) {
    ast* copy = tree;
    file = fopen("assembler-code.asm", "w");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    goThroughTree(copy);
    fclose(file);
}
