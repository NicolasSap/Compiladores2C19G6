#include <stdlib.h>
#include <string.h>
#include <ctype.h> 
#include <stdio.h>

typedef struct listSymbol {
    char name[220];
    char type[220];
    char value[220];
    int length;
    struct listSymbol* next;
} symbolNode;

typedef struct symbolIdentifier {
    char *value;
    struct symbolIdentifier* next;
} identifierNode;

symbolNode* symbolTable;
symbolNode* insert();
symbolNode* findSymbol();
symbolNode* fSymbol();
void deleteIfSizeDiff();
void putTypeIdentifierOnSymbolTable();
void concatenate();
void removeChar();
void saveTable();
void printTable();
void printTable1();
char* getSymbolName();
// Symbol Identifier auxiliars
identifierNode* identifierList;
identifierNode* identifierTypeList;
identifierNode* insertIdentifier();
identifierNode* findIdentifier();
void clearIdentifierList();
void clearTypeIdentifierList();

symbolNode* insert(char* value) {
    symbolNode* foundNode = findSymbol(value);
    if (foundNode != NULL) {
        return foundNode;
    }
    
    symbolNode* node = (symbolNode*) malloc(sizeof(symbolNode));
    int len = strlen(value);
    char* valueToInsert = (char*) malloc(len+1);
    strcpy(valueToInsert, value);
    
    // Is it a string constant?
    int isConstant = 0;
    if (valueToInsert[0] == '"') {
        removeChar(valueToInsert, '"');
        node->length = strlen(valueToInsert);
        isConstant = 1;
        strcpy(node->type, "STRING_CTE");
        // Is it a float constant?
    } else if (strchr(valueToInsert, '.') != NULL) {
        isConstant = 1;
        strcpy(node->type, "FLOAT_CTE");
        // Is it a integer constant?
    } else if (isdigit(valueToInsert[0]) != 0) {
        isConstant = 1;
        strcpy(node->type, "INTEGER_CTE");        
    }
    
    if (isConstant == 1) {
        strcpy(node->name, "");
        node->name[0] = '_' ;
        concatenate(node->name, valueToInsert);
    } else {
        strcpy(node->name, valueToInsert);
    }

    if (isConstant == 1) {
        strcpy(node->value, valueToInsert);
    }

    if(symbolTable == NULL) {
        strcpy(node->type,"");
        strcpy(node->value,"");
        node->length = 0;
    }

    node->next = symbolTable;
    symbolTable = node;
    return node;
}

identifierNode* insertIdentifier(char *name) {
    identifierNode* foundNode = findIdentifier(name);
    if (foundNode != NULL) {
        return foundNode;
    }
    identifierNode* node = (identifierNode*) malloc(sizeof(identifierNode));
    int len = strlen(name);
    char* valueToInsert = (char *) malloc(len+1);
    strcpy(valueToInsert, name);
    node->value = valueToInsert;
    node->next = identifierList;
    identifierList = node;
    return node;
}

identifierNode* insertTypeIdentifier(char* name) {
    identifierNode* node = (identifierNode*) malloc(sizeof(identifierNode));
    int len = strlen(name);
    char* valueToInsert = (char *) malloc(len+1);
    strcpy(valueToInsert, name);
    node->value = valueToInsert;
    node->next = identifierTypeList;
    identifierTypeList = node;
    return node;
}

identifierNode* findIdentifier(char* value) {
    identifierNode* identifierNode = identifierList;
    while(identifierNode != NULL) {
        if (strcmp(value, identifierNode->value) == 0) {
            return identifierNode;
        }
        identifierNode = identifierNode->next;
    }
    return NULL;
}

void deleteFromTS(char* val) {
    symbolNode* aux = symbolTable;
    symbolTable = symbolTable->next;
    free(aux);
}

void deleteIfSizeDiff(identifierNode* list, identifierNode* list1) { // borra el sobrante de la lista
    identifierNode* l = list;
    identifierNode* d = list;
    identifierNode* l1 = list1;
    identifierNode* d1 = list1;
    int cant = 0;
    int cant1 = 0;
    while(l != NULL){
        cant++;
        l = l->next;
    }
    while(l1 != NULL){
        cant1++;
        l1 = l1->next;
    }
    if(cant > cant1) {
        // antes deberia mandarlo a sacar a la lista de simbolos
        //borrar el primer id
        deleteFromTS(d->value);
        identifierList = identifierList->next;
        free(d);
    }else if (cant < cant1){
        // borrar el primer tipo dato
        identifierTypeList = identifierTypeList->next;
        free(d1);
    }
}

void putTypeIdentifierOnSymbolTable() {
    deleteIfSizeDiff(identifierList, identifierTypeList);
    identifierNode* identifierTypeNode = identifierTypeList;
    identifierNode* identifierNode = identifierList;
    while(identifierNode != NULL && identifierTypeNode != NULL) {
        symbolNode* symbol = findSymbol(identifierNode->value);
        // Symbol should never be NULL but just in case..
        if (symbol != NULL) {
            if (strlen(symbol->type) != 0) {
                fprintf(stderr, "\n ERROR: Variable %s has been already declared", symbol->name);
                exit(1);
            }
            int len = strlen(identifierTypeNode->value);
            char* valueToInsert = (char*) malloc(len+1);
            strcpy(valueToInsert, identifierTypeNode->value);
            strcpy(symbol->type, valueToInsert);
            strcpy(symbol->value, "-");
        }
        identifierNode = identifierNode->next;
        identifierTypeNode = identifierTypeNode->next;
    }
    clearIdentifierList();
    clearTypeIdentifierList();
}

void putConstOnSymbolTable(char* id, char* value, int numb, float fnumb, char* type, int option) {
    symbolNode* symbol = fSymbol(id);
    // Symbol should never be NULL but just in case..
    if (symbol != NULL) {
        if (strlen(symbol->type) != 0) {
            fprintf(stderr, "\n ERROR: Variable %s has been already declared", symbol->name);
            exit(1);
        }
        int len = strlen(type);
        char* valueToInsert = (char*) malloc(len+1);
        strcpy(valueToInsert, type);
        strcpy(symbol->type, valueToInsert);
        if (option == 1) {
            char s[220];
            itoa(numb, s, 10);
            strcpy(symbol->value,s);
            symbol->length = 0;
        } else if (option == 2) {
            removeChar(value,'"');
            strcpy(symbol->value, value);
            symbol->length = strlen(value);
        } else if (option == 3) {
            char s[220];
            gcvt (fnumb, 7, s);
            strcpy(symbol->value,s);
            symbol->length = 0;
        }
        
    }
}

void clearIdentifierList() {
    identifierList = NULL;
}

void clearTypeIdentifierList() {
    identifierTypeList = NULL;
}

void saveTable() {
    FILE *file = fopen("ts.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    symbolNode* current = symbolTable;
    fprintf(file, "NOMBRE\tTIPODATO\tVALOR\tLONGITUD\n");
    while(current != NULL) {
        fprintf(file, "%s\t%s\t%s\t%d\n", current->name, current->type, current->value, current->length);
        current = current->next;
    }
    fclose(file);
}

void concatenate(char* original, char* add) {
    original++;     
    while(*add){
        *original = *add;
        add++;
        original++;
    }
    *original = '\0';
}

void removeChar(char *s, int c){ 
    int j, n = strlen(s); 
    int i;
    for (i=j=0; i<n; i++) {
        if (s[i] != c) {
            s[j++] = s[i];
        } 
    } 
    s[j] = '\0'; 
}

symbolNode* findSymbol(char* value) {
    symbolNode* tableNode = symbolTable;
    while(tableNode != NULL) {
        if ((tableNode->value != NULL && strcmp(value, tableNode->value) == 0) || (strcmp(value, tableNode->name) == 0)) {
            return tableNode;
        }
        tableNode = tableNode->next;
    }
    return NULL;
}

symbolNode* fSymbol(char* value) {
    symbolNode* tableNode = symbolTable;
    while(tableNode != NULL) {
        if (tableNode->value != NULL && (strcmp(value, tableNode->name) == 0)) {
            return tableNode;
        }
        tableNode = tableNode->next;
    }
    return NULL;
}

char* getSymbolName(void *symbolPointer, int type) {
    char symbol[220];
    int integerValue;
    float floatValue;
    switch (type) {
        case 1:
            integerValue = *(int*)symbolPointer;
            itoa(integerValue, symbol, 10);
            break;
        case 2:
            floatValue = *(float*)symbolPointer;
            gcvt (floatValue, 7, symbol);
            break; 
        case 3:
            strcpy(symbol, (char*)symbolPointer);
            removeChar(symbol, '"');
    }
    symbolNode* node = findSymbol(symbol);
    if (node == NULL) {
        fprintf(stderr, "\n ERROR: symbol %s not found", symbol);
        exit(1);
    }
    return strdup(node->name); 
}

void printTable() {
    symbolNode* current = symbolTable;
    printf("\n TABLA DE SIMBOLOS \n");
    printf("\nNOMBRE\tTIPODATO\tVALOR\tLONGITUD\n");
    while(current != NULL){
        printf("%s\t%s\t%s\t%d\n", current->name, current->type, current->value, current->length);
        current = current->next;
    }   
}

void printTable1(identifierNode* a) {
    identifierNode* current = a;
    printf("\n LISTA DE TIPO DATO O ID \n");
    printf("\nVALOR\n");
    while(current != NULL){
        printf("%s\n", current->value);
        current = current->next;
    }   
}